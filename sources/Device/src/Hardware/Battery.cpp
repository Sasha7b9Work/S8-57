#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Hardware/Battery.h"
#include "Hardware/Hardware.h"
#include <stm32f4xx_hal.h>
#endif


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Battery::showOnDisplay = false;

static ADC_HandleTypeDef handleADC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Battery::Init()
{
    // Теперь настроим вход АЦП от батареи - будем считывать значение напряжения
    // PA2 - ADC3 IN2
    // Режим работы:

    __ADC3_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_2,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOA, &isGPIOadc);

    handleADC.Instance = ADC3;
    handleADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handleADC.Init.Resolution = ADC_RESOLUTION12b;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.ScanConvMode = DISABLE;
    handleADC.Init.EOCSelection = DISABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DMAContinuousRequests = DISABLE;
    handleADC.Init.NbrOfConversion = 1;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONV_T1_CC1;  // Программный запуск преобразования вручную
    handleADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONVEDGE_NONE;

    if (HAL_ADC_Init(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Battery::GetVoltage()
{
    if (HAL_ADC_Start(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_PollForConversion(&handleADC, 10) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    uint value = 0;

    if ((HAL_ADC_GetState(&handleADC) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
    {
        value = HAL_ADC_GetValue(&handleADC);
    }

    return (float)value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Battery::Draw()
{
    int width = 50;
    int height = 25;

    int x = Grid::Right() - width;
    int y = Grid::Top();

    Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(String("%d", GetVoltage())).Draw(x + 1, y + 1);
}
