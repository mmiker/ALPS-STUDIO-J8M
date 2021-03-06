/*****************************************************************************
 *
 * Filename:
 * ---------
 *    charging_pmic.c
 *
 * Project:
 * --------
 *   ALPS_Software
 *
 * Description:
 * ------------
 *   This file implements the interface between BMT and ADC scheduler.
 *
 * Author:
 * -------
 *  Oscar Liu
 *
 *============================================================================
  * $Revision:   1.0  $
 * $Modtime:   11 Aug 2005 10:28:16  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/hal/peripheral/inc/bmt_chr_setting.h-arc  $
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include <mt-plat/charging.h>
#include "psc5415a.h"
#include <mt-plat/upmu_common.h>
//#include <mt-plat/mt_gpio.h>
//#include <cust_gpio_usage.h>
//#include <mt-plat/upmu_hw.h>
//#include <linux/xlog.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/reboot.h>
//#include <mt-plat/mt_sleep.h>
#include <mt-plat/mt_boot.h>
//#include <mt-plat/system.h>
#include <cust_charging.h>

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


/*#include "psc5415a.h"
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/types.h>
#include <mt-plat/charging.h>
#include <mt-plat/mt_boot.h>
#include <mt-plat/mt_gpio.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif
#include <mt-plat/upmu_common.h>*/

 // ============================================================ //
 //define
 // ============================================================ //
#define STATUS_OK	0
#define STATUS_UNSUPPORTED	-1
#define GETARRAYNUM(array) (sizeof(array)/sizeof(array[0]))


 // ============================================================ //
 //global variable
 // ============================================================ //
static CHARGER_TYPE g_charger_type = CHARGER_UNKNOWN;
#if defined(MTK_WIRELESS_CHARGER_SUPPORT)
#define WIRELESS_CHARGER_EXIST_STATE 0
int wireless_charger_gpio_number   = (168 | 0x80000000); 
#endif
/*
#if 1
#include <cust_gpio_usage.h>
int gpio_number   = GPIO_SWCHARGER_EN_PIN; 
int gpio_off_mode = GPIO_SWCHARGER_EN_PIN_M_GPIO;
int gpio_on_mode  = GPIO_SWCHARGER_EN_PIN_M_GPIO;
#else
int gpio_number   = (19 | 0x80000000); 
int gpio_off_mode = 0;
int gpio_on_mode  = 0;
#endif

int gpio_off_dir  = GPIO_DIR_OUT;
int gpio_off_out  = GPIO_OUT_ONE;
int gpio_on_dir   = GPIO_DIR_OUT;
int gpio_on_out   = GPIO_OUT_ZERO;
*/
kal_bool charging_type_det_done = KAL_TRUE;

const u32 VBAT_CV_VTH[]=
{
	BATTERY_VOLT_03_500000_V,   BATTERY_VOLT_03_520000_V,	BATTERY_VOLT_03_540000_V,   BATTERY_VOLT_03_560000_V,
	BATTERY_VOLT_03_580000_V,   BATTERY_VOLT_03_600000_V,	BATTERY_VOLT_03_620000_V,   BATTERY_VOLT_03_640000_V,
	BATTERY_VOLT_03_660000_V,	BATTERY_VOLT_03_680000_V,	BATTERY_VOLT_03_700000_V,	BATTERY_VOLT_03_720000_V,
	BATTERY_VOLT_03_740000_V,	BATTERY_VOLT_03_760000_V,	BATTERY_VOLT_03_780000_V,	BATTERY_VOLT_03_800000_V,
	BATTERY_VOLT_03_820000_V,	BATTERY_VOLT_03_840000_V,	BATTERY_VOLT_03_860000_V,	BATTERY_VOLT_03_880000_V,
	BATTERY_VOLT_03_900000_V,	BATTERY_VOLT_03_920000_V,	BATTERY_VOLT_03_940000_V,	BATTERY_VOLT_03_960000_V,
	BATTERY_VOLT_03_980000_V,	BATTERY_VOLT_04_000000_V,	BATTERY_VOLT_04_020000_V,	BATTERY_VOLT_04_040000_V,
	BATTERY_VOLT_04_060000_V,	BATTERY_VOLT_04_080000_V,	BATTERY_VOLT_04_100000_V,	BATTERY_VOLT_04_120000_V,
	BATTERY_VOLT_04_140000_V,   BATTERY_VOLT_04_160000_V,	BATTERY_VOLT_04_180000_V,   BATTERY_VOLT_04_200000_V,
	BATTERY_VOLT_04_220000_V,   BATTERY_VOLT_04_240000_V,	BATTERY_VOLT_04_260000_V,   BATTERY_VOLT_04_280000_V,
	BATTERY_VOLT_04_300000_V,   BATTERY_VOLT_04_320000_V,	BATTERY_VOLT_04_340000_V,   BATTERY_VOLT_04_360000_V,	
	BATTERY_VOLT_04_380000_V,   BATTERY_VOLT_04_400000_V,	BATTERY_VOLT_04_420000_V,   BATTERY_VOLT_04_440000_V	
	
};
#ifndef CONFIG_HL7006
const u32 CS_VTH[]=
{
 CHARGE_CURRENT_425_00_MA,   CHARGE_CURRENT_775_00_MA,	CHARGE_CURRENT_950_00_MA, CHARGE_CURRENT_1150_00_MA,
  CHARGE_CURRENT_1350_00_MA,	CHARGE_CURRENT_1450_00_MA }; 
