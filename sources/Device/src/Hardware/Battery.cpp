#include "defines.h"
#include "Display/Primitives.h"
#include "Hardware/Battery.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Averager.h"


/// Максимальное значение, которое возможно считать с АЦП
const float Battery::MAX_ADC_REL = static_cast<float>((1 << 12) - 1);
/// Напряжение, соответствующее MAX_ADC_REL
const float Battery::MAX_ADC_ABS = 2.91F;

const float Battery::VOLTAGE_100_PERCENTS = 8.2F;

const float Battery::VOLTAGE_0_PERCENTS = 6.0F;


void Battery::Init()
{
    HAL_ADC1::Init();
}


float Battery::GetVoltageAKK(uint *adc)
{
    static Utils::AroundAverager<float> averager(32);

    *adc = HAL_ADC1::ReadValueAKK();

    averager.Push(static_cast<float>(*adc));

    return BatADC_ToVoltage(static_cast<float>(*adc));
}


float Battery::GetVoltagePOW(uint *adc)
{
    static Utils::AroundAverager<float> averager(32);

    *adc = HAL_ADC1::ReadValuePOW();

    averager.Push(static_cast<float>(*adc));

    return PowerADC_ToVoltage(averager.Value());
}


float Battery::CalculatePercents(float volts)
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


void Battery::DrawUGO(int x, int y, float percents)
{
    int width = 38;

    int dY = 5;

    Rectangle(width + 2, 8 + dY).Draw(x + 5, y - dY, Color::BATTERY);
    Rectangle(4, 4 + dY).Draw(x + 1, y + 2 - dY);

    int filled = static_cast<int>((width - 2) * percents / 100.0F + 0.5F);

    Region(filled, 4 + dY).Fill(x + width - filled + 5, y + 2 - dY, Color::BATTERY);
}


void Battery::Draw(int x, int y)
{
    uint akkADC = 0;
    float akk = GetVoltageAKK(&akkADC);

    //uint powADC = 0;
    //float pow = GetVoltagePOW(&powADC);

    float percents = CalculatePercents(akk);

    //Text(String("%1.2f В", akk)).Draw(x + 8, y, Color::FILL);
    //Text("Test").Draw(x + 8, y, Color::FILL);

    DrawUGO(x + 1, y + 9, percents);

    if(set.dbg.showBattery)
    {
        Font::Set(TypeFont::_8);

        Text(String("%4.1f В", akk)).Draw(x + 10, y, Color::FILL);
        Text(String("%4.1f %%", percents)).Draw(x + 10, y + 9);
    }
}


float Battery::PowerADC_ToVoltage(float value)
{
    const float k = 124.0F / 24.0F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}


float Battery::BatADC_ToVoltage(float value)
{
    const float k = 101.1F / 26.1F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}
