#include "defines.h"
#include "log.h"
#include "Display/Primitives.h"
#include "Hardware/Battery.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Averager.h"


// Максимальное значение, которое возможно считать с АЦП
const float Battery::MAX_ADC_REL = static_cast<float>((1 << 12) - 1);
// Напряжение, соответствующее MAX_ADC_REL
const float Battery::MAX_ADC_ABS = 3.0F;


const float Battery::MIN_ABS = 5.4F;


void Battery::Init()
{
    HAL_ADC1::Init();
}


float Battery::GetVoltageAKK()
{
    static Utils::AroundAverager<float> averager(32);

    uint akk = HAL_ADC1::ReadValueAKK();

    averager.Push(static_cast<float>(akk));

#ifdef WIN32
    return 8.0F;
#else
    return BatADC_ToVoltage(static_cast<float>(averager.Value()));
#endif
}


float Battery::CalculatePercents(float volts)
{
    if (volts >= Voltage100())
    {
        return 100.0F;
    }
    else if (volts > Voltage0())
    {
        volts -= Voltage0();

        return volts / (Voltage100() - Voltage0()) * 100.0F;
    }
    else
    {
        return 0.0F;
    }
}


void Battery::DrawUGO(int x, int y, float percents)
{
    int widthBig = 30;
    int widthSmall = 4;

    int dY = 5;

    Rectangle(widthBig + 2, 8 + dY).Draw(x + widthSmall + 1, y - dY, Color::BATTERY);
    Rectangle(widthSmall, 4 + dY).Draw(x + 1, y + 2 - dY);

    int filled = static_cast<int>((widthBig - 2) * percents / 100.0F + 0.5F);

    Region(filled, 4 + dY).Fill(x + widthBig - filled + widthSmall + 1, y + 2 - dY, Color::BATTERY);


}


void Battery::Draw(int x, int y)
{
    float akk = GetVoltageAKK();

    float percents = CalculatePercents(akk);

    DrawUGO(x + 1, y + 9, percents);

    if(S_DBG_SHOW_BATTERY)
    {
        DFont::Set(DTypeFont::_8);

        Text(String("%4.2f В", akk)).Draw(x + 10, y, Color::FILL);
        Text(String("%4.1f %%", percents)).Draw(x + 10, y + 9);
    }
}


float Battery::PowerADC_ToVoltage(uint value)
{
    const float k = 124.0F / 24.0F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}


float Battery::BatADC_ToVoltage(float value)
{
    const float k = 101.1F / 26.1F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}


float Battery::Voltage100()
{
    return ChargerIsConnected() ? 8.401F : 8.4F;
}


float Battery::Voltage0()
{
    return ChargerIsConnected() ? MIN_ABS : MIN_ABS; //-V583
}


bool Battery::ChargerIsConnected()
{
    uint pow = HAL_ADC1::ReadValueCharger();

    return PowerADC_ToVoltage(pow) > 8.0F;
}
