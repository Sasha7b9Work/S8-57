#pragma once


class Battery
{
public:
    /// Инициализация
    static void Init();

    static void Draw(int x, int y);
    /// Получить значение напряжения батареи
    static float GetVoltageAKK(uint *adc);
    /// Получить значение напряжения источника зарядного напряжения
    static float GetVoltagePOW(uint *adc);
private:
    /// Перевод считанного значения ЦАП источника в вольты
    static float PowerADC_ToVoltage(float value);
    /// Перевод считанного значения ЦАП батареи в вольты
    static float BatADC_ToVoltage(float value);
    /// Рассчитать процент отставшегося заряда
    static float CalculatePercents(float volts);
    /// Отобразить заряд батареи в графическом виде
    static void DrawUGO(int x, int y, float procents);

    /// Максимальное значение, которое возможно считать с АЦП
    static const float MAX_ADC_REL;
    /// Напряжение, соответствующее MAX_ADC_REL
    static const float MAX_ADC_ABS;

    static const float VOLTAGE_100_PERCENTS;

    static const float VOLTAGE_0_PERCENTS;
};
