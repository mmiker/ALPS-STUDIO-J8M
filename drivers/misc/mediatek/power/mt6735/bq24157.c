#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
//#include <linux/earlysuspend.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>
#include <linux/module.h>
#include <linux/gpio.h>
//#include <cust_acc.h>
//#include <linux/hwmsensor.h>
//#include <linux/hwmsen_dev.h>
//#include <linux/sensors_io.h>
//#include <linux/hwmsen_helper.h>
//#include <linux/xlog.h>


#include <mt-plat/mt_typedefs.h>
#include <mt-plat/mt_gpio.h>
//#include <mt-plat/mt_pm_ldo.h>

#include "bq24157.h"
#include "cust_charging.h"
#include <mt-plat/charging.h>
/*#include "bq24157.h"
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/workqueue.h>

#include <mt-plat/charging.h>
#include <mt-plat/mt_gpio.h>*/

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif


/**********************************************************
  *
  *   [I2C Slave Setting] 
  *
  *********************************************************/
#define bq24157_SLAVE_ADDR_WRITE   0xD4
#define bq24157_SLAVE_ADDR_Read    0xD5

static struct i2c_client *new_client = NULL;
static const struct i2c_device_id bq24157_i2c_id[] = {{"bq24157",0},{}};   
kal_bool chargin_hw_init_done = KAL_FALSE; 
static int bq24157_driver_probe(struct i2c_client *client, const struct i2c_device_id *id);

#ifdef CONFIG_OF
static const struct of_device_id bq24157_of_match[] = {
	{.compatible = "mediatek,SWITHING_CHARGER",},
	{},
};

MODULE_DEVICE_TABLE(of, bq24157_of_match);
#endif
static struct i2c_driver bq24157_driver = {
    .driver = {
        .name    = "bq24157",
#ifdef CONFIG_OF
		   .of_match_table = bq24157_of_match,
#endif
    },
    .probe       = bq24157_driver_probe,
    .id_table    = bq24157_i2c_id,
};

/**********************************************************
  *
  *   [Global Variable] 
  *
  *********************************************************/
u8 bq24157_reg[bq24157_REG_NUM] = {0};

static DEFINE_MUTEX(bq24157_i2c_access);
/**********************************************************
  *
  *   [I2C Function For Read/Write bq24157] 
  *
  *********************************************************/
int bq24157_read_byte(u8 cmd, u8 *returnData)
{
    char     cmd_buf[1]={0x00};
    char     readData = 0;
    int      ret=0;

    mutex_lock(&bq24157_i2c_access);
    
    //new_client->addr = ((new_client->addr) & I2C_MASK_FLAG) | I2C_WR_FLAG;    
    new_client->ext_flag=((new_client->ext_flag ) & I2C_MASK_FLAG ) | I2C_WR_FLAG | I2C_DIRECTION_FLAG;

    cmd_buf[0] = cmd;
    ret = i2c_master_send(new_client, &cmd_buf[0], (1<<8 | 1));
    if (ret < 0) 
    {    
        //new_client->addr = new_client->addr & I2C_MASK_FLAG;
        new_client->ext_flag=0;

        mutex_unlock(&bq24157_i2c_access);
        return 0;
    }
    
    readData = cmd_buf[0];
    *returnData = readData;

    // new_client->addr = new_client->addr & I2C_MASK_FLAG;
    new_client->ext_flag=0;
    
    mutex_unlock(&bq24157_i2c_access);    
    return 1;
}

int bq24157_write_byte(u8 cmd, u8 writeData)
{
    char    write_data[2] = {0};
    int     ret=0;
    
    mutex_lock(&bq24157_i2c_access);
    
    write_data[0] = cmd;
    write_data[1] = writeData;
    
    new_client->ext_flag=((new_client->ext_flag ) & I2C_MASK_FLAG ) | I2C_DIRECTION_FLAG;
    
    ret = i2c_master_send(new_client, write_data, 2);
    if (ret < 0) 
    {
       
        new_client->ext_flag=0;
        mutex_unlock(&bq24157_i2c_access);
        return 0;
    }
    
    new_client->ext_flag=0;
    mutex_unlock(&bq24157_i2c_access);
    return 1;
}

