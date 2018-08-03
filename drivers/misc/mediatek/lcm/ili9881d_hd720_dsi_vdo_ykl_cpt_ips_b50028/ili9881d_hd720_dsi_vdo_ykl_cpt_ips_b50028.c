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
#define MIN_VOLTAGE (-100)	//++++rgk bug-id:no modify by yangjuwei 20140401
#define MAX_VOLTAGE (200)	//++++rgk bug-id:no modify by yangjuwei 20140401

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
        params->dsi.noncont_clock  			= 1;
        params->dsi.noncont_clock_period    = 1;
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
{0xFF,3,{0x98,0x81,0x03}},
//GIP_1
{0x01,1,{0x00}},
{0x02,1,{0x00}},
{0x03,1,{0x73}},
{0x04,1,{0x00}},
{0x05,1,{0x00}},
{0x06,1,{0x0A}},
{0x07,1,{0x00}},
{0x08,1,{0x00}},
{0x09,1,{0x01}},
{0x0a,1,{0x00}},
{0x0b,1,{0x00}},
{0x0c,1,{0x01}},
{0x0d,1,{0x00}},
{0x0e,1,{0x00}},
{0x0f,1,{0x1D}},      
{0x10,1,{0x1D}},      
{0x11,1,{0x00}},
{0x12,1,{0x00}},
{0x13,1,{0x00}},
{0x14,1,{0x00}},
{0x15,1,{0x00}},
{0x16,1,{0x00}},
{0x17,1,{0x00}},
{0x18,1,{0x00}},
{0x19,1,{0x00}},
{0x1a,1,{0x00}},
{0x1b,1,{0x00}},
{0x1c,1,{0x00}},
{0x1d,1,{0x00}},
{0x1e,1,{0x40}},
{0x1f,1,{0x80}},
{0x20,1,{0x06}},   
{0x21,1,{0x02}},
{0x22,1,{0x00}},
{0x23,1,{0x00}},
{0x24,1,{0x00}},
{0x25,1,{0x00}},
{0x26,1,{0x00}},
{0x27,1,{0x00}},
{0x28,1,{0x33}},
{0x29,1,{0x03}},        
{0x2a,1,{0x00}},
{0x2b,1,{0x00}},
{0x2c,1,{0x00}},
{0x2d,1,{0x00}},
{0x2e,1,{0x00}},
{0x2f,1,{0x00}},
{0x30,1,{0x00}},
{0x31,1,{0x00}},
{0x32,1,{0x00}},
{0x33,1,{0x00}},
{0x34,1,{0x04}},
{0x35,1,{0x00}},
{0x36,1,{0x00}},
{0x37,1,{0x00}},
{0x38,1,{0x3C}},
{0x39,1,{0x35}},
{0x3a,1,{0x01}},
{0x3b,1,{0x40}},
{0x3c,1,{0x00}},
{0x3d,1,{0x01}},
{0x3e,1,{0x00}},
{0x3f,1,{0x00}},
{0x40,1,{0x00}},
{0x41,1,{0x88}},
{0x42,1,{0x00}},
{0x43,1,{0x00}},
{0x44,1,{0x1F}},
//GIP_2         
{0x50,1,{0x01}},
{0x51,1,{0x23}},
{0x52,1,{0x45}},
{0x53,1,{0x67}},
{0x54,1,{0x89}},
{0x55,1,{0xab}},
{0x56,1,{0x01}},
{0x57,1,{0x23}},
{0x58,1,{0x45}},
{0x59,1,{0x67}},
{0x5a,1,{0x89}},
{0x5b,1,{0xab}},
{0x5c,1,{0xcd}},
{0x5d,1,{0xef}},
{0x5e,1,{0x11}},
{0x5f,1,{0x01}},
{0x60,1,{0x00}},
{0x61,1,{0x15}},
{0x62,1,{0x14}},
{0x63,1,{0x0E}},
{0x64,1,{0x0F}},
{0x65,1,{0x0C}},
{0x66,1,{0x0D}},
{0x67,1,{0x06}},
{0x68,1,{0x02}},
{0x69,1,{0x07}},
{0x6a,1,{0x02}},
{0x6b,1,{0x02}},
{0x6c,1,{0x02}},
{0x6d,1,{0x02}},
{0x6e,1,{0x02}},
{0x6f,1,{0x02}},
{0x70,1,{0x02}},
{0x71,1,{0x02}},
{0x72,1,{0x02}},
{0x73,1,{0x02}},
{0x74,1,{0x02}},
{0x75,1,{0x01}},
{0x76,1,{0x00}},
{0x77,1,{0x14}},
{0x78,1,{0x15}},
{0x79,1,{0x0E}},
{0x7a,1,{0x0F}},
{0x7b,1,{0x0C}},
{0x7c,1,{0x0D}},
{0x7d,1,{0x06}},
{0x7e,1,{0x02}},
{0x7f,1,{0x07}},
{0x80,1,{0x02}},
{0x81,1,{0x02}},
{0x82,1,{0x02}},
{0x83,1,{0x02}},
{0x84,1,{0x02}},
{0x85,1,{0x02}},
{0x86,1,{0x02}},
{0x87,1,{0x02}},
{0x88,1,{0x02}},
{0x89,1,{0x02}},
{0x8A,1,{0x02}},




