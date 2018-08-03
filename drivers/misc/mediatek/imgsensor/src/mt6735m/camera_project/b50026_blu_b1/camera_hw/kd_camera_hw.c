#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "kd_camera_hw.h"

#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_camera_feature.h"

/******************************************************************************
 * Debug configuration
******************************************************************************/
#define PFX "[kd_camera_hw]"
#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
#define PK_DBG_FUNC(fmt, args...)    pr_debug(PFX  fmt, ##args)

#define DEBUG_CAMERA_HW_K
#ifdef DEBUG_CAMERA_HW_K
#define PK_DBG PK_DBG_FUNC
#define PK_ERR(fmt, arg...) pr_err(fmt, ##arg)
#define PK_XLOG_INFO(fmt, args...)  pr_debug(PFX  fmt, ##args)

#else
#define PK_DBG(a, ...)
#define PK_ERR(a, ...)
#define PK_XLOG_INFO(fmt, args...)
#endif


#if 1// !defined(CONFIG_MTK_LEGACY)

/* GPIO Pin control*/
struct platform_device *cam_plt_dev = NULL;
struct pinctrl *camctrl = NULL;
struct pinctrl_state *cam0_pnd_h = NULL;
struct pinctrl_state *cam0_pnd_l = NULL;
struct pinctrl_state *cam0_rst_h = NULL;
struct pinctrl_state *cam0_rst_l = NULL;
struct pinctrl_state *cam1_pnd_h = NULL;
struct pinctrl_state *cam1_pnd_l = NULL;
struct pinctrl_state *cam1_rst_h = NULL;
struct pinctrl_state *cam1_rst_l = NULL;
struct pinctrl_state *cam_ldo0_h = NULL;
struct pinctrl_state *cam_ldo0_l = NULL;

int mtkcam_gpio_init(struct platform_device *pdev)
{
	int ret = 0;

	camctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(camctrl)) {
		dev_err(&pdev->dev, "Cannot find camera pinctrl!");
		ret = PTR_ERR(camctrl);
	}
	/*Cam0 Power/Rst Ping initialization */
	cam0_pnd_h = pinctrl_lookup_state(camctrl, "cam0_pnd1");
	if (IS_ERR(cam0_pnd_h)) {
		ret = PTR_ERR(cam0_pnd_h);
		pr_debug("%s : pinctrl err, cam0_pnd_h\n", __func__);
	}

	cam0_pnd_l = pinctrl_lookup_state(camctrl, "cam0_pnd0");
	if (IS_ERR(cam0_pnd_l)) {
		ret = PTR_ERR(cam0_pnd_l);
		pr_debug("%s : pinctrl err, cam0_pnd_l\n", __func__);
	}


	cam0_rst_h = pinctrl_lookup_state(camctrl, "cam0_rst1");
	if (IS_ERR(cam0_rst_h)) {
		ret = PTR_ERR(cam0_rst_h);
		pr_debug("%s : pinctrl err, cam0_rst_h\n", __func__);
	}

	cam0_rst_l = pinctrl_lookup_state(camctrl, "cam0_rst0");
	if (IS_ERR(cam0_rst_l)) {
		ret = PTR_ERR(cam0_rst_l);
		pr_debug("%s : pinctrl err, cam0_rst_l\n", __func__);
	}

	/*Cam1 Power/Rst Ping initialization */
	cam1_pnd_h = pinctrl_lookup_state(camctrl, "cam1_pnd1");
	if (IS_ERR(cam1_pnd_h)) {
		ret = PTR_ERR(cam1_pnd_h);
		pr_debug("%s : pinctrl err, cam1_pnd_h\n", __func__);
	}

	cam1_pnd_l = pinctrl_lookup_state(camctrl, "cam1_pnd0");
	if (IS_ERR(cam1_pnd_l)) {
		ret = PTR_ERR(cam1_pnd_l);
		pr_debug("%s : pinctrl err, cam1_pnd_l\n", __func__);
	}


	cam1_rst_h = pinctrl_lookup_state(camctrl, "cam1_rst1");
	if (IS_ERR(cam1_rst_h)) {
		ret = PTR_ERR(cam1_rst_h);
		pr_debug("%s : pinctrl err, cam1_rst_h\n", __func__);
	}


	cam1_rst_l = pinctrl_lookup_state(camctrl, "cam1_rst0");
	if (IS_ERR(cam1_rst_l)) {
		ret = PTR_ERR(cam1_rst_l);
		pr_debug("%s : pinctrl err, cam1_rst_l\n", __func__);
	}
	/*externel LDO enable */
	cam_ldo0_h = pinctrl_lookup_state(camctrl, "cam_ldo0_1");
	if (IS_ERR(cam_ldo0_h)) {
		ret = PTR_ERR(cam_ldo0_h);
		pr_debug("%s : pinctrl err, cam_ldo0_h\n", __func__);
	}


	cam_ldo0_l = pinctrl_lookup_state(camctrl, "cam_ldo0_0");
	if (IS_ERR(cam_ldo0_l)) {
		ret = PTR_ERR(cam_ldo0_l);
		pr_debug("%s : pinctrl err, cam_ldo0_l\n", __func__);
	}
	return ret;
}

