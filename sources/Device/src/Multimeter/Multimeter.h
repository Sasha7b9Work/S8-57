#pragma once
#include <stm32f4xx_hal.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MULTI_MEASURE           (set.multi_meas)
#define MULTI_RANGE_AC          (set.multi_rangeAC)
#define MULTI_RANGE_DC          (set.multi_rangeDC)
#define MULTI_RANGE_RESISTANCE  (set.multi_rangeResist)
#define MULTI_AVP               (set.multi_avp)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Multimeter
{
public:

    /// Используется для отрисовки
    class Graphics
    {
    public:
        static void Update();
    private:
        static void PrepareConstantVoltage();
        static void PrepareVariableVoltage();
        static void PrepareConstantCurrent();
        static void PrepareVariableCurrent();
        static void PrepareResistance();
        static void PrepareTestDiode();
        static void PrepareRing();
    };
    /// Инициализация
    static void Init();

    static void Update();
    /// Сообщает мультиметру, что произошло изменение режима и нужно дождаться результата измерения перед выводом
    static void ChangeMode();

    static void ChangeAVP();
    /// Через эту функцию поступает измерение от прибора
    static void SetMeasure(uint8 buffer[10]);

    static UART_HandleTypeDef handlerUART;

    /// Режим измерений мультиметра
    struct Measure
    {
        enum E
        {
            VoltageDC,
            VoltageAC,
            CurrentDC,
            CurrentAC,
            Resistance,
            TestDiode,
            Bell,
            Number
        } value;
        Measure(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
        char Symbol() const
        {
            static const char symbols[Number] = {'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
            return symbols[value];
        }
        static Measure ForSymbol(char symbol)
        {
            switch(symbol)
            {
                case 'V':   return Measure::VoltageAC;
                case 'I':   return Measure::CurrentDC;  
                case 'J':   return Measure::CurrentAC;  
                case 'R':   return Measure::Resistance; 
                case 'Y':   return Measure::TestDiode;  
                case 'W':   return Measure::Bell;       
            }
            return Measure::Number;
        }
    };

    /// АВП - автовыбор предела
    struct AVP
    {
        enum E
        {
            Off,
            On
        } value;
        AVP(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
    };

    /// Предел имзерения постоянного напряжения
    struct RangeDC
    {
        enum E
        {
            _2V,
            _20V,
            _500V
        } value;
        RangeDC(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
    };

    /// Предел измерения переменного напряжения
    struct RangeAC
    {
        enum E
        {
            _2V,
            _20V,
            _400V
        } value;
        RangeAC(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
    };

    /// Предел измерения сопротивленя постоянному току
    struct RangeResistance
    {
        enum E
        {
            _2k,
            _20k,
            _200k,
            _10M
        } value;
        RangeResistance(E v) : value(v) { };
        operator uint8() const  { return (uint8)value; };
    };

private:
    /// Если нулевой элемент == 0, то выводить ничего не нужно
    static char         buffer[11];
};
