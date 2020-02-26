#include "usbd_def.h"


USBD_DescriptorsTypeDef VCP_Desc;


USBD_StatusTypeDef USBD_Init(USBD_HandleTypeDef * /*pdev*/, USBD_DescriptorsTypeDef * /*pdesc*/, uint8_t /*id*/)
{
    return USBD_FAIL;
}


USBD_StatusTypeDef USBD_RegisterClass(USBD_HandleTypeDef * /*pdev*/, USBD_ClassTypeDef * /*pclass*/)
{
    return USBD_FAIL;
}


USBD_StatusTypeDef USBD_Start(USBD_HandleTypeDef * /*pdev*/)
{
    return USBD_FAIL;
}