int mtkcam_gpio_set(int PinIdx, int PwrType, int Val)
{
	int ret = 0;

	switch (PwrType) {
	case CAMRST:
		if (PinIdx == 0) {
			if (Val == 0)
				pinctrl_select_state(camctrl, cam0_rst_l);
			else
				pinctrl_select_state(camctrl, cam0_rst_h);
		} else {
			if (Val == 0)
				pinctrl_select_state(camctrl, cam1_rst_l);
			else
				pinctrl_select_state(camctrl, cam1_rst_h);
		}
		break;
	case CAMPDN:
		if (PinIdx == 0) {
			if (Val == 0)
				pinctrl_select_state(camctrl, cam0_pnd_l);
			else
				pinctrl_select_state(camctrl, cam0_pnd_h);
		} else {
			if (Val == 0)
				pinctrl_select_state(camctrl, cam1_pnd_l);
			else
				pinctrl_select_state(camctrl, cam1_pnd_h);
		}

		break;
	case CAMLDO:
		if (Val == 0)
			pinctrl_select_state(camctrl, cam_ldo0_l);
		else
			pinctrl_select_state(camctrl, cam_ldo0_h);
		break;
	default:
		PK_DBG("PwrType(%d) is invalid !!\n", PwrType);
		break;
	};

	PK_DBG("PinIdx(%d) PwrType(%d) val(%d)\n", PinIdx, PwrType, Val);

	return ret;
}




int cntVCAMD = 0;
int cntVCAMA = 0;
int cntVCAMIO = 0;
int cntVCAMAF = 0;
int cntVCAMD_SUB = 0;

static DEFINE_SPINLOCK(kdsensor_pw_cnt_lock);


bool _hwPowerOnCnt(KD_REGULATOR_TYPE_T powerId, int powerVolt, char *mode_name)
{

	if (_hwPowerOn(powerId, powerVolt)) {
		spin_lock(&kdsensor_pw_cnt_lock);
		if (powerId == VCAMD)
			cntVCAMD += 1;
		else if (powerId == VCAMA)
			cntVCAMA += 1;
		else if (powerId == VCAMIO)
			cntVCAMIO += 1;
		else if (powerId == VCAMAF)
			cntVCAMAF += 1;
		else if (powerId == SUB_VCAMD)
			cntVCAMD_SUB += 1;
		spin_unlock(&kdsensor_pw_cnt_lock);
		return true;
	}
	return false;
}

bool _hwPowerDownCnt(KD_REGULATOR_TYPE_T powerId, char *mode_name)
{

	if (_hwPowerDown(powerId)) {
		spin_lock(&kdsensor_pw_cnt_lock);
		if (powerId == VCAMD)
			cntVCAMD -= 1;
		else if (powerId == VCAMA)
			cntVCAMA -= 1;
		else if (powerId == VCAMIO)
			cntVCAMIO -= 1;
		else if (powerId == VCAMAF)
			cntVCAMAF -= 1;
		else if (powerId == SUB_VCAMD)
			cntVCAMD_SUB -= 1;
		spin_unlock(&kdsensor_pw_cnt_lock);
		return true;
	}
	return false;
}

void checkPowerBeforClose(char *mode_name)
{

	int i = 0;

	PK_DBG
	    ("[checkPowerBeforClose]cntVCAMD:%d, cntVCAMA:%d,cntVCAMIO:%d, cntVCAMAF:%d, cntVCAMD_SUB:%d,\n",
	     cntVCAMD, cntVCAMA, cntVCAMIO, cntVCAMAF, cntVCAMD_SUB);


	for (i = 0; i < cntVCAMD; i++)
		_hwPowerDown(VCAMD);
	for (i = 0; i < cntVCAMA; i++)
		_hwPowerDown(VCAMA);
	for (i = 0; i < cntVCAMIO; i++)
		_hwPowerDown(VCAMIO);
	for (i = 0; i < cntVCAMAF; i++)
		_hwPowerDown(VCAMAF);
	for (i = 0; i < cntVCAMD_SUB; i++)
		_hwPowerDown(SUB_VCAMD);

	cntVCAMD = 0;
	cntVCAMA = 0;
	cntVCAMIO = 0;
	cntVCAMAF = 0;
	cntVCAMD_SUB = 0;

}