#else
const u32 CS_VTH[]=
{
	CHARGE_CURRENT_550_00_MA,   CHARGE_CURRENT_650_00_MA,	CHARGE_CURRENT_750_00_MA, CHARGE_CURRENT_850_00_MA,
  CHARGE_CURRENT_1050_00_MA,	CHARGE_CURRENT_1150_00_MA, CHARGE_CURRENT_1350_00_MA,CHARGE_CURRENT_1450_00_MA    //p19
}; 

#endif
 const u32 INPUT_CS_VTH[]=
 {
	 CHARGE_CURRENT_100_00_MA,	 CHARGE_CURRENT_500_00_MA,	 CHARGE_CURRENT_800_00_MA, CHARGE_CURRENT_MAX
 }; 

 const u32 VCDT_HV_VTH[]=
 {
	  BATTERY_VOLT_04_200000_V, BATTERY_VOLT_04_250000_V,	  BATTERY_VOLT_04_300000_V,   BATTERY_VOLT_04_350000_V,
	  BATTERY_VOLT_04_400000_V, BATTERY_VOLT_04_450000_V,	  BATTERY_VOLT_04_500000_V,   BATTERY_VOLT_04_550000_V,
	  BATTERY_VOLT_04_600000_V, BATTERY_VOLT_06_000000_V,	  BATTERY_VOLT_06_500000_V,   BATTERY_VOLT_07_000000_V,
	  BATTERY_VOLT_07_500000_V, BATTERY_VOLT_08_500000_V,	  BATTERY_VOLT_09_500000_V,   BATTERY_VOLT_10_500000_V		  
 };

static u32 charging_error = false;
 // ============================================================ //
 // function prototype
 // ============================================================ //

static u32 charging_get_error_state(void);
static u32 charging_set_error_state(void *data);
 
 // ============================================================ //
 //extern variable
 // ============================================================ //
 
 // ============================================================ //
 //extern function
 // ============================================================ //
 //extern u32 upmu_get_reg_value(u32 reg);
 extern bool mt_usb_is_device(void);
 extern void Charger_Detect_Init(void);
 extern void Charger_Detect_Release(void);
 extern void mt_power_off(void);
 
 extern bool get_usb_current_unlimited(void);
 
 // ============================================================ //
 u32 charging_value_to_parameter(const u32 *parameter, const u32 array_size, const u32 val)
{
	if (val < array_size)
	{
		return parameter[val];
	}
	else
	{
		battery_xlog_printk(BAT_LOG_CRTI, "Can't find the parameter \r\n");	
		return parameter[0];
	}
}

 
 u32 charging_parameter_to_value(const u32 *parameter, const u32 array_size, const u32 val)
{
	u32 i;

	for(i=0;i<array_size;i++)
	{
		if (val == *(parameter + i))
		{
				return i;
		}
	}

    battery_xlog_printk(BAT_LOG_CRTI, "NO register value match \r\n");
	//TODO: ASSERT(0);	// not find the value
	return 0;
}


 static u32 bmt_find_closest_level(const u32 *pList,u32 number,u32 level)
 {
	 u32 i;
	 u32 max_value_in_last_element;
 
	 if(pList[0] < pList[1])
		 max_value_in_last_element = KAL_TRUE;
	 else
		 max_value_in_last_element = KAL_FALSE;
 
	 if(max_value_in_last_element == KAL_TRUE)
	 {
		 for(i = (number-1); i >= 0; i--)	 //max value in the last element
		 {
			 if(pList[i] <= level)
			 {
				 return pList[i];
			 }	  
		 }

 		 battery_xlog_printk(BAT_LOG_CRTI, "Can't find closest level, small value first \r\n");
		 return pList[0];
		 //return CHARGE_CURRENT_0_00_MA;
	 }
	 else
	 {
		 for(i = 0; i< number; i++)  // max value in the first element
		 {
			 if(pList[i] <= level)
			 {
				 return pList[i];
			 }	  
		 }

		 battery_xlog_printk(BAT_LOG_CRTI, "Can't find closest level, large value first \r\n"); 	 
		 return pList[number -1];
  		 //return CHARGE_CURRENT_0_00_MA;
	 }
 }