/**********************************************************
  *
  *   [Read / Write Function] 
  *
  *********************************************************/
u32 bq24157_read_interface (u8 RegNum, u8 *val, u8 MASK, u8 SHIFT)
{
    u8 bq24157_reg = 0;
    int ret = 0;

   battery_xlog_printk(BAT_LOG_FULL,"--------------------------------------------------\n");

    ret = bq24157_read_byte(RegNum, &bq24157_reg);

	battery_xlog_printk(BAT_LOG_FULL,"[bq24157_read_interface] Reg[%x]=0x%x\n", RegNum, bq24157_reg);
	
    bq24157_reg &= (MASK << SHIFT);
    *val = (bq24157_reg >> SHIFT);
	
	battery_xlog_printk(BAT_LOG_FULL,"[bq24157_read_interface] val=0x%x\n", *val);
	
    return ret;
}

u32 bq24157_config_interface (u8 RegNum, u8 val, u8 MASK, u8 SHIFT)
{
    u8 bq24157_reg = 0;
    int ret = 0;

    battery_xlog_printk(BAT_LOG_FULL,"--------------------------------------------------\n");

    ret = bq24157_read_byte(RegNum, &bq24157_reg);
    battery_xlog_printk(BAT_LOG_FULL,"[bq24157_config_interface] Reg[%x]=0x%x\n", RegNum, bq24157_reg);
    
    bq24157_reg &= ~(MASK << SHIFT);
    bq24157_reg |= (val << SHIFT);

	if(RegNum == bq24157_CON4 && val == 1 && MASK ==CON4_RESET_MASK && SHIFT == CON4_RESET_SHIFT)
	{
		// RESET bit
	}
	else if(RegNum == bq24157_CON4)
	{
		bq24157_reg &= ~0x80;	//RESET bit read returs 1, so clear it
	}
	 

    ret = bq24157_write_byte(RegNum, bq24157_reg);
    battery_xlog_printk(BAT_LOG_FULL,"[bq24157_config_interface] write Reg[%x]=0x%x\n", RegNum, bq24157_reg);

    // Check
    //bq24157_read_byte(RegNum, &bq24157_reg);
    //printk("[bq24157_config_interface] Check Reg[%x]=0x%x\n", RegNum, bq24157_reg);

    return ret;
}

//write one register directly
u32 bq24157_config_interface_liao (u8 RegNum, u8 val)
{   
    int ret = 0;
    
    ret = bq24157_write_byte(RegNum, val);

    return ret;
}

/**********************************************************
  *
  *   [Internal Function] 
  *
  *********************************************************/
//CON0----------------------------------------------------

void bq24157_set_tmr_rst(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON0), 
                                    (u8)(val),
                                    (u8)(CON0_TMR_RST_MASK),
                                    (u8)(CON0_TMR_RST_SHIFT)
                                    );
}

u32 bq24157_get_otg_status(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON0), 
                                    (&val),
                                    (u8)(CON0_OTG_MASK),
                                    (u8)(CON0_OTG_SHIFT)
                                    );
    return val;
}

void bq24157_set_en_stat(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON0), 
                                    (u8)(val),
                                    (u8)(CON0_EN_STAT_MASK),
                                    (u8)(CON0_EN_STAT_SHIFT)
                                    );
}

u32 bq24157_get_chip_status(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON0), 
                                    (&val),
                                    (u8)(CON0_STAT_MASK),
                                    (u8)(CON0_STAT_SHIFT)
                                    );
    return val;
}

u32 bq24157_get_boost_status(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON0), 
                                    (&val),
                                    (u8)(CON0_BOOST_MASK),
                                    (u8)(CON0_BOOST_SHIFT)
                                    );
    return val;
}

u32 bq24157_get_fault_status(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON0), 
                                    (&val),
                                    (u8)(CON0_FAULT_MASK),
                                    (u8)(CON0_FAULT_SHIFT)
                                    );
    return val;
}

