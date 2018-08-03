#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/gpio.h>
#include <linux/device.h>

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#endif
static unsigned int lcm_rst_pin;
static unsigned int lcm_gpio_enn;
static unsigned int lcm_gpio_enp;

#define AUXADC_LCM_VOLTAGE_CHANNEL     12
#define THREE_POWER_MIN_VOLTAGE (0)	
#define THREE_POWER_MAX_VOLTAGE (100)	

#define  TRUE   1 
#define  FALSE  0
#define FRAME_WIDTH                                          (720)
#define FRAME_HEIGHT                                         (1280)

#define REGFLAG_DELAY                                         0XFFE
#define REGFLAG_END_OF_TABLE                      			 0x1FF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE                                    0

//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

static LCM_UTIL_FUNCS lcm_util = {0};

//#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
static void lcm_rst_pin_set(unsigned int GPIO, unsigned int output);
#define SET_RESET_PIN(v)    lcm_rst_pin_set(lcm_rst_pin,v)//(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 				(lcm_util.udelay(n))
#define MDELAY(n) 				(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)		lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)			lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)							lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg                                            lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

#ifndef BUILD_LK
extern atomic_t ESDCheck_byCPU;
#endif

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{

	memset(params, 0, sizeof(LCM_PARAMS));
	
	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;


#if (LCM_DSI_CMD_MODE)
	params->dsi.mode   = CMD_MODE;
#else
	params->dsi.mode   = SYNC_PULSE_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 
#endif
	
	// DSI
	/* Command mode setting */
	//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	// Video mode setting		
	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		
	params->dsi.vertical_sync_active				= 8;//4;//calm lcd noise
	params->dsi.vertical_backporch					= 18;//10;//calm lcd noise
	params->dsi.vertical_frontporch					= 20;//10;//calm lcd noise
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 20;// 50  2
		params->dsi.horizontal_backporch				= 100;//90
		params->dsi.horizontal_frontporch				= 80;//90
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		params->dsi.ssc_disable                         = 1;
		params->dsi.HS_TRAIL                             = 15;
		/*ui = 1000/(dis_params->PLL_CLOCK*2) + 0x01; 
		cycle_time = 8000/(dis_params->PLL_CLOCK*2) + 0x01;
		HS_TRAIL = (0x04*ui + 0x50)/cycle_time;*/
	//params->dsi.LPX=8; 
	
		params->physical_width = 62;
		params->physical_height = 110;

	// Bit rate calculation
		//1 Every lane speed
		//params->dsi.pll_select=1;
		//params->dsi.PLL_CLOCK  = LCM_DSI_6589_PLL_CLOCK_377;
		params->dsi.PLL_CLOCK=230;//208//270
		//params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		//params->dsi.pll_div2=0;		// div2=0,1,2,3;div1_real=1,2,4,4	

	//	params->dsi.fbk_div =7;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	
		//params->dsi.compatibility_for_nvk = 1;		// this parameter would be set to 1 if DriverIC is NTK's and when force match DSI clock for NTK's
#if 0
        params->dsi.esd_check_enable = 0;
		params->dsi.customization_esd_check_enable = 0;
		params->dsi.lcm_esd_check_table[0].cmd			= 0x0a;
		params->dsi.lcm_esd_check_table[0].count		= 1;
		params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
#else
		params->dsi.esd_check_enable = 1;
		params->dsi.customization_esd_check_enable = 1;
		params->dsi.lcm_esd_check_table[0].cmd			= 0x0a;
		params->dsi.lcm_esd_check_table[0].count		= 1;
		params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
#endif
}

