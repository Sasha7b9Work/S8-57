#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Hardware/Battery.h"
#include "Hardware/Hardware.h"

#include "Settings/Settings.h"
#include "Utils/Averager.h"
#include "Display/Font/Font.h"
#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;


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
/// Рассчитать процент отставшегося заряда
static float CalculatePercents(float volts);
/// Отобразить заряд батареи в графическом виде
static void DrawBatteryUGO(int x, int y, float procents);
/// Максимальное значение, которое возможно считать с АЦП
static const float MAX_ADC_REL = (float)((1 << 12) - 1);
/// Напряжение, соответствующее MAX_ADC_REL
static const float MAX_ADC_ABS = 2.91F;

static const float VOLTAGE_100_PERCENTS = 8.2F;

static const float VOLTAGE_0_PERCENTS = 6.0F;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware::Battery::Init()
{
    HAL::ADC1_::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Hardware::Battery::GetVoltageAKK(uint *adc)
{
    static Utils::AroundAverager<float> averager(32);

    *adc = ReadValueAKK();

    averager.Push((float)*adc);

    return BatADC_ToVoltage(averager.Value());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Hardware::Battery::GetVoltagePOW(uint *adc)
{
    static Utils::AroundAverager<float> averager(32);

    *adc = ReadValuePOW();

    averager.Push((float)*adc);

    return PowerADC_ToVoltage(averager.Value());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadVoltage()
{
    return HAL::ADC1_::ReadValue();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadValueAKK()
{
    HAL::ADC1_::SetActiveChannel2();

    return ReadVoltage();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint ReadValuePOW()
{
    HAL::ADC1_::SetActiveChannel9();

    return ReadVoltage();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float CalculatePercents(float volts)
{
    if (volts >= VOLTAGE_100_PERCENTS)
    {
        return 100.0F;
    }
    else if (volts > VOLTAGE_0_PERCENTS)
    {
        volts -= VOLTAGE_0_PERCENTS;

        return volts / (VOLTAGE_100_PERCENTS - VOLTAGE_0_PERCENTS) * 100.0F;
    }
    else
    {
        return 0.0F;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawBatteryUGO(int x, int y, float percents)
{
    int width = 38;

    Rectangle(width + 2, 8).Draw(x + 5, y, Color::FILL);
    Rectangle(4, 4).Draw(x + 1, y + 2);

    int filled = (int)((width - 2) * percents / 100.0F + 0.5F);

    Region(filled, 4).Fill(x + width - filled + 5, y + 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::Battery::Draw(int x, int y)
{
    Font::SetCurrent(Font::Type::_8);

    uint akkADC = 0;
    float akk = GetVoltageAKK(&akkADC);

    //uint powADC = 0;
    //float pow = GetVoltagePOW(&powADC);

    float percents = CalculatePercents(akk);

    Color::SetCurrent(Color::FILL);

    Font::SetCurrent(Font::Type::_5);

    Text(String("%1.2f В %4.1f%%", akk, percents)).Draw(x + 4, y - 1);

    Font::SetCurrent(Font::Type::_8);

    DrawBatteryUGO(x + 1, y + 10, percents);
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
