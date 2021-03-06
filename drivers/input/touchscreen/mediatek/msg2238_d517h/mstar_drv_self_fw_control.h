////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2014 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (??MStar Confidential Information??) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @file    mstar_drv_self_fw_control.h
 *
 * @brief   This file defines the interface of touch screen
 *
 *
 */

#ifndef __MSTAR_DRV_SELF_FW_CONTROL_H__
#define __MSTAR_DRV_SELF_FW_CONTROL_H__

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/

#include "mstar_drv_common.h"

#if defined(CONFIG_ENABLE_TOUCH_DRIVER_FOR_SELF_IC)

/*--------------------------------------------------------------------------*/
/* COMPILE OPTION DEFINITION                                                */
/*--------------------------------------------------------------------------*/

//#define CONFIG_SWAP_X_Y

//#define CONFIG_REVERSE_X
//#define CONFIG_REVERSE_Y

/*--------------------------------------------------------------------------*/
/* PREPROCESSOR CONSTANT DEFINITION                                         */
/*--------------------------------------------------------------------------*/

#define DEMO_MODE_PACKET_LENGTH    (8)
#define MAX_TOUCH_NUM           (2)     

#define MAX_ERASE_EFLASH_TIMES   (2) // for update firmware of MSG22xx 

#define MSG21XXA_FIRMWARE_MAIN_BLOCK_SIZE (32) //32K
#define MSG21XXA_FIRMWARE_INFO_BLOCK_SIZE (1)  //1K
#define MSG21XXA_FIRMWARE_WHOLE_SIZE (MSG21XXA_FIRMWARE_MAIN_BLOCK_SIZE+MSG21XXA_FIRMWARE_INFO_BLOCK_SIZE) //33K

#define MSG22XX_FIRMWARE_MAIN_BLOCK_SIZE (48)  //48K
#define MSG22XX_FIRMWARE_INFO_BLOCK_SIZE (512) //512Byte


//#define MSG21XXA_FIRMWARE_MODE_UNKNOWN_MODE   (0xFF)
#define MSG21XXA_FIRMWARE_MODE_DEMO_MODE      (0x00)
#define MSG21XXA_FIRMWARE_MODE_DEBUG_MODE     (0x01)
#define MSG21XXA_FIRMWARE_MODE_RAW_DATA_MODE  (0x02)

//#define MSG22XX_FIRMWARE_MODE_UNKNOWN_MODE    (0xFF)
#define MSG22XX_FIRMWARE_MODE_DEMO_MODE       (0x00)
#define MSG22XX_FIRMWARE_MODE_DEBUG_MODE      (0x01)
#define MSG22XX_FIRMWARE_MODE_RAW_DATA_MODE   (0x02)


#define DEBUG_MODE_PACKET_LENGTH    (128)

#ifdef CONFIG_UPDATE_FIRMWARE_BY_SW_ID
#define UPDATE_FIRMWARE_RETRY_COUNT (2)
#endif //CONFIG_UPDATE_FIRMWARE_BY_SW_ID

#ifdef CONFIG_ENABLE_GESTURE_WAKEUP
#ifdef CONFIG_ENABLE_GESTURE_INFORMATION_MODE
#define FIRMWARE_GESTURE_INFORMATION_MODE_A	(0x00)
#define FIRMWARE_GESTURE_INFORMATION_MODE_B	(0x01)
#define FIRMWARE_GESTURE_INFORMATION_MODE_C	(0x02)
#endif //CONFIG_ENABLE_GESTURE_INFORMATION_MODE
#endif //CONFIG_ENABLE_GESTURE_WAKEUP

#define SAMPLE_FWVER_DJ1_MAJOR 2
#define  SAMPLE_FWVER_DJ1_MINOR 3
#define SAMPLE_FWVER_HELITAI_MAJOR  1
#define SAMPLE_FWVER_HELITAI_MINOR  11
#define SAMPLE_FWVER_DJ2_MAJOR 3
#define  SAMPLE_FWVER_DJ2_MINOR 2
/*--------------------------------------------------------------------------*/
/* DATA TYPE DEFINITION                                                     */
/*--------------------------------------------------------------------------*/

typedef struct
{
    unsigned short nX;
    unsigned short nY;
} TouchPoint_t;

typedef struct
{
    unsigned char nTouchKeyMode;
    unsigned char nTouchKeyCode;
    unsigned char nFingerNum;
    TouchPoint_t tPoint[MAX_TOUCH_NUM];
} TouchInfo_t;