//static unsigned int vcom = 0x30;
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;
	
	for(i = 0; i < count; i++) 
	{   
		unsigned cmd;
		cmd = table[i].cmd;

		switch (cmd)
		{
			case REGFLAG_DELAY :
				MDELAY(table[i].count);
				break;
			case REGFLAG_END_OF_TABLE :
				break;
			/*case 0xd9 :
				table[i].para_list[0] = vcom;
				vcom +=2;
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
				break;*/
			default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}
}

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
{0x28, 0, {}},
{REGFLAG_DELAY, 20, {}},
{0x10, 0, {}},
{REGFLAG_DELAY, 120, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_initialization_setting[] = {
{0xFF,03,{0x98,0x81,0x03}},
{0x01,01,{0x00}},
{0x02,01,{0x00}},
{0x03,01,{0x56}},
{0x04,01,{0x13}},
{0x05,01,{0x00}},
{0x06,01,{0x06}},
{0x07,01,{0x01}},
{0x08,01,{0x00}},
{0x09,01,{0x30}},
{0x0A,01,{0x01}},
{0x0B,01,{0x00}},
{0x0C,01,{0x30}},
{0x0D,01,{0x01}},
{0x0E,01,{0x00}},
{0x0F,01,{0x18}},
{0x10,01,{0x18}},
{0x11,01,{0x00}},
{0x12,01,{0x00}},
{0x13,01,{0x00}},
{0x14,01,{0x00}},
{0x15,01,{0x00}},
{0x16,01,{0x00}},
{0x17,01,{0x00}},
{0x18,01,{0x00}},
{0x19,01,{0x00}},
{0x1A,01,{0x00}},
{0x1B,01,{0x00}},
{0x1C,01,{0x00}},
{0x1D,01,{0x00}},
{0x1E,01,{0x40}},
{0x1F,01,{0xC0}},
{0x20,01,{0x02}},
{0x21,01,{0x05}},
{0x22,01,{0x02}},
{0x23,01,{0x00}},
{0x24,01,{0x86}},
{0x25,01,{0x85}},
{0x26,01,{0x00}},
{0x27,01,{0x00}},
{0x28,01,{0x3B}},
{0x29,01,{0x03}},
{0x2A,01,{0x00}},
{0x2B,01,{0x00}},
{0x2C,01,{0x00}},
{0x2D,01,{0x00}},
{0x2E,01,{0x00}},
{0x2F,01,{0x00}},
{0x30,01,{0x00}},
{0x31,01,{0x00}},
{0x32,01,{0x00}},
{0x33,01,{0x00}},
{0x34,01,{0x00}},
{0x35,01,{0x00}},
{0x36,01,{0x00}},
{0x37,01,{0x00}},
{0x38,01,{0x00}},
{0x39,01,{0x00}},
{0x3A,01,{0x00}},
{0x3B,01,{0x00}},
{0x3C,01,{0x00}},
{0x3D,01,{0x00}},
{0x3E,01,{0x00}},
{0x3F,01,{0x00}},
{0x40,01,{0x00}},
{0x41,01,{0x00}},
{0x42,01,{0x00}},
{0x43,01,{0x00}},
{0x44,01,{0x00}},
{0x50,01,{0x01}},
{0x51,01,{0x23}},
{0x52,01,{0x45}},
{0x53,01,{0x67}},
{0x54,01,{0x89}},
{0x55,01,{0xAB}},
{0x56,01,{0x01}},
{0x57,01,{0x23}},
{0x58,01,{0x45}},
{0x59,01,{0x67}},
{0x5A,01,{0x89}},
{0x5B,01,{0xAB}},
{0x5C,01,{0xCD}},
{0x5D,01,{0xEF}},
{0x5E,01,{0x11}},
{0x5F,01,{0x08}},
{0x60,01,{0x00}},
{0x61,01,{0x01}},
{0x62,01,{0x02}},
{0x63,01,{0x02}},
{0x64,01,{0x0F}},
{0x65,01,{0x0E}},
{0x66,01,{0x0D}},
{0x67,01,{0x0C}},
{0x68,01,{0x02}},
{0x69,01,{0x02}},
{0x6A,01,{0x02}},
{0x6B,01,{0x02}},
{0x6C,01,{0x02}},
{0x6D,01,{0x02}},
{0x6E,01,{0x06}},
{0x6F,01,{0x02}},
{0x70,01,{0x02}},
{0x71,01,{0x02}},
{0x72,01,{0x02}},
{0x73,01,{0x02}},
{0x74,01,{0x02}},
{0x75,01,{0x06}},
{0x76,01,{0x00}},
{0x77,01,{0x01}},
{0x78,01,{0x02}},
{0x79,01,{0x02}},
{0x7A,01,{0x0F}},
{0x7B,01,{0x0E}},
{0x7C,01,{0x0D}},
{0x7D,01,{0x0C}},
{0x7E,01,{0x02}},
{0x7F,01,{0x02}},
{0x80,01,{0x02}},
{0x81,01,{0x02}},
{0x82,01,{0x02}},
{0x83,01,{0x02}},
{0x84,01,{0x08}},
{0x85,01,{0x02}},
{0x86,01,{0x02}},
{0x87,01,{0x02}},
{0x88,01,{0x02}},
{0x89,01,{0x02}},
{0x8A,01,{0x02}},


{0xFF,03,{0x98,0x81,0x04}},

{0x69,01,{0x57}},
{0x33,01,{0x44}},

{0x6C,01,{0x15}},
{0x6E,01,{0x2B}},
{0x6F,01,{0x33}},
{0x3A,01,{0xA4}},
{0x8D,01,{0x15}},
{0x87,01,{0xBA}},

{0x88,01,{0x0b}},

{0x26,01,{0x76}},
{0xB2,01,{0xD1}},
{0xB5,01,{0x06}},


//****

{0xFF,03,{0x98,0x81,0x01}},
{0x22,01,{0x0A}},
{0x31,01,{0x00}},
{0x53,01,{0x7D}},
{0x55,01,{0x7D}},
              
{0x50,01,{0xAF}},
{0x51,01,{0xA8}},
{0x60,01,{0x30}},
              
{0xA0,01,{0x15}},
{0xA1,01,{0x24}},
{0xA2,01,{0x31}},
{0xA3,01,{0x13}},
{0xA4,01,{0x15}},
{0xA5,01,{0x28}},
{0xA6,01,{0x1C}},
{0xA7,01,{0x1E}},
{0xA8,01,{0x95}},
{0xA9,01,{0x1C}},
{0xAA,01,{0x28}},
{0xAB,01,{0x84}},
{0xAC,01,{0x1E}},
              
{0xAD,01,{0x1D}},
{0xAE,01,{0x51}},
{0xAF,01,{0x26}},
{0xB0,01,{0x2A}},
{0xB1,01,{0x4E}},
{0xB2,01,{0x6C}},
{0xB3,01,{0x3F}},
              
{0xC0,01,{0x08}},
{0xC1,01,{0x24}},
{0xC2,01,{0x32}},
{0xC3,01,{0x14}},
{0xC4,01,{0x15}},
{0xC5,01,{0x28}},
              
{0xC6,01,{0x1C}},
{0xC7,01,{0x1E}},
{0xC8,01,{0x95}},
{0xC9,01,{0x1C}},
{0xCA,01,{0x29}},
{0xCB,01,{0x85}},
{0xCC,01,{0x1E}},
{0xCD,01,{0x1D}},
{0xCE,01,{0x51}},
{0xCF,01,{0x25}},
{0xD0,01,{0x2A}},
{0xD1,01,{0x4E}},
{0xD2,01,{0x6C}},
              
{0xD3,01,{0x3F}},

{0xFF,03,{0x98,0x81,0x00}},
{0x11,0,{}},                // Sleep-Out
{REGFLAG_DELAY,200,{}},
{0x29,0,{}}, 
{REGFLAG_END_OF_TABLE, 0x00, {}}  
};
static void lcm_get_pin(void)
{
 static struct device_node *node;
 node = of_find_compatible_node(NULL, NULL, "mediatek,lcm_b50016_gpio_node");
 lcm_rst_pin = of_get_named_gpio(node, "lcm_rst_pin", 0);
 lcm_gpio_enn = of_get_named_gpio(node, "lcm_gpio_enn", 0);
  lcm_gpio_enp = of_get_named_gpio(node, "lcm_gpio_enp", 0);
  gpio_request(lcm_rst_pin, "lcm_rst_pin");
  gpio_request(lcm_gpio_enn, "lcm_gpio_enn");
 gpio_request(lcm_gpio_enp, "lcm_gpio_enp");

}

static void lcm_rst_pin_set(unsigned int GPIO, unsigned int output)
{
	
	lcm_get_pin();
	gpio_set_value(GPIO, output);
}


#if 0
static void lcm_update(unsigned int x, unsigned int y,
        unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[16];

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
    dsi_set_cmdq(data_array, 3, 1);
    
    data_array[0]= 0x00053902;
    data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
    data_array[2]= (y1_LSB);
    dsi_set_cmdq(data_array, 3, 1);
    
    data_array[0]= 0x00290508;
    dsi_set_cmdq(data_array, 1, 1);
    
    data_array[0]= 0x002c3909;
    dsi_set_cmdq(data_array, 1, 0);
}
#endif

static void lcm_init(void)
{
	#ifdef BUILD_LK
		printf("sym  lcm_init start\n");
	#else
		printk("sym  lcm_init  start\n");
	#endif

 lcm_get_pin();

//gpio_direction_input(GPIO_READ_ID_PIN);//����Ϊ����
//gpio_get_value(GPIO_READ_ID_PIN);//��ȡ�����ƽ�����Լ�Ū����������һ�¡�
  gpio_direction_output(lcm_gpio_enn, 0);//��Ϊ��� 	
   	MDELAY(10);
  gpio_direction_output(lcm_gpio_enp, 0);//��Ϊ���
 	MDELAY(10);
	gpio_set_value(lcm_gpio_enn, 1);//��Ϊ����ߵ�ƽ
	MDELAY(10);
	gpio_set_value(lcm_gpio_enp, 1);//��Ϊ����ߵ�ƽ
	MDELAY(30);
//	ktd2150_write_bytes(addr,data);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
	#ifdef BUILD_LK
		printf("sym  lcm_init end\n");
	#else
		printk("sym  lcm_init end\n");
	#endif
}

static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	
	SET_RESET_PIN(1);
	MDELAY(10); 
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);	
	MDELAY(120);
	
	