static void hw_bc11_dump_register(void)
{
/*
	u32 reg_val = 0;
	u32 reg_num = MT6350_CHR_CON18;
	u32 i = 0;

	for(i=reg_num ; i<=MT6350_CHR_CON19 ; i+=2)
	{
		//reg_val = upmu_get_reg_value(i);
		reg_val = pmic_get_register_value(i);
		battery_xlog_printk(BAT_LOG_FULL, "Chr Reg[0x%x]=0x%x \r\n", i, reg_val);
	}
*/
	return;
}


 static void hw_bc11_init(void)
 {
 	 msleep(300);
	 Charger_Detect_Init();
		 
	 //RG_BC11_BIAS_EN=1	
	 //upmu_set_rg_bc11_bias_en(0x1);
     pmic_set_register_value(PMIC_RG_BC11_BIAS_EN,1);
	 //RG_BC11_VSRC_EN[1:0]=00
	 //upmu_set_rg_bc11_vsrc_en(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_VSRC_EN,0);
	 //RG_BC11_VREF_VTH = [1:0]=00
	 //upmu_set_rg_bc11_vref_vth(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
	 //RG_BC11_CMP_EN[1.0] = 00
	 //upmu_set_rg_bc11_cmp_en(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
	 //RG_BC11_IPU_EN[1.0] = 00
	 //upmu_set_rg_bc11_ipu_en(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_IPU_EN,0);
	 //RG_BC11_IPD_EN[1.0] = 00
	 //upmu_set_rg_bc11_ipd_en(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_IPD_EN,0);
	 //BC11_RST=1
	 //upmu_set_rg_bc11_rst(0x1);
	 pmic_set_register_value(PMIC_RG_BC11_RST,1);
	 //BC11_BB_CTRL=1
	 //upmu_set_rg_bc11_bb_ctrl(0x1);
	 pmic_set_register_value(PMIC_RG_BC11_BB_CTRL,1);
 
 	 //msleep(10);
 	 mdelay(50);

	 if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	 {
    		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_init() \r\n");
		hw_bc11_dump_register();
	 }	
	 
 }
 
 
 static U32 hw_bc11_DCD(void)
 {
	 U32 wChargerAvail = 0;
 
	 //RG_BC11_IPU_EN[1.0] = 10
	 //upmu_set_rg_bc11_ipu_en(0x2);
     pmic_set_register_value(PMIC_RG_BC11_IPU_EN,2);
	 //RG_BC11_IPD_EN[1.0] = 01
	 //upmu_set_rg_bc11_ipd_en(0x1);
     pmic_set_register_value(PMIC_RG_BC11_IPD_EN,1);
	 //RG_BC11_VREF_VTH = [1:0]=01
	 //upmu_set_rg_bc11_vref_vth(0x1);
     pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,1);
	 //RG_BC11_CMP_EN[1.0] = 10
	 //upmu_set_rg_bc11_cmp_en(0x2);
     pmic_set_register_value(PMIC_RG_BC11_CMP_EN,2);
 
	 //msleep(20);
	 mdelay(80);

 	 //wChargerAvail = upmu_get_rgs_bc11_cmp_out();
     wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);
	 
	 if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	 {
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_DCD() \r\n");
		hw_bc11_dump_register();
	 }
	 
	 //RG_BC11_IPU_EN[1.0] = 00
	 //upmu_set_rg_bc11_ipu_en(0x0);
     pmic_set_register_value(PMIC_RG_BC11_IPU_EN,0);
	 //RG_BC11_IPD_EN[1.0] = 00
	 //upmu_set_rg_bc11_ipd_en(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_IPD_EN,0);
	 //RG_BC11_CMP_EN[1.0] = 00
	 //upmu_set_rg_bc11_cmp_en(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
	 //RG_BC11_VREF_VTH = [1:0]=00
	 //upmu_set_rg_bc11_vref_vth(0x0);
	 pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
 
	 return wChargerAvail;
 }
 
 
 static U32 hw_bc11_stepA1(void)
 {
	U32 wChargerAvail = 0;

	//RG_BC11_IPD_EN[1:0] = 01
	//upmu_set_rg_bc11_ipd_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN,1);
	//RG_BC11_VREF_VTH[1:0]=00
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
	// RG_BC11_CMP_EN[1:0] = 01
	//upmu_set_rg_bc11_cmp_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,1);
 
	//msleep(80);
	mdelay(80);
 
	//wChargerAvail = upmu_get_rgs_bc11_cmp_out();
	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);
 
	if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	{
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_stepA1() \r\n");
		hw_bc11_dump_register();
	}
 
	//RG_BC11_IPD_EN[1.0] = 00
	//upmu_set_rg_bc11_ipd_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN,0);
	//RG_BC11_CMP_EN[1.0] = 00
	//upmu_set_rg_bc11_cmp_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
 
	return  wChargerAvail;
 }
 
 
 static U32 hw_bc11_stepB1(void)
 {
	U32 wChargerAvail = 0;
	  
	//RG_BC11_IPU_EN[1.0] = 01
	//upmu_set_rg_bc11_ipu_en(0x1);
	//upmu_set_rg_bc11_ipd_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN,1);
	//RG_BC11_VREF_VTH = [1:0]=10
	//upmu_set_rg_bc11_vref_vth(0x2);
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
	//RG_BC11_CMP_EN[1.0] = 01
	//upmu_set_rg_bc11_cmp_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,1);
 
	//msleep(80);
	mdelay(80);
 
	//wChargerAvail = upmu_get_rgs_bc11_cmp_out();
	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);
 
	if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	{
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_stepB1() \r\n");
		hw_bc11_dump_register();
	}
 
	//RG_BC11_IPU_EN[1.0] = 00
	//upmu_set_rg_bc11_ipu_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN,0);
	//RG_BC11_CMP_EN[1.0] = 00
	//upmu_set_rg_bc11_cmp_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
	//RG_BC11_VREF_VTH = [1:0]=00
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
 
	return  wChargerAvail;
 }
 
 
 static U32 hw_bc11_stepC1(void)
 {
	U32 wChargerAvail = 0;
	  
	//RG_BC11_IPU_EN[1.0] = 01
	//upmu_set_rg_bc11_ipu_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN,1);
	//RG_BC11_VREF_VTH = [1:0]=10
	//upmu_set_rg_bc11_vref_vth(0x2);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,2);
	//RG_BC11_CMP_EN[1.0] = 01
	//upmu_set_rg_bc11_cmp_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,1);
 
	//msleep(80);
	mdelay(80);
 
	//wChargerAvail = upmu_get_rgs_bc11_cmp_out();
	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);
 
	if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	{
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_stepC1() \r\n");
		hw_bc11_dump_register();
	}
 
	//RG_BC11_IPU_EN[1.0] = 00
	//upmu_set_rg_bc11_ipu_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN,0);
	//RG_BC11_CMP_EN[1.0] = 00
	//upmu_set_rg_bc11_cmp_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
	//RG_BC11_VREF_VTH = [1:0]=00
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
 
	return  wChargerAvail;
 }
 
 
 static U32 hw_bc11_stepA2(void)
 {
	U32 wChargerAvail = 0;
	  
	//RG_BC11_VSRC_EN[1.0] = 10 
	//upmu_set_rg_bc11_vsrc_en(0x2);
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN,2);
	//RG_BC11_IPD_EN[1:0] = 01
	//upmu_set_rg_bc11_ipd_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN,1);
	//RG_BC11_VREF_VTH = [1:0]=00
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
	//RG_BC11_CMP_EN[1.0] = 01
	//upmu_set_rg_bc11_cmp_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,1);
 
	//msleep(80);
	mdelay(80);
 
	//wChargerAvail = upmu_get_rgs_bc11_cmp_out();
	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);
 
	if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	{
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_stepA2() \r\n");
		hw_bc11_dump_register();
	}
 
	//RG_BC11_VSRC_EN[1:0]=00
	//upmu_set_rg_bc11_vsrc_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN,0);
	//RG_BC11_IPD_EN[1.0] = 00
	//upmu_set_rg_bc11_ipd_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN,0);
	//RG_BC11_CMP_EN[1.0] = 00
	//upmu_set_rg_bc11_cmp_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
 
	return  wChargerAvail;
 }
 
 
 static U32 hw_bc11_stepB2(void)
 {
	U32 wChargerAvail = 0;
 
	//RG_BC11_IPU_EN[1:0]=10
	//upmu_set_rg_bc11_ipu_en(0x2);
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN,2);
	//RG_BC11_VREF_VTH = [1:0]=10
	//upmu_set_rg_bc11_vref_vth(0x1);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,1);
	//RG_BC11_CMP_EN[1.0] = 01
	//upmu_set_rg_bc11_cmp_en(0x1);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,1);
 
	//msleep(80);
	mdelay(80);
 
	//wChargerAvail = upmu_get_rgs_bc11_cmp_out();
	wChargerAvail = pmic_get_register_value(PMIC_RGS_BC11_CMP_OUT);
 
	if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	{
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_stepB2() \r\n");
		hw_bc11_dump_register();
	}
 
	//RG_BC11_IPU_EN[1.0] = 00
	//upmu_set_rg_bc11_ipu_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN,0);
	//RG_BC11_CMP_EN[1.0] = 00
	//upmu_set_rg_bc11_cmp_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
	//RG_BC11_VREF_VTH = [1:0]=00
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
 
	return  wChargerAvail;
 }
 
 
 static void hw_bc11_done(void)
 {
	//RG_BC11_VSRC_EN[1:0]=00
	//upmu_set_rg_bc11_vsrc_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VSRC_EN,0);
	//RG_BC11_VREF_VTH = [1:0]=0
	//upmu_set_rg_bc11_vref_vth(0x0);
	pmic_set_register_value(PMIC_RG_BC11_VREF_VTH,0);
	//RG_BC11_CMP_EN[1.0] = 00
	//upmu_set_rg_bc11_cmp_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_CMP_EN,0);
	//RG_BC11_IPU_EN[1.0] = 00
	//upmu_set_rg_bc11_ipu_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPU_EN,0);
	//RG_BC11_IPD_EN[1.0] = 00
	//upmu_set_rg_bc11_ipd_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_IPD_EN,0);
	//RG_BC11_BIAS_EN=0
	//upmu_set_rg_bc11_bias_en(0x0);
	pmic_set_register_value(PMIC_RG_BC11_BIAS_EN,0);
 
	Charger_Detect_Release();

	if(Enable_BATDRV_LOG == BAT_LOG_FULL)
	{
		battery_xlog_printk(BAT_LOG_FULL, "hw_bc11_done() \r\n");
		hw_bc11_dump_register();
	}
    
 }

 static u32 charging_hw_init(void *data)
 {
 	u32 status = STATUS_OK;
	static bool charging_init_flag = KAL_FALSE;
	/*
  node = of_find_compatible_node(NULL, NULL, "mediatek,charger_enable_node");
  charger_enable_pin = of_get_named_gpio(node, "charger_enable", 0);
  gpio_request(charger_enable_pin, "charger_enable");*/
  
  //gpio_direction_output(charger_enable_pin, 0);
 	//MDELAY(10);
	//gpio_set_value(charger_enable_pin, 0);

	//mt_set_gpio_mode(gpio_number,gpio_on_mode);  
	//mt_set_gpio_dir(gpio_number,gpio_on_dir);
	//mt_set_gpio_out(gpio_number,gpio_on_out);
#if defined(MTK_WIRELESS_CHARGER_SUPPORT)
	mt_set_gpio_mode(wireless_charger_gpio_number,0); // 0:GPIO mode
	mt_set_gpio_dir(wireless_charger_gpio_number,0); // 0: input, 1: output
#endif
	//battery_xlog_printk(BAT_LOG_FULL, "gpio_number=0x%x,gpio_on_mode=%d,gpio_off_mode=%d\n", gpio_number, gpio_on_mode, gpio_off_mode);
	
	//upmu_set_rg_usbdl_rst(1);		//force leave USBDL mode
	pmic_set_register_value(PMIC_RG_USBDL_RST,1);
#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
	psc5415a_config_interface_liao(0x06,0x77); // ISAFE = 1250mA, VSAFE = 4.34V
#else
	psc5415a_config_interface_liao(0x06,0x70);
#endif
	    
	psc5415a_config_interface_liao(0x00,0xC0);	//kick chip watch dog
#ifndef CONFIG_HL7006	
	psc5415a_config_interface_liao(0x01,0xf8);	//TE=1, CE=0, HZ_MODE=0, OPA_MODE=0
	psc5415a_config_interface_liao(0x05,0x04);
	//psc5415a_config_interface_liao(0x04,0x1A); //146mA
	//psc5415a_config_interface_liao(0x02,0xa2); 
#else
	psc5415a_config_interface_liao(0x01,0xF8);	//TE=1, CE=0, HZ_MODE=0, OPA_MODE=0
	psc5415a_config_interface_liao(0x05,0x03);
	//psc5415a_config_interface_liao(0x04,0x7A); //146mA
	psc5415a_set_iterm(0x2);//150ma  ���ý�ֹ����
#endif    

	if ( !charging_init_flag ) {   
			psc5415a_config_interface_liao(0x06,0x77); //146mA
		//psc5415a_config_interface_liao(0x04,0x1A); //146mA
			//psc5415a_config_interface_liao(0x02,0xa2); 
			psc5415a_config_interface_liao(0x01,0xf8);	//TE=1, CE=0, HZ_MODE=0, OPA_MODE=0
			psc5415a_config_interface_liao(0x05,0x04);
		charging_init_flag = KAL_TRUE;
	}    
	psc5415a_dump_register();
	printk("sym charging hw init\n");
//	msleep(4000);
 	return status;
 }


 static u32 charging_dump_register(void *data)
 {
 	u32 status = STATUS_OK;

	psc5415a_dump_register();
   	
	return status;
 }	


 static u32 charging_enable(void *data)
 {
 	u32 status = STATUS_OK;
	u32 enable = *(u32*)(data);

	if(KAL_TRUE == enable) {
		psc5415a_set_ce(0);
		psc5415a_set_hz_mode(0);
		psc5415a_set_opa_mode(0);
	//	gpio_direction_output(charger_enable_pin, 0);
//	  gpio_set_value(charger_enable_pin, 0);
	} else {

#if defined(CONFIG_USB_MTK_HDRC_HCD)
   		if(mt_usb_is_device())
#endif 			
		{
			psc5415a_set_ce(1);
			/*
			mt_set_gpio_mode(gpio_number,gpio_off_mode);  
			mt_set_gpio_dir(gpio_number,gpio_off_dir);
			mt_set_gpio_out(gpio_number,gpio_off_out);
      */
   //   gpio_direction_output(charger_enable_pin, 0);
 	//MDELAY(10);
	//    gpio_set_value(charger_enable_pin, 1);
			
			if (charging_get_error_state()) {
				psc5415a_set_ce(0x1);
				battery_xlog_printk(BAT_LOG_CRTI,"[charging_enable] psc5415a_set_hz_mode(0x1)\n");
				psc5415a_set_hz_mode(0x1);	// disable power path
			}
		}
	}
	return status;
 }


 static u32 charging_set_cv_voltage(void *data)
 {
 	u32 status = STATUS_OK;
	u16 register_value;
	//u32 cv_value = *(u32 *)(data);

	register_value = charging_parameter_to_value(VBAT_CV_VTH, GETARRAYNUM(VBAT_CV_VTH) ,*(u32 *)(data));
	psc5415a_set_oreg(register_value); 

	return status;
 } 	


 static u32 charging_get_current(void *data)
 {
    u32 status = STATUS_OK;
    u32 array_size;
    u8 reg_value;
	
    //Get current level
    array_size = GETARRAYNUM(CS_VTH);
    psc5415a_read_interface(0x1, &reg_value, 0x3, 0x6);	//IINLIM
    *(u32 *)data = charging_value_to_parameter(CS_VTH,array_size,reg_value);
	
    return status;
 }  
  


 static u32 charging_set_current(void *data)
 {
 	u32 status = STATUS_OK;
	u32 set_chr_current;
	u32 array_size;
	u32 register_value;
	u32 current_value = *(u32 *)data;

	if(current_value <= CHARGE_CURRENT_350_00_MA)
	{
		psc5415a_set_io_level(1);
	}
	else
	{
		psc5415a_set_io_level(0);
		array_size = GETARRAYNUM(CS_VTH);
		set_chr_current = bmt_find_closest_level(CS_VTH, array_size, current_value);
		register_value = charging_parameter_to_value(CS_VTH, array_size ,set_chr_current);
		psc5415a_set_iocharge(register_value);
	}
	return status;
 } 	
 

 static u32 charging_set_input_current(void *data)
 {
 	u32 status = STATUS_OK;
	u32 set_chr_current;
	u32 array_size;
	u32 register_value;

    if(*(u32 *)data > CHARGE_CURRENT_900_00_MA)
    {
        register_value = 0x3;
    }
    else
    {
    	array_size = GETARRAYNUM(INPUT_CS_VTH);
    	set_chr_current = bmt_find_closest_level(INPUT_CS_VTH, array_size, *(u32 *)data);
    	register_value = charging_parameter_to_value(INPUT_CS_VTH, array_size ,set_chr_current);	
    }
    
    psc5415a_set_input_charging_current(register_value);

	return status;
 } 	


 static u32 charging_get_charging_status(void *data)
 {
 	u32 status = STATUS_OK;
	u32 ret_val;

	ret_val = psc5415a_get_chip_status();
	
	if(ret_val == 0x2)
		*(u32 *)data = KAL_TRUE;
	else
		*(u32 *)data = KAL_FALSE;
	
	return status;
 } 	


 static u32 charging_reset_watch_dog_timer(void *data)
 {
	 u32 status = STATUS_OK;
 
	 psc5415a_set_tmr_rst(1);
	 
	 return status;
 }
 
 
  static u32 charging_set_hv_threshold(void *data)
  {
	 u32 status = STATUS_OK;
 
	 u32 set_hv_voltage;
	 u32 array_size;
	 u16 register_value;
	 u32 voltage = *(u32*)(data);
	 
	 array_size = GETARRAYNUM(VCDT_HV_VTH);
	 set_hv_voltage = bmt_find_closest_level(VCDT_HV_VTH, array_size, voltage);
	 register_value = charging_parameter_to_value(VCDT_HV_VTH, array_size ,set_hv_voltage);
	 //upmu_set_rg_vcdt_hv_vth(register_value);
	 pmic_set_register_value(PMIC_RG_VCDT_HV_VTH,register_value);
 
	 return status;
  }
 
 
  static u32 charging_get_hv_status(void *data)
  {
	   u32 status = STATUS_OK;
 
	   //*(kal_bool*)(data) = upmu_get_rgs_vcdt_hv_det();
	   *(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_VCDT_HV_DET);
	   
	   return status;
  }


 static u32 charging_get_battery_status(void *data)
 {
	   u32 status = STATUS_OK;
 
 	   //upmu_set_baton_tdet_en(1);
 	   pmic_set_register_value(PMIC_BATON_TDET_EN,1);
	   //upmu_set_rg_baton_en(1);
	   pmic_set_register_value(PMIC_RG_BATON_EN,1);
	   //*(kal_bool*)(data) = upmu_get_rgs_baton_undet();
       *(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_BATON_UNDET);
	   
	   return status;
 }


 static u32 charging_get_charger_det_status(void *data)
 {
	u32 status = STATUS_OK;
 
	//*(kal_bool*)(data) = upmu_get_rgs_chrdet();
	*(kal_bool*)(data) = pmic_get_register_value(PMIC_RGS_CHRDET);
	   
	//if( upmu_get_rgs_chrdet() == 0 )
	if( pmic_get_register_value(PMIC_RGS_CHRDET) == 0)
		g_charger_type = CHARGER_UNKNOWN;
       
	return status;
 }