//CON1----------------------------------------------------

void bq24157_set_input_charging_current(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON1), 
                                    (u8)(val),
                                    (u8)(CON1_LIN_LIMIT_MASK),
                                    (u8)(CON1_LIN_LIMIT_SHIFT)
                                    );
}

void bq24157_set_v_low(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON1), 
                                    (u8)(val),
                                    (u8)(CON1_LOW_V_MASK),
                                    (u8)(CON1_LOW_V_SHIFT)
                                    );
}

void bq24157_set_te(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON1), 
                                    (u8)(val),
                                    (u8)(CON1_TE_MASK),
                                    (u8)(CON1_TE_SHIFT)
                                    );
}

void bq24157_set_ce(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON1), 
                                    (u8)(val),
                                    (u8)(CON1_CE_MASK),
                                    (u8)(CON1_CE_SHIFT)
                                    );
}

void bq24157_set_hz_mode(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON1), 
                                    (u8)(val),
                                    (u8)(CON1_HZ_MODE_MASK),
                                    (u8)(CON1_HZ_MODE_SHIFT)
                                    );
}

void bq24157_set_opa_mode(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON1), 
                                    (u8)(val),
                                    (u8)(CON1_OPA_MODE_MASK),
                                    (u8)(CON1_OPA_MODE_SHIFT)
                                    );
}

//CON2----------------------------------------------------

void bq24157_set_oreg(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON2), 
                                    (u8)(val),
                                    (u8)(CON2_OREG_MASK),
                                    (u8)(CON2_OREG_SHIFT)
                                    );
}

void bq24157_set_otg_pl(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON2), 
                                    (u8)(val),
                                    (u8)(CON2_OTG_PL_MASK),
                                    (u8)(CON2_OTG_PL_SHIFT)
                                    );
}

void bq24157_set_otg_en(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON2), 
                                    (u8)(val),
                                    (u8)(CON2_OTG_EN_MASK),
                                    (u8)(CON2_OTG_EN_SHIFT)
                                    );
}

//CON3----------------------------------------------------

u32 bq24157_get_vender_code(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON3), 
                                    (&val),
                                    (u8)(CON3_VENDER_CODE_MASK),
                                    (u8)(CON3_VENDER_CODE_SHIFT)
                                    );
    return val;
}

u32 bq24157_get_pn(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON3), 
                                    (&val),
                                    (u8)(CON3_PIN_MASK),
                                    (u8)(CON3_PIN_SHIFT)
                                    );
    return val;
}

u32 bq24157_get_revision(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON3), 
                                    (&val),
                                    (u8)(CON3_REVISION_MASK),
                                    (u8)(CON3_REVISION_SHIFT)
                                    );
    return val;
}

//CON4----------------------------------------------------

void bq24157_set_reset(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON4), 
                                    (u8)(val),
                                    (u8)(CON4_RESET_MASK),
                                    (u8)(CON4_RESET_SHIFT)
                                    );
}

void bq24157_set_iocharge(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON4), 
                                    (u8)(val),
                                    (u8)(CON4_I_CHR_MASK),
                                    (u8)(CON4_I_CHR_SHIFT)
                                    );
}

void bq24157_set_iterm(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON4), 
                                    (u8)(val),
                                    (u8)(CON4_I_TERM_MASK),
                                    (u8)(CON4_I_TERM_SHIFT)
                                    );
}

//CON5----------------------------------------------------

void bq24157_set_dis_vreg(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON5), 
                                    (u8)(val),
                                    (u8)(CON5_DIS_VREG_MASK),
                                    (u8)(CON5_DIS_VREG_SHIFT)
                                    );
}

void bq24157_set_io_level(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON5), 
                                    (u8)(val),
                                    (u8)(CON5_IO_LEVEL_MASK),
                                    (u8)(CON5_IO_LEVEL_SHIFT)
                                    );
}