typedef struct
{
    unsigned char nFirmwareMode;
    unsigned char nLogModePacketHeader;
    unsigned short nLogModePacketLength;
    unsigned char nIsCanChangeFirmwareMode;
} FirmwareInfo_t;


#ifdef CONFIG_UPDATE_FIRMWARE_BY_SW_ID
/*
 * Note.
 * The following is sw id enum definition for MSG22XX.
 * 0x0000 and 0xFFFF are not allowed to be defined as SW ID.
 * SW_ID_UNDEFINED is a reserved enum value, do not delete it or modify it.
 * Please modify the SW ID of the below enum value depends on the TP vendor that you are using.
 */
typedef enum {
    MSG22XX_SW_ID_XXXX = 0x0001,
    MSG22XX_SW_ID_YYYY = 0x0002,  
    MSG22XX_SW_ID_ZZZZ = 0x0003,  
    MSG22XX_SW_ID_UNDEFINED
} Msg22xxSwId_e;


/*
 * Note.
 * The following is sw id enum definition for MSG21XXA.
 * SW_ID_UNDEFINED is a reserved enum value, do not delete it or modify it.
 * Please modify the SW ID of the below enum value depends on the TP vendor that you are using.
 */
typedef enum {
    MSG21XXA_SW_ID_XXXX = 0,  
    MSG21XXA_SW_ID_YYYY,
    MSG21XXA_SW_ID_UNDEFINED
} Msg21xxaSwId_e;
#endif //CONFIG_UPDATE_FIRMWARE_BY_SW_ID

/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION DECLARATION                                              */
/*--------------------------------------------------------------------------*/

#ifdef CONFIG_ENABLE_GESTURE_WAKEUP
extern void DrvFwCtrlOpenGestureWakeup(unsigned int *pMode);
extern void DrvFwCtrlCloseGestureWakeup(void);

#ifdef CONFIG_ENABLE_GESTURE_DEBUG_MODE
extern void DrvFwCtrlOpenGestureDebugMode(unsigned char nGestureFlag);
extern void DrvFwCtrlCloseGestureDebugMode(void);
#endif //CONFIG_ENABLE_GESTURE_DEBUG_MODE
#endif //CONFIG_ENABLE_GESTURE_WAKEUP

extern unsigned int DrvFwCtrlReadDQMemValue(unsigned short nAddr);
extern void DrvFwCtrlWriteDQMemValue(unsigned short nAddr, unsigned int nData);

extern unsigned short DrvFwCtrlChangeFirmwareMode(unsigned short nMode);        
extern void DrvFwCtrlGetFirmwareInfo(FirmwareInfo_t *pInfo);
extern void DrvFwCtrlRestoreFirmwareModeToLogDataMode(void);

#ifdef CONFIG_UPDATE_FIRMWARE_BY_SW_ID
extern void DrvFwCtrlCheckFirmwareUpdateBySwId(void);
#endif //CONFIG_UPDATE_FIRMWARE_BY_SW_ID

#ifdef CONFIG_ENABLE_PROXIMITY_DETECTION
extern signed int DrvFwCtrlEnableProximity(void);
extern signed int DrvFwCtrlDisableProximity(void);
#endif //CONFIG_ENABLE_PROXIMITY_DETECTION

extern void DrvFwCtrlVariableInitialize(void);
extern void DrvFwCtrlOptimizeCurrentConsumption(void);
extern unsigned char DrvFwCtrlGetChipType(void);
extern void DrvFwCtrlGetCustomerFirmwareVersion(unsigned short *pMajor, unsigned short *pMinor, unsigned char **ppVersion);
extern void DrvFwCtrlGetPlatformFirmwareVersion(unsigned char **ppVersion);
extern void DrvFwCtrlHandleFingerTouch(void);
extern signed int DrvFwCtrlUpdateFirmware(unsigned char szFwData[][1024], EmemType_e eEmemType);
extern signed int DrvFwCtrlUpdateFirmwareBySdCard(const char *pFilePath);

#ifdef CONFIG_ENABLE_CHARGER_DETECTION
extern void DrvFwCtrlChargerDetection(unsigned char nChargerStatus);
#endif //CONFIG_ENABLE_CHARGER_DETECTION

#endif //CONFIG_ENABLE_TOUCH_DRIVER_FOR_SELF_IC
        
#endif  /* __MSTAR_DRV_SELF_FW_CONTROL_H__ */
