#include "defines.h"
#include "log.h"
#include "Display/Primitives.h"
#include "Hardware/Battery.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Averager.h"


static const float MAX_ADC_REL = static_cast<float>((1 << 12) - 1);     // Максимальное значение, которое возможно считать с АЦП
static const float MAX_ADC_ABS = 3.0F;                                  // Напряжение, соответствующее MAX_ADC_REL
const float Battery::SHUTDOWN_VOLTAGE = 5.4F;

static bool isBusy = false;

static Utils::AroundAverager<float> averager(32);

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

static void DrawFilled(int x, int y, int full, float percents);

// Значения, соответствующие 100% и 0%, для текущих напряжения аккумуляторов akk и источника заряда pow
static float Voltage100();
static float Voltage0();

void Battery::Init()
{
    HAL_ADC1::Init();
}


bool Battery::IsBusy()
{
    return isBusy;
}


float Battery::GetVoltage()
{
    isBusy = true;

    uint akk = HAL_ADC1::ValueBattery();

    averager.Push(static_cast<float>(akk));

    isBusy = false;

    return BatteryADC_ToVoltage(static_cast<float>(averager.Value()));
}


float Battery::GetVoltageAverage()
{
    return BatteryADC_ToVoltage(static_cast<float>(averager.Value()));
}


static float CalculatePercents(float volts) //-V2506
{
    if (volts >= Voltage100())
    {
        return 100.0F;
    }
    else if (volts <= Voltage0()) //-V2516
    {
        return 0.0F;
    }

    volts -= Voltage0();

    return volts / (Voltage100() - Voltage0()) * 100.0F;
}


static void DrawUGO(int x, int y, float percents)
{
    int widthFull = 43;
    int widthSmall = 6;

    int dY = 5;

    Rectangle(widthFull - widthSmall, 8 + dY).Draw(x + widthSmall + 1, y - dY, percents <= 25.0F ? Color::RED : Color::BATTERY);
    Rectangle(widthSmall, 4 + dY).Draw(x + 1, y + 2 - dY);

    DrawFilled(x + widthSmall + 3, y - 3, widthFull - widthSmall - 4, percents);
}


static void DrawFilled(int x, int y, int full, float percents)
{
    int filled = static_cast<int>(full * percents / 100.0F + 0.5F);

    //if (ChargerIsConnected() && percents < 100.0F)
    //{
    //    int onePart = 1000 / full;
    //
    //    int time = static_cast<int>(TIME_MS % 1000);
    //
    //    int area = time / onePart;
    //
    //    Region(area, 9).Fill(x + full - area, y, Color::BATTERY_EMPTY);
    //
    //    if (filled > area)
    //    {
    //        filled = area;
    //    }
    //}

    Region(filled, 9).Fill(x + full - filled, y, percents <= 25.0F ? Color::RED : Color::BATTERY);
}


void Battery::Draw(int x, int y)
{
    float akk = GetVoltage();

    float percents = CalculatePercents(akk);

    DrawUGO(x + 1, y + 8, percents);

    if(S_DBG_SHOW_BATTERY)
    {
        DFont::Set(DTypeFont::_8);

        Text(String("%4.2f В", akk)).Draw(x + 10, y, Color::FILL);
        Text(String("%4.1f %%", percents)).Draw(x + 10, y + 9);
    }
}


static float ChargerADC_ToVoltage(uint value)
{
    float result = (value / MAX_ADC_REL) * MAX_ADC_ABS;

    result *= 124.0F / 24.0F;

    return result;
}


static float BatteryADC_ToVoltage(float value)
{
    float result = (value / MAX_ADC_REL) * MAX_ADC_ABS;

    result *= 101.1F / 26.1F;

    return result;
}


static float Voltage100()
{
    return ChargerIsConnected() ? 8.301F : 8.3F;
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