u32 bq24157_get_sp_status(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON5), 
                                    (&val),
                                    (u8)(CON5_SP_STATUS_MASK),
                                    (u8)(CON5_SP_STATUS_SHIFT)
                                    );
    return val;
}

u32 bq24157_get_en_level(void)
{
    u32 ret=0;
    u8 val=0;

    ret=bq24157_read_interface(     (u8)(bq24157_CON5), 
                                    (&val),
                                    (u8)(CON5_EN_LEVEL_MASK),
                                    (u8)(CON5_EN_LEVEL_SHIFT)
                                    );
    return val;
}

void bq24157_set_vsp(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON5), 
                                    (u8)(val),
                                    (u8)(CON5_VSP_MASK),
                                    (u8)(CON5_VSP_SHIFT)
                                    );
}

//CON6----------------------------------------------------

void bq24157_set_i_safe(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON6), 
                                    (u8)(val),
                                    (u8)(CON6_ISAFE_MASK),
                                    (u8)(CON6_ISAFE_SHIFT)
                                    );
}

void bq24157_set_v_safe(u32 val)
{
    u32 ret=0;    

    ret=bq24157_config_interface(   (u8)(bq24157_CON6), 
                                    (u8)(val),
                                    (u8)(CON6_VSAFE_MASK),
                                    (u8)(CON6_VSAFE_SHIFT)
                                    );
}

/**********************************************************
  *
  *   [Internal Function] 
  *
  *********************************************************/
void bq24157_dump_register(void)
{
    int i=0;
    printk("[bq24157] ");
    for (i=0;i<bq24157_REG_NUM;i++)
    {
        bq24157_read_byte(i, &bq24157_reg[i]);
        printk("[0x%x]=0x%x ", i, bq24157_reg[i]);        
    }
    printk("\n");
}

#if 0
extern int g_enable_high_vbat_spec;
extern int g_pmic_cid;

void bq24157_hw_init(void)
{    
    if(g_enable_high_vbat_spec == 1)
    {
        if(g_pmic_cid == 0x1020)
        {
            printk("[bq24157_hw_init] (0x06,0x70) because 0x1020\n");
            bq24157_config_interface_liao(0x06,0x70); // set ISAFE
        }
        else
        {
            printk("[bq24157_hw_init] (0x06,0x77)\n");
            bq24157_config_interface_liao(0x06,0x77); // set ISAFE and HW CV point (4.34)
        }
    }
    else
    {
        printk("[bq24157_hw_init] (0x06,0x70) \n");
        bq24157_config_interface_liao(0x06,0x70); // set ISAFE
    }
}
#endif

static int bq24157_driver_probe(struct i2c_client *client, const struct i2c_device_id *id) 
{             
    int err=0; 
    static struct device_node *node;

    battery_xlog_printk(BAT_LOG_CRTI,"[bq24157_driver_probe] \n");

    if (!(new_client = kmalloc(sizeof(struct i2c_client), GFP_KERNEL))) {
        err = -ENOMEM;
        goto exit;
    }    
    memset(new_client, 0, sizeof(struct i2c_client));

    new_client = client;    

    //---------------------

    node = of_find_compatible_node(NULL, NULL, "mediatek,charger_enable_node");
    charger_enable_pin = of_get_named_gpio(node, "charger_enable", 0);
    gpio_request(charger_enable_pin, "charger_enable");
    gpio_direction_output(charger_enable_pin, 0);
	  gpio_set_value(charger_enable_pin, 1);
	
    //  bq24157_hw_init();
    bq24157_dump_register();
    chargin_hw_init_done = KAL_TRUE;

    return 0;                                                                                       

exit:
    return err;

}

/**********************************************************
  *
  *   [platform_driver API] 
  *
  *********************************************************/