kal_bool charging_type_detection_done(void)
{
	 return charging_type_det_done;
}


 static u32 charging_get_charger_type(void *data)
 {
	 u32 status = STATUS_OK;
#if defined(CONFIG_POWER_EXT)
	 *(CHARGER_TYPE*)(data) = STANDARD_HOST;
#else
#if defined(MTK_WIRELESS_CHARGER_SUPPORT)
	int wireless_state = 0;
	wireless_state = mt_get_gpio_in(wireless_charger_gpio_number);
	if(wireless_state == WIRELESS_CHARGER_EXIST_STATE) {
		*(CHARGER_TYPE*)(data) = WIRELESS_CHARGER;
		battery_xlog_printk(BAT_LOG_CRTI, "WIRELESS_CHARGER!\r\n");
		return status;
	}
#endif
	if(g_charger_type!=CHARGER_UNKNOWN && g_charger_type!=WIRELESS_CHARGER) {
		*(CHARGER_TYPE*)(data) = g_charger_type;
		battery_xlog_printk(BAT_LOG_CRTI, "return %d!\r\n", g_charger_type);
		return status;
	}

	charging_type_det_done = KAL_FALSE;
	/********* Step initial  ***************/		 
	hw_bc11_init();
 
	/********* Step DCD ***************/  
	if(1 == hw_bc11_DCD())
	{
		 /********* Step A1 ***************/
		 if(1 == hw_bc11_stepA1())
		 {
			*(CHARGER_TYPE*)(data) = APPLE_2_1A_CHARGER;
			battery_xlog_printk(BAT_LOG_CRTI, "step A1 : Apple 2.1A CHARGER!\r\n");
			 }	 
			 else
			 {
				*(CHARGER_TYPE*)(data) = NONSTANDARD_CHARGER;
				battery_xlog_printk(BAT_LOG_CRTI, "step A1 : Non STANDARD CHARGER!\r\n");
			 }	 
		 }
	else
	{
		 /********* Step A2 ***************/
		 if(1 == hw_bc11_stepA2())
		 {
			 /********* Step B2 ***************/
			 if(1 == hw_bc11_stepB2())
			 {
				 *(CHARGER_TYPE*)(data) = STANDARD_CHARGER;
				 battery_xlog_printk(BAT_LOG_CRTI, "step B2 : STANDARD CHARGER!\r\n");
			 }
			 else
			 {
				 *(CHARGER_TYPE*)(data) = CHARGING_HOST;
				 battery_xlog_printk(BAT_LOG_CRTI, "step B2 :  Charging Host!\r\n");
			 }
		 }
		 else
		 {
			*(CHARGER_TYPE*)(data) = STANDARD_HOST;
			 battery_xlog_printk(BAT_LOG_CRTI, "step A2 : Standard USB Host!\r\n");
		 }
 
	}
 
	 /********* Finally setting *******************************/
	 hw_bc11_done();

 	charging_type_det_done = KAL_TRUE;

	g_charger_type = *(CHARGER_TYPE*)(data);
#endif
	 return status;
}