int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, bool On,
		       char *mode_name)
{

	u32 pinSetIdx = 0;

#define IDX_PS_CMRST 0
#define IDX_PS_CMPDN 4
#define IDX_PS_MODE 1
#define IDX_PS_ON   2
#define IDX_PS_OFF  3

#define VOL_2800 2800000
#define VOL_1800 1800000
#define VOL_1500 1500000
#define VOL_1200 1200000
#define VOL_1000 1000000

	u32 pinSet[3][8] = {

		{CAMERA_CMRST_PIN,
		 CAMERA_CMRST_PIN_M_GPIO,	/* mode */
		 GPIO_OUT_ONE,	/* ON state */
		 GPIO_OUT_ZERO,	/* OFF state */
		 CAMERA_CMPDN_PIN,
		 CAMERA_CMPDN_PIN_M_GPIO,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 },
		{CAMERA_CMRST1_PIN,
		 CAMERA_CMRST1_PIN_M_GPIO,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 CAMERA_CMPDN1_PIN,
		 CAMERA_CMPDN1_PIN_M_GPIO,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 },
		{GPIO_CAMERA_INVALID,
		 GPIO_CAMERA_INVALID,	/* mode */
		 GPIO_OUT_ONE,	/* ON state */
		 GPIO_OUT_ZERO,	/* OFF state */
		 GPIO_CAMERA_INVALID,
		 GPIO_CAMERA_INVALID,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 }
	};



	if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx)
		pinSetIdx = 0;
	 else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx)
		pinSetIdx = 1;
	 else if (DUAL_CAMERA_MAIN_2_SENSOR == SensorIdx)
		pinSetIdx = 2;

	if (On) {
		if((0 == strcmp(currSensorName,"imx135mipiraw"))&&(pinSetIdx != 0))
			PK_DBG("[PowerON]135 not main\n");
		else
			ISP_MCLK1_EN(1);

		PK_DBG("[PowerON]pinSetIdx:%d, currSensorName: %s\n", pinSetIdx, currSensorName);

		if (currSensorName && (0 == strcmp(currSensorName, "imx135mipiraw")))  {
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}


			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}

			if (TRUE != _hwPowerOnCnt(VCAMAF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);


			if (TRUE != _hwPowerOnCnt(VCAMA, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A), power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1000, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D), power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);


			if (TRUE != _hwPowerOnCnt(VCAMIO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(2);



			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

				if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
					mtkcam_gpio_set(pinSetIdx, CAMPDN,
							pinSet[pinSetIdx][IDX_PS_CMPDN +
									  IDX_PS_ON]);
				}
			}

		}
		//2365 5005
	
		else if (currSensorName &&
			((0 == strcmp(SENSOR_DRVNAME_S5K3L8_MIPI_RAW, currSensorName))  )) {
			/* First Power Pin low and Reset Pin Low */
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
						#if 1
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			
			if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			
			mdelay(5);
			
			/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);
			/* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);


			#if 1
			/* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n", VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}
			#endif
			mdelay(10);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			mdelay(5);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(10);
			/*add by lkj 2016/3/4 */

		}
		
	
        
                   else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SUB_GC5005_MIPI_RAW, currSensorName)))
        {   		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

		    /* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
			mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }

	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV8856_MIPI_RAW, currSensorName)))
        {
			PK_DBG("   fanfan SENSOR_DRVNAME_OV8856_MIPI_RAW   \n");
			#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			#endif
			/* First Power Pin low and Reset Pin Low */
		#if 1
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
				pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
		#endif 
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
				pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			mdelay(20);
			ISP_MCLK1_EN(1);
			/* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(20);

			/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(20);

			if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(10);
			pinctrl_select_state(camctrl, cam_ldo0_h);
			 /* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}
			
            			/* enable active sensor */
            #if 1		
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
			#endif
			//mdelay(20);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(20);
        }
		
		 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_GC5025_MIPI_RAW, currSensorName)))
        {
             	#if 1
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            //mtkcam_gpio_set(1, CAMPDN, pinSet[1][IDX_PS_CMPDN + IDX_PS_ON]);
            PK_DBG("B50028_GC5025 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
    
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
		ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
        
        else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_GC5025AF_MIPI_RAW, currSensorName)))
        {
             	#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            
            PK_DBG("B50028_GC5025AF [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
      mdelay(20);
          pinctrl_select_state(camctrl, cam_ldo0_h);
            /* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(20);
		ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        } 
        
       else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_OV8856_MIPI_RAW, currSensorName)))
        {
             	#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            
            PK_DBG("B50028_OV8856 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
      mdelay(20);
          pinctrl_select_state(camctrl, cam_ldo0_h);
            /* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(20);
		ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }  
        
		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SUB_GC2385_MIPI_RAW, currSensorName)))
        {
             	#if 1
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            //mtkcam_gpio_set(1, CAMPDN, pinSet[1][IDX_PS_CMPDN + IDX_PS_ON]);
            PK_DBG("B50028_SUB_GC2385 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
		ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			
        }
		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SUB_GC033A_MIPI_RAW, currSensorName)))
        {
             	#if 1
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
   
            PK_DBG("B50028_SUB_GC033A [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

		    /* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);
			#if 0
				//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);
			#endif
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(10);
			ISP_MCLK1_EN(1);
            mdelay(10);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			mdelay(10);;
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
			mdelay(5);;
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			

        }
		
        else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC5025_MIPI_RAW, currSensorName)))
        {
             	#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            //mtkcam_gpio_set(1, CAMPDN, pinSet[1][IDX_PS_CMPDN + IDX_PS_ON]);
            PK_DBG("GC5005 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
		ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
        
        else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SP5506_MIPI_RAW, currSensorName)))
        {
             	#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            //mtkcam_gpio_set(1, CAMPDN, pinSet[1][IDX_PS_CMPDN + IDX_PS_ON]);
            PK_DBG("GC5005 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_HI846_MIPI_RAW, currSensorName)))
			 {
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			pinctrl_select_state(camctrl, cam_ldo0_h);
			/* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(5);

			/* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(20);
		}
		
				else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50048_OV5695_MIPI_RAW, currSensorName)))
        {
            			//mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
			#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			#endif
						/* First Power Pin low and Reset Pin Low */
						if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
							mtkcam_gpio_set(pinSetIdx, CAMPDN,
							pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

						if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
							mtkcam_gpio_set(pinSetIdx, CAMRST,
							pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            
						mdelay(2);
/* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(2);

			/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(2);

			if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

						
			mdelay(5);
            			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(20);

				pinctrl_select_state(camctrl, cam_ldo0_h);
			
			/* AF_VCC */

			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			
        }
		

 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50048_SUB_GC5024_MIPI_RAW, currSensorName)))
        {
             	#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            //mtkcam_gpio_set(1, CAMPDN, pinSet[1][IDX_PS_CMPDN + IDX_PS_ON]);
            PK_DBG("GC5024 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1500)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			
			
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }




 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC5024_MIPI_RAW, currSensorName)))
        {
             	#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
        
            //mtkcam_gpio_set(1, CAMPDN, pinSet[1][IDX_PS_CMPDN + IDX_PS_ON]);
            PK_DBG("GC5024 [PowerON] currSensorName: %s\n", currSensorName);     		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1500)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(20);
			
			pinctrl_select_state(camctrl, cam_ldo0_h);
			
			/* AF_VCC */

			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }



else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC2365_MIPI_RAW, currSensorName))) {
	//		mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			mdelay(10);
			/* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable IO power (VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			mdelay(10);
			#if 0
			if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(10);
			#endif
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);


		
#if 0
			/* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}
#endif

			mdelay(10);
			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
				mdelay(5);
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}
			mdelay(5);
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				mdelay(5);
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			}
			mdelay(5);
		
		}

		   else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC5005_MIPI_RAW, currSensorName)))
        {   		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

		    /* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);
            mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
            #if 1
			/* AF_VCC */
			if (TRUE != _hwPowerOn(VCAMAF, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n", VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}
			#endif
			mdelay(10);
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
			mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
		
		
		 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SUB_SP250A_MIPI_RAW, currSensorName)))
        {   		
			PK_DBG("  B50028_SUB_SP250A power on \n");
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

		    /* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			#if 0
				//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			#endif
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
			mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
		
		 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SP5509_MIPI_RAW, currSensorName)))
        {   		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			PK_DBG("B50028_SP5509 power on  \n");
		    /* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			#if 1
				//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			#endif
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
     	    if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
			mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
		
		
        else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SP2509_MIPI_RAW, currSensorName)))
        {   		            
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

     			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

		    /* VCAM_IO */
			if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n", VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			#if 0
				//VCAM_D
				if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			#endif
          		/* VCAM_A */
			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n", VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
            mdelay(10);;
			ISP_MCLK1_EN(1);
            //enable active sensor
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
     	    if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
			mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			
        }
		 else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_OV5648_MIPI_RAW, currSensorName))) {
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);

			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			/* VCAM_IO */
			if (TRUE != _hwPowerOnCnt(VCAMIO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* VCAM_A */
			if (TRUE != _hwPowerOnCnt(VCAMA, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1500, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(5);

			/* AF_VCC */
			if (TRUE != _hwPowerOnCnt(VCAMAF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(20);
		}
		

		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_HI843B_MIPI_RAW,currSensorName)))
			  {
			  	
			  		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
	
	            mdelay(50);
				mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
	            PK_DBG("kdCISModulePowerOn get in---  SENSOR_DRVNAME_OV8858_MIPI_RAW-----FMYFMY \n");
	            PK_DBG("[ON_general 2.8V]sensorIdx:%d \n",SensorIdx);
	
	     			if (TRUE != _hwPowerOn(VCAMA, VOL_2800)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
	            mdelay(10);
	
	     		if (TRUE != _hwPowerOn(VCAMIO, VOL_1800)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
	            mdelay(10);
	
	      		if (TRUE != _hwPowerOn(VCAMD, VOL_1200)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
	            mdelay(10);
	#if 1
	            if(TRUE != _hwPowerOn(VCAMAF, VOL_2800))
	            {
	                PK_DBG("[CAMERA SENSOR] Fail to enable digital power (VCAM_AF),power id = %d\n",VCAMAF);
	                //return -EIO;
	                goto _kdCISModulePowerOn_exit_;
	            }
		#endif 
	            mdelay(50);
	            	if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
		
					  		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

		
				mdelay(10);
	
	            //disable inactive sensor
				if(pinSetIdx == 0) {//disable sub
				   pinSetIdx = 1;
	            }
	            else{
	               pinSetIdx = 0;
	            }
	            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
             if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
		
					  		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
	
	            }
			  }

	



			else if (currSensorName  && (0 == strcmp(SENSOR_DRVNAME_HI553_MIPI_RAW, currSensorName))) 
			{
				#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
		//	mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
			PK_DBG("qingfeng SENSOR_DRVNAME_HI553_MIPI_RAW \n");
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			mdelay(10);
			/* VCAM_IO */
			if (TRUE != _hwPowerOnCnt(VCAMIO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			/* VCAM_A */
			if (TRUE != _hwPowerOnCnt(VCAMA, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1200, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(5);

			/* AF_VCC */
		#if 0
	            if(TRUE != _hwPowerOnCnt(VCAMAF, VOL_2800, mode_name))
	            {
	                PK_DBG("[CAMERA SENSOR] Fail to enable AF power\n");
	                //return -EIO;
	                goto _kdCISModulePowerOn_exit_;
	            }
		#endif
			mdelay(10);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			mdelay(20);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(20);
		}
	else if (currSensorName  && (0 == strcmp(SENSOR_DRVNAME_SUBHI553_MIPI_RAW, currSensorName))) 
			{
				#if 0
			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(1-pinSetIdx, CAMPDN, pinSet[1-pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			mdelay(2);

			if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(1-pinSetIdx, CAMRST, pinSet[1-pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				#endif
		//	mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
			PK_DBG("qingfeng SUBHI553_MIPI_RAW \n");
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			mdelay(10);
/* VCAM_A */
			if (TRUE != _hwPowerOnCnt(VCAMA, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			
			mdelay(10);
			/* VCAM_IO */
			if (TRUE != _hwPowerOnCnt(VCAMIO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR]Fail to enable digital power(VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			

			if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1200, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			mdelay(5);

			/* AF_VCC */
		#if 0
	            if(TRUE != _hwPowerOnCnt(VCAMAF, VOL_2800, mode_name))
	            {
	                PK_DBG("[CAMERA SENSOR] Fail to enable AF power\n");
	                //return -EIO;
	                goto _kdCISModulePowerOn_exit_;
	            }
		#endif
			mdelay(10);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			mdelay(20);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);

			mdelay(20);
		}
		

		 else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_GC2355_MIPI_RAW, currSensorName))) {
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 1);
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			mdelay(50);

			/* VCAM_A */
			if (TRUE != _hwPowerOnCnt(VCAMA, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A),power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			/* VCAM_IO */
			if (TRUE != _hwPowerOnCnt(VCAMIO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable IO power (VCAM_IO),power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1500, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			/* AF_VCC */
			if (TRUE != _hwPowerOnCnt(VCAMAF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}


			mdelay(50);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
				mdelay(5);
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			}
			mdelay(5);
			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
				mdelay(5);
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}

			mdelay(5);

		} else {
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerOnCnt(VCAMIO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerOnCnt(VCAMA, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A), power id = %d\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_D */
			if (currSensorName
			    && (0 == strcmp(SENSOR_DRVNAME_S5K2P8_MIPI_RAW, currSensorName))) {
				if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1200, mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					goto _kdCISModulePowerOn_exit_;
				}
			} else if (currSensorName
				   && (0 ==
				       strcmp(SENSOR_DRVNAME_IMX219_MIPI_RAW, currSensorName))) {
				if (pinSetIdx == 0
				    && TRUE != _hwPowerOnCnt(VCAMD, VOL_1200, mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					goto _kdCISModulePowerOn_exit_;
				}
			} else {	/* Main VCAMD max 1.5V */
				if (TRUE != _hwPowerOnCnt(VCAMD, VOL_1500, mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					goto _kdCISModulePowerOn_exit_;
				}

			}


			/* AF_VCC */
			if (TRUE != _hwPowerOnCnt(VCAMAF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(5);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
			}

			mdelay(1);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON]);
			}
		}

	} else {		/* power OFF */

		PK_DBG("[PowerOFF]pinSetIdx:%d\n", pinSetIdx);

		if ((currSensorName && (0 == strcmp(currSensorName, "imx135mipiraw"))) ||
		    (currSensorName && (0 == strcmp(currSensorName, "imx220mipiraw")))) {
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDownCnt(VCAMAF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDownCnt(VCAMIO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerDownCnt(VCAMA, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}

		}
		//2365 5005


	 	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_HI843B_MIPI_RAW,currSensorName)))
					{
				PK_DBG("kdCISModulePower--off get in---SENSOR_DRVNAME_HI843B_MIPI_RAW-----FMYFMY \n");

				 if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
					mtkcam_gpio_set(pinSetIdx, CAMRST,
							pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			
						  		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
					mtkcam_gpio_set(pinSetIdx, CAMPDN,
							pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
					 mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
				if (TRUE != _hwPowerDown(VCAMD)) {
					PK_DBG
					    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n",
					     VCAMD);
					goto _kdCISModulePowerOn_exit_;
				}

			if (TRUE != _hwPowerDown(VCAMIO)) {
					PK_DBG
					    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n",
					     VCAMIO);
					/* return -EIO; */
					goto _kdCISModulePowerOn_exit_;
				}

			if (TRUE != _hwPowerDown(VCAMA)) {
					PK_DBG
					    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n",
					     VCAMA);
					/* return -EIO; */
					goto _kdCISModulePowerOn_exit_;
				}

					if (TRUE != _hwPowerDown(VCAMAF)) {
					PK_DBG
					    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF),power id = %d\n",
					     VCAMAF);
					/* return -EIO; */
					goto _kdCISModulePowerOn_exit_;
				}

	        }	
			

		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_HI553_MIPI_RAW, currSensorName))) 
		 	{
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}

			ISP_MCLK1_EN(0);

		//	mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
		if (TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			
			/* VCAM_A */
			if (TRUE != _hwPowerDownCnt(VCAMA, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDownCnt(VCAMIO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
		#if 0
				if (TRUE != _hwPowerDownCnt(VCAMAF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
		#endif
		}
		
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SUBHI553_MIPI_RAW, currSensorName))) 
		 	{
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}

			ISP_MCLK1_EN(0);

		//	mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
		if (TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			
			/* VCAM_A */
			if (TRUE != _hwPowerDownCnt(VCAMA, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDownCnt(VCAMIO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
		#if 0
				if (TRUE != _hwPowerDownCnt(VCAMAF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
		#endif
		}	
	
           
                         else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SUB_GC5005_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====GC5005 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
          }


			else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV8856_MIPI_RAW, currSensorName)))
        {

            //Set Power Pin low and Reset Pin Low
            #if 1
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
            	mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
            }
			#endif
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            	mtkcam_gpio_set(pinSetIdx, CAMRST, 
							pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            }
		ISP_MCLK1_EN(0);
	    if (TRUE != _hwPowerDown(VCAMD)) 
	    {
			PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
			goto _kdCISModulePowerOn_exit_;
	     }
            if (TRUE != _hwPowerDown(VCAMA)) 
	     {
			PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
			goto _kdCISModulePowerOn_exit_;
	     }
	     if (TRUE != _hwPowerDown(VCAMIO)) 
	     {
			PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
			goto _kdCISModulePowerOn_exit_;
	     }
			pinctrl_select_state(camctrl, cam_ldo0_l);
	     /* AF_VCC */
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
	     
     }
	 
	 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_GC5025_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B50028_GC5025 power off\n");
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

           			ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
     }
       
      else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_GC5025AF_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B50028_GC5025AF power off\n");
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

           			ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			pinctrl_select_state(camctrl, cam_ldo0_l);
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAMAF),power id= (%d)\n", VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			
      } 
      
      
        else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_OV8856_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B50028_OV8856 power off\n");
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

           			ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			pinctrl_select_state(camctrl, cam_ldo0_l);
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAMAF),power id= (%d)\n", VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			
          }   
	 
	 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SUB_GC2385_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B5028_SUB_GC2385 power off\n");
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

           			ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

          }
	 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SUB_GC033A_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B5028_SUB_GC033A power off\n");
			
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    	if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			#if 0
			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			#endif
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			
          }
	 
		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC5025_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====GC5005 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

           			ISP_MCLK1_EN(1);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
          }
          
		
          else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SP5506_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====GC5005 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

           		ISP_MCLK1_EN(1);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
          }
          else if (currSensorName &&
			   (0 == strcmp(SENSOR_DRVNAME_HI846_MIPI_RAW, currSensorName))) {
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

			/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n",
				     VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n",
				     VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			pinctrl_select_state(camctrl, cam_ldo0_l);
			/* AF_VCC */
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

		}
		
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50048_OV5695_MIPI_RAW, currSensorName)))
        {

            //Set Power Pin low and Reset Pin Low
            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
            	mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
            }

            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            	mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            }
			ISP_MCLK1_EN(0);
	    if (TRUE != _hwPowerDown(VCAMD)) 
	    {
			PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
			goto _kdCISModulePowerOn_exit_;
	     }
            if (TRUE != _hwPowerDown(VCAMA)) 
	     {
			PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
			goto _kdCISModulePowerOn_exit_;
	     }
	     if (TRUE != _hwPowerDown(VCAMIO)) 
	     {
			PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
			goto _kdCISModulePowerOn_exit_;
	     }
			pinctrl_select_state(camctrl, cam_ldo0_l);

			/* AF_VCC */

		if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
		 
     }

