#pragma once
#include "usbh_conf.h"


typedef enum
{
    HOST_IDLE = 0,
    HOST_DEV_WAIT_FOR_ATTACHMENT,
    HOST_DEV_ATTACHED,
    HOST_DEV_DISCONNECTED,
    HOST_DETECT_DEVICE_SPEED,
    HOST_ENUMERATION,
    HOST_CLASS_REQUEST,
    HOST_INPUT,
    HOST_SET_CONFIGURATION,
    HOST_CHECK_CLASS,
    HOST_CLASS,
    HOST_SUSPENDED,
    HOST_ABORT_STATE,

} HOST_StateTypeDef;


typedef enum
{
    ENUM_IDLE = 0,
    ENUM_GET_FULL_DEV_DESC,
    ENUM_SET_ADDR,
    ENUM_GET_CFG_DESC,
    ENUM_GET_FULL_CFG_DESC,
    ENUM_GET_MFC_STRING_DESC,
    ENUM_GET_PRODUCT_STRING_DESC,
    ENUM_GET_SERIALNUM_STRING_DESC,

} ENUM_StateTypeDef;


typedef enum
{
    CMD_IDLE = 0,
    CMD_SEND,
    CMD_WAIT

} CMD_StateTypeDef;

typedef enum
{
    USBH_OK = 0,
    USBH_BUSY,
    USBH_FAIL,
    USBH_NOT_SUPPORTED,
    USBH_UNRECOVERED_ERROR,
    USBH_ERROR_SPEED_UNKNOWN,

} USBH_StatusTypeDef;


typedef struct
{
    uint8_t               pipe_in;
    uint8_t               pipe_out;
    uint8_t               pipe_size;
    uint8_t               *buff;
    uint16_t              length;
    uint16_t              timer;
    //USB_Setup_TypeDef     setup;
    //CTRL_StateTypeDef     state;
    uint8     setup;
    uint8     state;
    uint8_t               errorcount;

} USBH_CtrlTypeDef;


typedef struct
{
#if (USBH_KEEP_CFG_DESCRIPTOR == 1)  
    uint8_t                           CfgDesc_Raw[USBH_MAX_SIZE_CONFIGURATION];
#endif  
    uint8_t                           Data[USBH_MAX_DATA_BUFFER];
    uint8_t                           address;
    uint8_t                           speed;
    __IO uint8_t                      is_connected;
    uint8_t                           current_interface;
    //USBH_DevDescTypeDef               DevDesc;
    //USBH_CfgDescTypeDef               CfgDesc;
    uint8               DevDesc;
    uint8               CfgDesc;
}USBH_DeviceTypeDef;


typedef struct
{
    const char          *Name;
    uint8_t              ClassCode;
    USBH_StatusTypeDef(*Init)        (struct _USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef(*DeInit)      (struct _USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef(*Requests)    (struct _USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef(*BgndProcess) (struct _USBH_HandleTypeDef *phost);
    USBH_StatusTypeDef(*SOFProcess) (struct _USBH_HandleTypeDef *phost);
    void*                pData;
} USBH_ClassTypeDef;


typedef struct _USBH_HandleTypeDef
{
    __IO HOST_StateTypeDef     gState;       /*  Host State Machine Value */
    ENUM_StateTypeDef     EnumState;    /* Enumeration state Machine */
    CMD_StateTypeDef      RequestState;
    USBH_CtrlTypeDef      Control;
    USBH_DeviceTypeDef    device;
    USBH_ClassTypeDef*    pClass[USBH_MAX_NUM_SUPPORTED_CLASS];
    USBH_ClassTypeDef*    pActiveClass;
    uint32_t              ClassNumber;
    uint32_t              Pipes[15];
    __IO uint32_t         Timer;
    uint8_t               id;
    void*                 pData;
    void(*pUser)(struct _USBH_HandleTypeDef *pHandle, uint8_t id);
} USBH_HandleTypeDef;
