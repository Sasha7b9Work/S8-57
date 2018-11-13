#pragma once
#include "Hardware/stm32/stm32.h"


/** @defgroup Hardware
*   @brief Аппаратное обеспечение
*   @{
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PRIORITY_SYS_TICK           0, 0
#define PRIORITY_FPGA_ADC           1, 0
#define PRIORITY_FPGA_P2P           1, 1
#define PRIORITY_TIMER_TIM6         2, 0
#define PRIORITY_PANEL_EXTI9_5      3, 0
#define PRIORITY_PANEL_SPI1         4, 0
#define PRIORITY_SOUND_DMA1_STREAM5 5, 0
#define PRIORITY_FLASHDRIVE_OTG     6, 0
#define PRIORITY_VCP_OTG            7, 0
#define PRIORITY_RAM_DMA2_STREAM0   8, 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hardware
{
public:

    static void Init();

    static uint CalculateCRC32(uint address, uint size);

private:

    static void SystemClock_Config(void);

    static void MX_GPIO_Init(void);
    static void MX_ADC3_Init(void);
    static void MX_DAC_Init(void);
    static void MX_SPI4_Init(void);
    static void MX_USB_OTG_FS_PCD_Init(void);
    static void MX_USB_OTG_HS_PCD_Init(void);
};


/** @}
*/