static u32 charging_get_is_pcm_timer_trigger(void *data)
{
    u32 status = STATUS_OK;
/*
    if(slp_get_wake_reason() == WR_PCM_TIMER)
        *(kal_bool*)(data) = KAL_TRUE;
    else
        *(kal_bool*)(data) = KAL_FALSE;

    battery_xlog_printk(BAT_LOG_CRTI, "slp_get_wake_reason=%d\n", slp_get_wake_reason());
       */
    return status;
}

static u32 charging_set_platform_reset(void *data)
{
    u32 status = STATUS_OK;

    battery_xlog_printk(BAT_LOG_CRTI, "charging_set_platform_reset\n");
 
    //arch_reset(0,NULL);
        
    return status;
}

static u32 charging_get_platform_boot_mode(void *data)
{
    u32 status = STATUS_OK;
  
    *(u32*)(data) = get_boot_mode();

    battery_xlog_printk(BAT_LOG_CRTI, "get_boot_mode=%d\n", get_boot_mode());
         
    return status;
}

static u32 charging_set_power_off(void *data)
{
    u32 status = STATUS_OK;

    battery_xlog_printk(BAT_LOG_CRTI, "charging_set_power_off\n");
    mt_power_off();

    return status;
}

static u32 charging_get_power_source(void *data)
{
	u32 status = STATUS_UNSUPPORTED;

	return status;
}

