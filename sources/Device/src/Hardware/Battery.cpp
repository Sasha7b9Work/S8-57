#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Hardware/Battery.h"
#include "Hardware/Hardware.h"
#include <stm32f4xx_hal.h>

#include "Settings/Settings.h"
#include "Utils/Averager.h"
#include "Display/Font/Font.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Для датчика напряжения аккумулятора
static ADC_HandleTypeDef handle;
/// Для конфигурации
static ADC_ChannelConfTypeDef config =
{
    ADC_CHANNEL_2,
    1,
    ADC_SAMPLETIME_3CYCLES,
    0
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Читает напряжение с АЦП в соответствии с установками
static uint ReadVoltage();
/// Перевод считанного значения ЦАП источника в вольты
static float PowerADC_ToVoltage(float value);
/// Перевод считанного значения ЦАП батареи в вольты
static float BatADC_ToVoltage(float value);
/// Читает АЦП батареи
static uint ReadValueAKK();
/// Читает АЦП зарядного устройства
static uint ReadValuePOW();
/// Максимальное значение, которое возможно считать с АЦП
static const float MAX_ADC_REL = (float)((1 << 12) - 1);
/// Напряжение, соответствующее MAX_ADC_REL
static const float MAX_ADC_ABS = 2.91F;

static const float VOLTAGE_100_PERCENTS = 8.0F;

static const float VOLTAGE_0_PERCENTS = 6.0F;


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
float Battery::GetVoltageAKK(uint *adc)
{
    static Utils::AroundAverager<float> averager(32);

    *adc = ReadValueAKK();

    averager.Push((float)*adc);

    return BatADC_ToVoltage(averager.Value());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Battery::GetVoltagePOW(uint *adc)
{
    static Utils::AroundAverager<float> averager(32);

    *adc = ReadValuePOW();

    averager.Push((float)*adc);

    return PowerADC_ToVoltage(averager.Value());
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
    } while (HAL_ADC_PollForConversion(&handle, 1) != HAL_OK);

    return HAL_ADC_GetValue(&handle);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadValueAKK()
{
    config.Channel = ADC_CHANNEL_2;

    return ReadVoltage();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadValuePOW()
{
    config.Channel = ADC_CHANNEL_9;

    return ReadVoltage();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Battery::Draw(int x, int y)
{
    Font::SetCurrent(Font::Type::_8);

    uint akkADC = 0;
    float akk = GetVoltageAKK(&akkADC);

    uint powADC = 0;
    float pow = GetVoltagePOW(&powADC);

    Color::SetCurrent(Color::FILL);

    Text(String("%.3f В", akk)).Draw(x + 2, y + 1);

    Text(String("%.3f В", pow)).Draw(x + 2, y + 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float PowerADC_ToVoltage(float value)
{
    const float k = 124.0F / 24.0F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float BatADC_ToVoltage(float value)
{
    const float k = 101.1F / 26.1F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}