//	ktd2150_write_bytes(addr,data);
	MDELAY(50);

//gpio_direction_input(GPIO_READ_ID_PIN);//����Ϊ����
//gpio_get_value(GPIO_READ_ID_PIN);//��ȡ�����ƽ�����Լ�Ū����������һ�¡�
 gpio_direction_output(lcm_gpio_enn, 0);//��Ϊ���
 	MDELAY(10);
 gpio_direction_output(lcm_gpio_enp, 0);//��Ϊ���
	MDELAY(10);
	gpio_set_value(lcm_gpio_enn, 0);//��Ϊ���low��ƽ
	MDELAY(10);
	gpio_set_value(lcm_gpio_enp, 0);//��Ϊ���low��ƽ
MDELAY(10);

}


static void lcm_resume(void)
{
    lcm_init();
}
#if 0
static unsigned int lcm_compare_id(void)
	{
		
		int array[4];
		char buffer[3];
		//char id_high=0;
		//char id_low=0;
		int id=0;
	
		SET_RESET_PIN(1);
		MDELAY(20); 
		SET_RESET_PIN(0);
		MDELAY(20);
		SET_RESET_PIN(1);
		MDELAY(120);
	
		//{0x39, 0xFF, 5, { 0xFF,0x98,0x06,0x04,0x01}}, // Change to Page 1 CMD
		array[0] = 0x00043902;
		array[1] = 0x018198FF;
		dsi_set_cmdq(array, 2, 1);
	
		array[0] = 0x00013700;
		dsi_set_cmdq(array, 1, 1);
		read_reg_v2(0x00, &buffer[0], 1);  //0x98
	
		id = buffer[0];
	
		
   #ifdef BUILD_LK
		  
			printf("jinmin [lk]=%d %d %d \n", buffer[0],buffer[1], buffer[2]);
			printf("id =0x%x\n", id);
#else
			printk("jinmin [kernel]=%d %d %d \n", buffer[0],buffer[1], buffer[2]);
			printk("id =0x%x\n", id);
   #endif
	
		return (0x98 == id)?1:0;
	
	}