else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50048_SUB_GC5024_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====GC5024 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			
          }



else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC5024_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====GC5024 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			pinctrl_select_state(camctrl, cam_ldo0_l);

			/* AF_VCC */

		if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

            if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			
          }


		else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC2365_MIPI_RAW, currSensorName))) {
		//	mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);


			/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n",
				     VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			//#if 0
			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			//#endif
			

			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n",
				     VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
		#if 0
			/* AF_VCC */
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF),power id = %d\n",
				     VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
		#endif
		} 
		
		   
              else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC5005_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====GC5005 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
			/* AF_VCC */
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n", VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
          }
		  
	else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SUB_SP250A_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B50028_SUB_SP250A power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			#if 0
			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			#endif
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			
          }	  
		  
		  
		  
		  
  else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_SP2509_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====SP2509 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
          }

		 else if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_B50028_SP5509_MIPI_RAW, currSensorName)))
        {
         PK_DBG("====B50028_SP5509 power off\n");
           if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

    			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
            ISP_MCLK1_EN(0);
				/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
          } 
		  
	else if (currSensorName &&
			(0 == strcmp(SENSOR_DRVNAME_S5K3L8_MIPI_RAW, currSensorName))) {
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN])
				mtkcam_gpio_set(pinSetIdx, CAMPDN, pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST])
				mtkcam_gpio_set(pinSetIdx, CAMRST, pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);

			if (TRUE != _hwPowerDown(VCAMD)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF core power (VCAM_D),power id = %d\n", VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}

			/* VCAM_A */
			if (TRUE != _hwPowerDown(VCAMA)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A),power id= (%d)\n", VCAMA);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			/* VCAM_IO */
			if (TRUE != _hwPowerDown(VCAMIO)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO),power id = %d\n", VCAMIO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			/* AF_VCC */
			if (TRUE != _hwPowerDown(VCAMAF)) {
				PK_DBG("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n", VCAMAF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

		}

		 else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_OV5648_MIPI_RAW, currSensorName))) {
			mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}


			if (TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerDownCnt(VCAMA, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDownCnt(VCAMIO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDownCnt(VCAMAF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

		} else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_GC2355_MIPI_RAW, currSensorName))) {
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON]);
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}

			ISP_MCLK1_EN(0);

			mtkcam_gpio_set(pinSetIdx, CAMLDO, 0);
			if (TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     VCAMD);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerDownCnt(VCAMA, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDownCnt(VCAMIO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDownCnt(VCAMAF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

		} else {
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				mtkcam_gpio_set(pinSetIdx, CAMPDN,
						pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF]);
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				mtkcam_gpio_set(pinSetIdx, CAMRST,
						pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF]);
			}


			if (currSensorName
			    && (0 == strcmp(SENSOR_DRVNAME_IMX219_MIPI_RAW, currSensorName))) {
				if (pinSetIdx == 0 && TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
					PK_DBG
					    ("[CAMERA SENSOR] main imx220 Fail to OFF core power (VCAM_D), power id = %d\n",
					     VCAMD);
					goto _kdCISModulePowerOn_exit_;
				}
			} else {
				if (TRUE != _hwPowerDownCnt(VCAMD, mode_name)) {
					PK_DBG
					    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
					     VCAMD);
					goto _kdCISModulePowerOn_exit_;
				}
			}

			/* VCAM_A */
			if (TRUE != _hwPowerDownCnt(VCAMA, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     VCAMA);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDownCnt(VCAMIO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     VCAMIO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDownCnt(VCAMAF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     VCAMAF);
				goto _kdCISModulePowerOn_exit_;
			}

		}
		if((0 == strcmp(currSensorName,"imx135mipiraw"))&&(pinSetIdx != 0))
			PK_DBG("[PowerOFF]135 not main\n");
		else
			ISP_MCLK1_EN(0);
	}


	return 0;


