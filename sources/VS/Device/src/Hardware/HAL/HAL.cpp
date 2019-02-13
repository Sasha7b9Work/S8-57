#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SystemClockConfig();
static void MX_GPIO_Init();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::Init()
{
    HAL_Init();

    SystemClockConfig();

    MX_GPIO_Init();

    CRC32_::Init();

    FSMC::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SystemClockConfig()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::ErrorHandler(const char *, int)
{
    while (true) {}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint HAL::TimeMS()
{
    return HAL_GetTick();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void MX_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, SEL_DISP_Pin | LED1_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF, TESTER_ON_Pin | TESTER_PNP_Pin | TESTER_U_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, TESTER_I_Pin | A_LF1_Pin | A_LF2_Pin | A_A1S_Pin
        | A_A0S_Pin | A_LFS_Pin | A_temp_Pin | SPI3_CS2_Pin
        | SPI3_CS3_Pin | KTG15_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(KT_GPIO_Port, KT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, A_A1_Pin | A_A2_Pin | A_A3_Pin | A_A4_Pin
        | SPI3_CS1_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : SEL_DISP_Pin LED1_Pin */
    GPIO_InitStruct.Pin = SEL_DISP_Pin | LED1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : TESTER_ON_Pin TESTER_PNP_Pin TESTER_U_Pin */
    GPIO_InitStruct.Pin = TESTER_ON_Pin | TESTER_PNP_Pin | TESTER_U_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : TESTER_I_Pin A_LF1_Pin A_LF2_Pin A_A1S_Pin
    A_A0S_Pin A_LFS_Pin A_temp_Pin SPI3_CS2_Pin
    SPI3_CS3_Pin KTG15_Pin */
    GPIO_InitStruct.Pin = TESTER_I_Pin | A_LF1_Pin | A_LF2_Pin | A_A1S_Pin
        | A_A0S_Pin | A_LFS_Pin | A_temp_Pin | SPI3_CS2_Pin
        | SPI3_CS3_Pin | KTG15_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pin : FLAG0_Pin */
    GPIO_InitStruct.Pin = FLAG0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FLAG0_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : KT_Pin */
    GPIO_InitStruct.Pin = KT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PB11 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : A_A1_Pin A_A2_Pin A_A3_Pin A_A4_Pin
    SPI3_CS1_Pin */
    GPIO_InitStruct.Pin = A_A1_Pin | A_A2_Pin | A_A3_Pin | A_A4_Pin
        | SPI3_CS1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : TESTER_STR_Pin */
    GPIO_InitStruct.Pin = TESTER_STR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TESTER_STR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PC10 PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : FL_DISP_Pin */
    GPIO_InitStruct.Pin = FL_DISP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FL_DISP_GPIO_Port, &GPIO_InitStruct);
}
