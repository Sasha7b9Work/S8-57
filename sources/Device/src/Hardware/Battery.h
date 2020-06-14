#pragma once


class Battery
{
public:

    static const float MIN_ABS;

    // Инициализация
    static void Init();

    static void Draw(int x, int y);

    // Получить значение напряжения батареи
    static float GetVoltageAKK();

private:   
    
    // Получить значение напряжения источника зарядного напряжения
    static float GetVoltageCharger();

    // Возвращает true, если зарядное устройство подключено
    static bool ChargerIsConnected();
    
    // Перевод считанного значения ЦАП источника в вольты
    static float PowerADC_ToVoltage(uint value);
    
    // Перевод считанного значения ЦАП батареи в вольты
    static float BatADC_ToVoltage(float value);
    
    // Рассчитать процент отставшегося заряда
    static float CalculatePercents(float volts);
    
    // Отобразить заряд батареи в графическом виде
    static void DrawUGO(int x, int y, float procents);

    // Значения, соответствующие 100% и 0%, для текущих напряжения аккумуляторов akk и источника заряда pow
    static float Voltage100();
    static float Voltage0();

    // Максимальное значение, которое возможно считать с АЦП
    static const float MAX_ADC_REL;
    
    // Напряжение, соответствующее MAX_ADC_REL
    static const float MAX_ADC_ABS;
};