_kdCISModulePowerOn_exit_:
	return -EIO;

}
#else

/*
#ifndef BOOL
typedef unsigned char BOOL;
#endif
*/


int cntVCAMD = 0;
int cntVCAMA = 0;
int cntVCAMIO = 0;
int cntVCAMAF = 0;
int cntVCAMD_SUB = 0;

static DEFINE_SPINLOCK(kdsensor_pw_cnt_lock);


bool _hwPowerOn(MT65XX_POWER powerId, int powerVolt, char *mode_name)
{

	if (hwPowerOn(powerId, powerVolt, mode_name)) {
		spin_lock(&kdsensor_pw_cnt_lock);
		if (powerId == CAMERA_POWER_VCAM_D)
			cntVCAMD += 1;
		else if (powerId == CAMERA_POWER_VCAM_A)
			cntVCAMA += 1;
		else if (powerId == CAMERA_POWER_VCAM_IO)
			cntVCAMIO += 1;
		else if (powerId == CAMERA_POWER_VCAM_AF)
			cntVCAMAF += 1;
		else if (powerId == SUB_CAMERA_POWER_VCAM_D)
			cntVCAMD_SUB += 1;
		spin_unlock(&kdsensor_pw_cnt_lock);
		return true;
	}
	return false;
}

bool _hwPowerDown(MT65XX_POWER powerId, char *mode_name)
{

	if (hwPowerDown(powerId, mode_name)) {
		spin_lock(&kdsensor_pw_cnt_lock);
		if (powerId == CAMERA_POWER_VCAM_D)
			cntVCAMD -= 1;
		else if (powerId == CAMERA_POWER_VCAM_A)
			cntVCAMA -= 1;
		else if (powerId == CAMERA_POWER_VCAM_IO)
			cntVCAMIO -= 1;
		else if (powerId == CAMERA_POWER_VCAM_AF)
			cntVCAMAF -= 1;
		else if (powerId == SUB_CAMERA_POWER_VCAM_D)
			cntVCAMD_SUB -= 1;
		spin_unlock(&kdsensor_pw_cnt_lock);
		return true;
	}
	return false;
}

