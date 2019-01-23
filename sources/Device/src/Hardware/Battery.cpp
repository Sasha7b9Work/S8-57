#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Hardware/Battery.h"
#include "Hardware/Hardware.h"
#include <stm32f4xx_hal.h>

#include "Settings/Settings.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Для датчика напряжения аккумулятора
static ADC_HandleTypeDef handle;
/// Для конфигурации
static ADC_ChannelConfTypeDef config =
{
    ADC_CHANNEL_2,
    1,
    ADC_SAMPLETIME_480CYCLES,
    0
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Читает напряжение с АЦП в соответствии с установками
static uint ReadVoltage();
/// Перевод считанного значения ЦАП источника в вольты
static float PowerADC_ToVoltage(float value);
/// Перевод считанного значения ЦАП батареи в вольты
static float BatADC_ToVoltage(float value);
/// Максимальное значение, которое возможно считать с АЦП
static const float MAX_ADC_REL = (float)((1 << 12) - 1);
/// Напряжение, соответствующее MAX_ADC_REL
static const float MAX_ADC_ABS = 3.0F;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Battery::Init()
{
    // Настроим входы АЦП для контроля напряжений
    // 36 : PA2 - ADC1 IN2 - контроль АКБ
    // 47 : PB1 - ADC1 IN9 - контроль источника
    // Режим работы:

    __ADC1_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_2,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOA, &isGPIOadc);

    isGPIOadc.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &isGPIOadc);

    handle.Instance = ADC1;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION_12B;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;  // Программный запуск преобразования вручную
    handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    if (HAL_ADC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Battery::GetVoltageAKK()
{
    config.Channel = ADC_CHANNEL_2;

    uint result = 0;
    int count = 4;

    for (int i = 0; i < count; i++)
    {
        result += ReadVoltage();
    }

    return (float)result / (float)count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Battery::GetVoltagePOW()
{
    config.Channel = ADC_CHANNEL_9;

    return PowerADC_ToVoltage((float)ReadVoltage());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadVoltage()
{
    if (HAL_ADC_ConfigChannel(&handle, &config) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    do
    {
    } while (HAL_ADC_PollForConversion(&handle, 10) != HAL_OK);

    return HAL_ADC_GetValue(&handle);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Battery::Draw()
{
    if (!BAT_SHOW_ON_DISPLAY)
    {
        //return;
    }

    int width = 50;
    int height = 25;

    int x = Grid::Right() - width;
    int y = Grid::Top();

    Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(String("%f", GetVoltageAKK())).Draw(x + 2, y + 1);

    Text(String("%f", GetVoltagePOW())).Draw(x + 2, y + 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float PowerADC_ToVoltage(float value)
{
    const float k = 130.0f / 30.0f;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float BatADC_ToVoltage(float value)
{

}