{0XFF,3,{0X98,0X81,0X04}},
{0X70,1,{0X00}},
{0X71,1,{0X00}},
{0X82,1,{0X0F}},            
{0X84,1,{0X0F}},                              
{0X85,1,{0X0D}},            
{0X32,1,{0XAC}},
{0X8C,1,{0X80}},
{0X3C,1,{0XF5}},
{0XB5,1,{0X07}},
{0X31,1,{0X45}},
{0X3A,1,{0X24}},
{0X88,1,{0X33}},

{0XFF,3,{0X98,0X81,0X01}},

{0X22,1,{0X0a}},		//BGR,1, SS
{0X31,1,{0X00}},		//inversion
{0X53,1,{0X98}},		//VCOM1  47
{0X55,1,{0XA2}},		//VCOM2
{0X50,1,{0X5b}},		//VREG1OUT=4.5V   81
{0X51,1,{0X5b}},		//VREG2OUT=--4.5V 85

{0XA0,1,{0X00}},		//VP255	Gamma P  
{0XA1,1,{0X0A}},              //VP251          
{0XA2,1,{0X14}},               //VP247          
{0XA3,1,{0X10}},              //VP243          
{0XA4,1,{0X11}},               //VP239          
{0XA5,1,{0X21}},               //VP231          
{0XA6,1,{0X16}},               //VP219          
{0XA7,1,{0X19}},               //VP203          
{0XA8,1,{0X52}},               //VP175          
{0XA9,1,{0X1a}},               //VP144          
{0XAA,1,{0X26}},               //VP111          
{0XAB,1,{0X50}},               //VP80           
{0XAC,1,{0X19}},               //VP52           
{0XAD,1,{0X17}},               //VP36           
{0XAE,1,{0X4b}},               //VP24           
{0XAF,1,{0X21}},               //VP16           
{0XB0,1,{0X28}},               //VP12           
{0XB1,1,{0X57}},               //VP8           
{0XB2,1,{0X69}},               //VP4            
{0XB3,1,{0X39}},               //VP0            
                                          
{0XC0,1,{0X00}},		//VN255 GAMMA N
{0XC1,1,{0X0b}},               //VN251        
{0XC2,1,{0X14}},               //VN247        
{0XC3,1,{0X11}},               //VN243        
{0XC4,1,{0X10}},               //VN239        
{0XC5,1,{0X22}},               //VN231        
{0XC6,1,{0X17}},               //VN219        
{0XC7,1,{0X1a}},               //VN203        
{0XC8,1,{0X53}},               //VN175        
{0XC9,1,{0X1b}},               //VN144        
{0XCA,1,{0X27}},               //VN111        
{0XCB,1,{0X50}},               //VN80         
{0XCC,1,{0X19}},               //VN52         
{0XCD,1,{0X17}},               //VN36         
{0XCE,1,{0X4c}},               //VN24         
{0XCF,1,{0X21}},               //VN16         
{0XD0,1,{0X28}},               //VN12         
{0XD1,1,{0X56}},               //VN8          
{0XD2,1,{0X6a}},               //VN4          
{0XD3,1,{0X39}},               //VN0  

{0xFF,3,{0x98,0x81,0x00}},
{0x35,1,{0x00}},
{0x11,1,{0x00}},
{REGFLAG_DELAY, 120, {}},  
{0x29,1,{0x00}},
{REGFLAG_DELAY, 20, {}},   
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
  printk("sym  lcm_init  start\n");
  lcm_get_pin();

  gpio_direction_output(lcm_gpio_enp, 0);//设为输出
  gpio_set_value(lcm_gpio_enp, 1);//设为输出高电平
  MDELAY(1);
  gpio_direction_output(lcm_gpio_enn, 0);//设为输出 	
	gpio_set_value(lcm_gpio_enn, 1);//设为输出高电平
//	ktd2150_write_bytes(addr,data);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(50);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
  printk("sym  lcm_init end\n");
}

static void lcm_esd_recover(void)
{
  printk("sym  lcm_esd_recover  start\n");
  lcm_get_pin();

  gpio_direction_output(lcm_gpio_enp, 0);//output
  gpio_set_value(lcm_gpio_enp, 1);//high
  MDELAY(1);
  gpio_direction_output(lcm_gpio_enn, 0);//output 	
	gpio_set_value(lcm_gpio_enn, 1);//high
//	ktd2150_write_bytes(addr,data);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
  printk("sym  lcm_esd_recover end\n");
}

static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(0);
	MDELAY(5);
  gpio_direction_output(lcm_gpio_enn, 0);//设为输出
  gpio_set_value(lcm_gpio_enn, 0);//设为输出low电平
  MDELAY(1);
  gpio_direction_output(lcm_gpio_enp, 0);//设为输出
	gpio_set_value(lcm_gpio_enp, 0);//设为输出low电平
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
	
    if (lcm_vol>=MIN_VOLTAGE &&lcm_vol <= MAX_VOLTAGE)// && lcm_compare_id())
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



LCM_DRIVER ili9881d_hd720_dsi_vdo_ykl_cpt_ips_b50028_lcm_drv = 
{
  .name           = "ili9881d_hd720_dsi_vdo_ykl_cpt_ips_b50028",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params     = lcm_get_params,
  .init           = lcm_init,
  .suspend        = lcm_suspend,
  .resume         = lcm_resume,
  .compare_id     = rgk_lcm_compare_id,
  .esd_recover    = lcm_esd_recover,
  .ata_check      = lcm_ata_check,
#if (LCM_DSI_CMD_MODE)
	//.set_backlight	= lcm_setbacklight,
	//.update		= lcm_update,
#endif
};