void checkPowerBeforClose(char *mode_name)
{

	int i = 0;

	PK_DBG
	    ("[checkPowerBeforClose]cntVCAMD:%d, cntVCAMA:%d,cntVCAMIO:%d, cntVCAMAF:%d, cntVCAMD_SUB:%d,\n",
	     cntVCAMD, cntVCAMA, cntVCAMIO, cntVCAMAF, cntVCAMD_SUB);


	for (i = 0; i < cntVCAMD; i++)
		hwPowerDown(CAMERA_POWER_VCAM_D, mode_name);
	for (i = 0; i < cntVCAMA; i++)
		hwPowerDown(CAMERA_POWER_VCAM_A, mode_name);
	for (i = 0; i < cntVCAMIO; i++)
		hwPowerDown(CAMERA_POWER_VCAM_IO, mode_name);
	for (i = 0; i < cntVCAMAF; i++)
		hwPowerDown(CAMERA_POWER_VCAM_AF, mode_name);
	for (i = 0; i < cntVCAMD_SUB; i++)
		hwPowerDown(SUB_CAMERA_POWER_VCAM_D, mode_name);

	cntVCAMD = 0;
	cntVCAMA = 0;
	cntVCAMIO = 0;
	cntVCAMAF = 0;
	cntVCAMD_SUB = 0;

}