#endif


// zhoulidong  add for lcm detect (start)
static unsigned int rgk_lcm_compare_id(void)
{
    int data[4] = {0,0,0,0};
    int res = 0;
    int rawdata = 0;
    int lcm_vol = 0;

#ifdef AUXADC_LCM_VOLTAGE_CHANNEL
    res = IMM_GetOneChannelValue(AUXADC_LCM_VOLTAGE_CHANNEL,data,&rawdata);
    if(res < 0)
    { 
			#ifdef BUILD_LK
			printf("[adc_uboot]: jinmin get data error\n");
			#endif
			return 0;
    }
#endif

    lcm_vol = data[0]*1000+data[1]*10;

    #ifdef BUILD_LK
    printf("[adc_uboot]: jinmin lcm_vol= %d\n",lcm_vol);
    #endif
	
    if (lcm_vol>=THREE_POWER_MIN_VOLTAGE &&lcm_vol <= THREE_POWER_MAX_VOLTAGE)// && lcm_compare_id())
    { 	
    	 #ifdef BUILD_LK
    	 printf("lxf is at four power\n");
    	 #else 
    	 printk("lxf is at four power\n");
    	 #endif 
   		 return 1;
    }
    else 
    	return 0; 
}
//add by yangjuwei 
static unsigned int lcm_ata_check(unsigned char *buffer)
{
	return 1;
/*#ifndef BUILD_LK
			int array[4];
			char buf[3];
			//char id_high=0;
			//char id_low=0;
			int id=0;
		
	    array[0] = 0x00043902;
		array[1] = 0x018198FF;
		dsi_set_cmdq(array, 2, 1);
	
		array[0] = 0x00013700;
		dsi_set_cmdq(array, 1, 1);
		atomic_set(&ESDCheck_byCPU,1);
		read_reg_v2(0x00, &buf[0], 1);  //0x98
		atomic_set(&ESDCheck_byCPU,0);
		id = buf[0];	
			return (0x98 == id)?1:0;
#else
	return 1;
#endif*/
}


LCM_DRIVER ili9881c_hd720_dsi_vdo_cw_cpt_ips_b55016_lcm_drv = 
{
	.name			= "ili9881c_hd720_dsi_vdo_cw_cpt_ips_b55016",
	.set_util_funcs		= lcm_set_util_funcs,
	.get_params		= lcm_get_params,
	.init			= lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = rgk_lcm_compare_id,
  .ata_check		= lcm_ata_check,
#if (LCM_DSI_CMD_MODE)
	//.set_backlight	= lcm_setbacklight,
	//.update		= lcm_update,
#endif
};