u8 g_reg_value_bq24157=0;
static ssize_t show_bq24157_access(struct device *dev,struct device_attribute *attr, char *buf)
{
    battery_xlog_printk(BAT_LOG_FULL,"[show_bq24157_access] 0x%x\n", g_reg_value_bq24157);
    return sprintf(buf, "%u\n", g_reg_value_bq24157);
}
static ssize_t store_bq24157_access(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
{
    int ret=0;
    char *pvalue = NULL;
    unsigned int reg_value = 0;
    unsigned int reg_address = 0;
    
    battery_xlog_printk(BAT_LOG_FULL,"[store_bq24157_access] \n");
    
    if(buf != NULL && size != 0)
    {
        battery_xlog_printk(BAT_LOG_FULL,"[store_bq24157_access] buf is %s and size is %d \n",buf,size);
        reg_address = simple_strtoul(buf,&pvalue,16);
        
        if(size > 3)
        {        
            reg_value = simple_strtoul((pvalue+1),NULL,16);        
            battery_xlog_printk(BAT_LOG_FULL,"[store_bq24157_access] write bq24157 reg 0x%x with value 0x%x !\n",reg_address,reg_value);
            ret=bq24157_config_interface(reg_address, reg_value, 0xFF, 0x0);
        }
        else
        {    
            ret=bq24157_read_interface(reg_address, &g_reg_value_bq24157, 0xFF, 0x0);
            battery_xlog_printk(BAT_LOG_FULL,"[store_bq24157_access] read bq24157 reg 0x%x with value 0x%x !\n",reg_address,g_reg_value_bq24157);
            battery_xlog_printk(BAT_LOG_FULL,"[store_bq24157_access] Please use \"cat bq24157_access\" to get value\r\n");
        }        
    }    
    return size;
}
static DEVICE_ATTR(bq24157_access, 0664, show_bq24157_access, store_bq24157_access); //664

static int bq24157_user_space_probe(struct platform_device *dev)    
{    
    int ret_device_file = 0;

    battery_xlog_printk(BAT_LOG_CRTI,"******** bq24157_user_space_probe!! ********\n" );
    
    ret_device_file = device_create_file(&(dev->dev), &dev_attr_bq24157_access);
    
    return 0;
}

struct platform_device bq24157_user_space_device = {
    .name   = "bq24157-user",
    .id     = -1,
};

static struct platform_driver bq24157_user_space_driver = {
    .probe      = bq24157_user_space_probe,
    .driver     = {
        .name = "bq24157-user",
    },
};


//static struct i2c_board_info __initdata i2c_bq24157 = { I2C_BOARD_INFO("bq24157", (bq24157_SLAVE_ADDR_WRITE>>1))};
unsigned int charger_enable_pin;
static int __init bq24157_init(void)
{    
    int ret=0;
    
    battery_xlog_printk(BAT_LOG_CRTI,"[bq24157_init] init start\n");
    
    //i2c_register_board_info(bq24157_BUSNUM, &i2c_bq24157, 1);

    if(i2c_add_driver(&bq24157_driver)!=0)
    {
        battery_xlog_printk(BAT_LOG_CRTI,"[bq24157_init] failed to register bq24157 i2c driver.\n");
    }
    else
    {
        battery_xlog_printk(BAT_LOG_CRTI,"[bq24157_init] Success to register bq24157 i2c driver.\n");
    }

    // bq24157 user space access interface
    ret = platform_device_register(&bq24157_user_space_device);
    if (ret) {
        battery_xlog_printk(BAT_LOG_CRTI,"****[bq24157_init] Unable to device register(%d)\n", ret);
        return ret;
    }    
    ret = platform_driver_register(&bq24157_user_space_driver);
    if (ret) {
        battery_xlog_printk(BAT_LOG_CRTI,"****[bq24157_init] Unable to register driver (%d)\n", ret);
        return ret;
    }
    
    /*node = of_find_compatible_node(NULL, NULL, "mediatek,charger_enable_node");
    charger_enable_pin = of_get_named_gpio(node, "charger_enable", 0);
    gpio_request(charger_enable_pin, "charger_enable");*/
    
    return 0;        
}

static void __exit bq24157_exit(void)
{
    i2c_del_driver(&bq24157_driver);
}

subsys_initcall(bq24157_init);
module_exit(bq24157_exit);
   

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C bq24157 Driver");
MODULE_AUTHOR("James Lo<james.lo@mediatek.com>");