int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On,
		       char *mode_name)
{

	u32 pinSetIdx = 0;	/* default main sensor */

#define IDX_PS_CMRST 0
#define IDX_PS_CMPDN 4
#define IDX_PS_MODE 1
#define IDX_PS_ON   2
#define IDX_PS_OFF  3


	u32 pinSet[3][8] = {
		/* for main sensor */
		{CAMERA_CMRST_PIN,	/* The reset pin of main sensor uses GPIO10 of mt6306, please call mt6306 API to set */
		 CAMERA_CMRST_PIN_M_GPIO,	/* mode */
		 GPIO_OUT_ONE,	/* ON state */
		 GPIO_OUT_ZERO,	/* OFF state */
		 CAMERA_CMPDN_PIN,
		 CAMERA_CMPDN_PIN_M_GPIO,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 },
		/* for sub sensor */
		{CAMERA_CMRST1_PIN,
		 CAMERA_CMRST1_PIN_M_GPIO,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 CAMERA_CMPDN1_PIN,
		 CAMERA_CMPDN1_PIN_M_GPIO,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 },
		/* for main_2 sensor */
		{GPIO_CAMERA_INVALID,
		 GPIO_CAMERA_INVALID,	/* mode */
		 GPIO_OUT_ONE,	/* ON state */
		 GPIO_OUT_ZERO,	/* OFF state */
		 GPIO_CAMERA_INVALID,
		 GPIO_CAMERA_INVALID,
		 GPIO_OUT_ONE,
		 GPIO_OUT_ZERO,
		 }
	};



	if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx)
		pinSetIdx = 0;
	 else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx)
		pinSetIdx = 1;
	 else if (DUAL_CAMERA_MAIN_2_SENSOR == SensorIdx)
		pinSetIdx = 2;

	/* power ON */
	if (On) {
		if((0 == strcmp(currSensorName,"imx135mipiraw"))&&(pinSetIdx != 0))
			PK_DBG("[PowerON]135 not main\n");
		else
            ISP_MCLK1_EN(1);

		PK_DBG("[PowerON]pinSetIdx:%d, currSensorName: %s\n", pinSetIdx, currSensorName);

		if ((currSensorName && (0 == strcmp(currSensorName, "imx135mipiraw"))) ||
		    (currSensorName && (0 == strcmp(currSensorName, "imx220mipiraw")))) {
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}


			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}
			/* AF_VCC */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* VCAM_A */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A), power id = %d\n",
				     CAMERA_POWER_VCAM_A);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1000, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D), power id = %d\n",
				     CAMERA_POWER_VCAM_D);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* VCAM_IO */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_IO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(2);


			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

		} else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_OV5648_MIPI_RAW, currSensorName))) {
			mt_set_gpio_mode(GPIO_SPI_MOSI_PIN, GPIO_MODE_00);
			mt_set_gpio_dir(GPIO_SPI_MOSI_PIN, GPIO_DIR_OUT);
			mt_set_gpio_out(GPIO_SPI_MOSI_PIN, GPIO_OUT_ONE);
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_IO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			/* VCAM_A */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A), power id = %d\n",
				     CAMERA_POWER_VCAM_A);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(1);

			if (TRUE != _hwPowerOn(SUB_CAMERA_POWER_VCAM_D, VOL_1500, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D), power id = %d\n",
				     SUB_CAMERA_POWER_VCAM_D);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(5);

			/* AF_VCC */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				goto _kdCISModulePowerOn_exit_;
			}


			mdelay(1);


			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}


			mdelay(2);


			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}

			}

			mdelay(20);
		} else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_GC2355_MIPI_RAW, currSensorName))) {
			mt_set_gpio_mode(GPIO_SPI_MOSI_PIN, GPIO_MODE_00);
			mt_set_gpio_dir(GPIO_SPI_MOSI_PIN, GPIO_DIR_OUT);
			mt_set_gpio_out(GPIO_SPI_MOSI_PIN, GPIO_OUT_ONE);
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}

			mdelay(50);

			/* VCAM_A */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A), power id = %d\n",
				     CAMERA_POWER_VCAM_A);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			/* VCAM_IO */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_IO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			if (TRUE != _hwPowerOn(SUB_CAMERA_POWER_VCAM_D, VOL_1500, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_D), power id = %d\n",
				     SUB_CAMERA_POWER_VCAM_D);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(10);

			/* AF_VCC */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				goto _kdCISModulePowerOn_exit_;
			}


			mdelay(50);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
				mdelay(5);
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}

			}
			mdelay(5);
			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
				mdelay(5);
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			mdelay(5);
		} else {
			/* First Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_IO, VOL_1800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_A), power id = %d\n",
				     CAMERA_POWER_VCAM_A);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_D */
			if (currSensorName
			    && (0 == strcmp(SENSOR_DRVNAME_S5K2P8_MIPI_RAW, currSensorName))) {
				if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1200, mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					goto _kdCISModulePowerOn_exit_;
				}
			} else if (currSensorName
				   && (0 ==
				       strcmp(SENSOR_DRVNAME_IMX219_MIPI_RAW, currSensorName))) {
				if (pinSetIdx == 0
				    && TRUE != _hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1200,
							  mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					goto _kdCISModulePowerOn_exit_;
				}
			} else {	/* Main VCAMD max 1.5V */
				if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500, mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					goto _kdCISModulePowerOn_exit_;
				}

			}


			/* AF_VCC */
			if (TRUE != _hwPowerOn(CAMERA_POWER_VCAM_AF, VOL_2800, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to enable analog power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				goto _kdCISModulePowerOn_exit_;
			}

			mdelay(5);

			/* enable active sensor */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			mdelay(1);

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_ON])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}
		}
	} else {		/* power OFF */

		PK_DBG("[PowerOFF]pinSetIdx:%d\n", pinSetIdx);
		if((0 == strcmp(currSensorName,"imx135mipiraw"))&&(pinSetIdx != 0))
			PK_DBG("[PowerOFF]135 not main\n");
		else
            ISP_MCLK1_EN(0);

		if ((currSensorName && (0 == strcmp(currSensorName, "imx135mipiraw"))) ||
		    (currSensorName && (0 == strcmp(currSensorName, "imx220mipiraw")))) {
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}
			/* Set Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_AF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_IO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     CAMERA_POWER_VCAM_D);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_A, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     CAMERA_POWER_VCAM_A);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

		} else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_OV5648_MIPI_RAW, currSensorName))) {
			mt_set_gpio_out(GPIO_SPI_MOSI_PIN, GPIO_OUT_ZERO);
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}

			if (TRUE != _hwPowerDown(SUB_CAMERA_POWER_VCAM_D, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     SUB_CAMERA_POWER_VCAM_D);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_A, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     CAMERA_POWER_VCAM_A);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_IO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_AF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

		} else if (currSensorName
			   && (0 == strcmp(SENSOR_DRVNAME_GC2355_MIPI_RAW, currSensorName))) {
			mt_set_gpio_out(GPIO_SPI_MOSI_PIN, GPIO_OUT_ZERO);
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_ON])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}

			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}


			if (TRUE != _hwPowerDown(SUB_CAMERA_POWER_VCAM_D, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
				     SUB_CAMERA_POWER_VCAM_D);
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_A */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_A, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     CAMERA_POWER_VCAM_A);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_IO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_AF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

		} else {
			/* Set Power Pin low and Reset Pin Low */
			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_MODE])) {
					PK_DBG("[CAMERA LENS] set gpio mode failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA LENS] set gpio dir failed!! (CMPDN)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMPDN],
				     pinSet[pinSetIdx][IDX_PS_CMPDN + IDX_PS_OFF])) {
					PK_DBG("[CAMERA LENS] set gpio failed!! (CMPDN)\n");
				}
			}


			if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
				if (mt_set_gpio_mode
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_MODE])) {
					PK_DBG("[CAMERA SENSOR] set gpio mode failed!! (CMRST)\n");
				}
				if (mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST], GPIO_DIR_OUT)) {
					PK_DBG("[CAMERA SENSOR] set gpio dir failed!! (CMRST)\n");
				}
				if (mt_set_gpio_out
				    (pinSet[pinSetIdx][IDX_PS_CMRST],
				     pinSet[pinSetIdx][IDX_PS_CMRST + IDX_PS_OFF])) {
					PK_DBG("[CAMERA SENSOR] set gpio failed!! (CMRST)\n");
				}
			}

			if (currSensorName
			    && (0 == strcmp(SENSOR_DRVNAME_IMX219_MIPI_RAW, currSensorName))) {
				if (pinSetIdx == 0
				    && TRUE != _hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
					PK_DBG
					    ("[CAMERA SENSOR] main imx220 Fail to OFF core power (VCAM_D), power id = %d\n",
					     CAMERA_POWER_VCAM_D);
					goto _kdCISModulePowerOn_exit_;
				}
			} else {

				if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
					PK_DBG
					    ("[CAMERA SENSOR] Fail to OFF core power (VCAM_D), power id = %d\n",
					     CAMERA_POWER_VCAM_D);
					goto _kdCISModulePowerOn_exit_;
				}
			}

			/* VCAM_A */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_A, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF analog power (VCAM_A), power id= (%d)\n",
				     CAMERA_POWER_VCAM_A);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* VCAM_IO */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_IO, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF digital power (VCAM_IO), power id = %d\n",
				     CAMERA_POWER_VCAM_IO);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}
			/* AF_VCC */
			if (TRUE != _hwPowerDown(CAMERA_POWER_VCAM_AF, mode_name)) {
				PK_DBG
				    ("[CAMERA SENSOR] Fail to OFF AF power (VCAM_AF), power id = %d\n",
				     CAMERA_POWER_VCAM_AF);
				/* return -EIO; */
				goto _kdCISModulePowerOn_exit_;
			}

		}

	}

	return 0;

_kdCISModulePowerOn_exit_:
	return -EIO;

}

#endif
EXPORT_SYMBOL(kdCISModulePowerOn);

/* !-- */
/*  */