static u32 charging_get_csdac_full_flag(void *data)
{
	return STATUS_UNSUPPORTED;	
}

static u32 charging_set_ta_current_pattern(void *data)
{
	return STATUS_UNSUPPORTED;	
}

static u32 charging_get_error_state(void)
{
	return charging_error;
}

static u32 charging_set_error_state(void *data)
{
	u32 status = STATUS_OK;
	charging_error = *(u32*)(data);
	
	return status;
}

static u32 charging_set_vbus_ovp_en(void *data)
{
	return STATUS_OK;
}

static u32 charging_set_vindpm(void *data)
{
	return STATUS_OK;
}

static unsigned int(*charging_func[CHARGING_CMD_NUMBER]) (void *data);
/*
 	  charging_hw_init
	,charging_dump_register  	
	,charging_enable
	,charging_set_cv_voltage
	,charging_get_current
	,charging_set_current
	,charging_set_input_current
	,charging_get_charging_status
	,charging_reset_watch_dog_timer
	,charging_set_hv_threshold
	,charging_get_hv_status
	,charging_get_battery_status
	,charging_get_charger_det_status
	,charging_get_charger_type
	,charging_get_is_pcm_timer_trigger
	,charging_set_platform_reset
	,charging_get_platfrom_boot_mode
	,charging_set_power_off
	,charging_get_power_source
	,charging_get_csdac_full_flag
	,charging_set_ta_current_pattern
	,charging_set_error_state
 };

 
 /*
 * FUNCTION
 *		Internal_chr_control_handler
 *
 * DESCRIPTION															 
 *		 This function is called to set the charger hw
 *
 * CALLS  
 *
 * PARAMETERS
 *		None
 *	 
 * RETURNS
 *		
 *
 * GLOBALS AFFECTED
 *	   None
 */
 s32 chr_control_interface(CHARGING_CTRL_CMD cmd, void *data)
 {
	 static signed int init = -1;

	if (init == -1) {
		init = 0;
		charging_func[CHARGING_CMD_INIT] = charging_hw_init;
		charging_func[CHARGING_CMD_DUMP_REGISTER] = charging_dump_register;
		charging_func[CHARGING_CMD_ENABLE] = charging_enable;
		charging_func[CHARGING_CMD_SET_CV_VOLTAGE] = charging_set_cv_voltage;
		charging_func[CHARGING_CMD_GET_CURRENT] = charging_get_current;
		charging_func[CHARGING_CMD_SET_CURRENT] = charging_set_current;
		charging_func[CHARGING_CMD_SET_INPUT_CURRENT] = charging_set_input_current;
		charging_func[CHARGING_CMD_GET_CHARGING_STATUS] =  charging_get_charging_status;
		charging_func[CHARGING_CMD_RESET_WATCH_DOG_TIMER] = charging_reset_watch_dog_timer;
		charging_func[CHARGING_CMD_SET_HV_THRESHOLD] = charging_set_hv_threshold;
		charging_func[CHARGING_CMD_GET_HV_STATUS] = charging_get_hv_status;
		charging_func[CHARGING_CMD_GET_BATTERY_STATUS] = charging_get_battery_status;
		charging_func[CHARGING_CMD_GET_CHARGER_DET_STATUS] = charging_get_charger_det_status;
		charging_func[CHARGING_CMD_GET_CHARGER_TYPE] = charging_get_charger_type;
		charging_func[CHARGING_CMD_SET_PLATFORM_RESET] = charging_set_platform_reset;
		charging_func[CHARGING_CMD_GET_PLATFORM_BOOT_MODE] = charging_get_platform_boot_mode;
		charging_func[CHARGING_CMD_SET_POWER_OFF] = charging_set_power_off;
		charging_func[CHARGING_CMD_SET_TA_CURRENT_PATTERN] = charging_set_ta_current_pattern;
		charging_func[CHARGING_CMD_SET_VBUS_OVP_EN] = charging_set_vbus_ovp_en;
		charging_func[CHARGING_CMD_SET_VINDPM] = charging_set_vindpm;
	}

	if (cmd < CHARGING_CMD_NUMBER) {
		if (charging_func[cmd] != NULL)
			return charging_func[cmd](data);
	}

	pr_debug("[%s]UNSUPPORT Function: %d\n", __func__, cmd);

	return STATUS_UNSUPPORTED;
 }


