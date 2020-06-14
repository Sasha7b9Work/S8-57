#include "defines.h"
#include "log.h"
#include "Display/Primitives.h"
#include "Hardware/Battery.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Averager.h"


static const float MAX_ADC_REL = static_cast<float>((1 << 12) - 1);     // Максимальное значение, которое возможно считать с АЦП
static const float MAX_ADC_ABS = 3.0F;                                  // Напряжение, соответствующее MAX_ADC_REL
const float Battery::SHUTDOWN_VOLTAGE = 5.4F;

// Возвращает true, если зарядное устройство подключено
static bool ChargerIsConnected();

// Перевод считанного значения ЦАП источника в вольты
static float ChargerADC_ToVoltage(uint value);

// Перевод считанного значения ЦАП батареи в вольты
static float BatteryADC_ToVoltage(float value);

// Рассчитать процент отставшегося заряда
static float CalculatePercents(float volts);

// Отобразить заряд батареи в графическом виде
static void DrawUGO(int x, int y, float procents);

// Значения, соответствующие 100% и 0%, для текущих напряжения аккумуляторов akk и источника заряда pow
static float Voltage100();
static float Voltage0();


void Battery::Init()
{
    HAL_ADC1::Init();
}


float Battery::GetVoltage()
{
    static Utils::AroundAverager<float> averager(32);

    uint akk = HAL_ADC1::ValueBattery();

    averager.Push(static_cast<float>(akk));

    return BatteryADC_ToVoltage(static_cast<float>(averager.Value()));
}


static float CalculatePercents(float volts)
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


static void DrawUGO(int x, int y, float percents)
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
    float akk = GetVoltage();

    float percents = CalculatePercents(akk);

    DrawUGO(x + 1, y + 9, percents);

    if(S_DBG_SHOW_BATTERY)
    {
        DFont::Set(DTypeFont::_8);

        Text(String("%4.2f В", akk)).Draw(x + 10, y, Color::FILL);
        Text(String("%4.1f %%", percents)).Draw(x + 10, y + 9);
    }
}


static float ChargerADC_ToVoltage(uint value)
{
    const float k = 124.0F / 24.0F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
}


static float BatteryADC_ToVoltage(float value)
{
#ifdef WIN32

    UNUSED_PARAMETER(value);

    return 8.0F;

#else

    const float k = 101.1F / 26.1F;

    return (value / MAX_ADC_REL) * MAX_ADC_ABS * k;
    
#endif
}


static float Voltage100()
{
    return ChargerIsConnected() ? 8.401F : 8.4F;
}


static float Voltage0()
{
    return ChargerIsConnected() ? Battery::SHUTDOWN_VOLTAGE : Battery::SHUTDOWN_VOLTAGE; //-V583
}


static bool ChargerIsConnected()
{
    uint pow = HAL_ADC1::ValueCharger();

    return ChargerADC_ToVoltage(pow) > 8.0F;
}
