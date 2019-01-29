#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MULTI_MEASURE           (set.multi_meas)
#define MULTI_RANGE_AC          (set.multi_rangeAC)
#define MULTI_RANGE_DC          (set.multi_rangeDC)
#define MULTI_RANGE_RESISTANCE  (set.multi_rangeResist)
#define MULTI_AVP               (set.multi_avp)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Multimeter
{
    /// Используется для отрисовки
    namespace Display
    {
        void Update();
    };
    /// Инициализация
    void Init();

    void DeInit();

    void Update();
    /// Сообщает мультиметру, что произошло изменение режима и нужно дождаться результата измерения перед выводом
    void ChangeMode();

    void ChangeAVP();
    /// Через эту функцию поступает измерение от прибора
    static void SetMeasure(const uint8 buffer[10]);
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
        explicit Measure(E v) : value(v) { };
        char Symbol() const
        {
            static const char symbols[Number] = {'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
            return symbols[value]; //-V2006
        }
        static Measure::E ForSymbol(char symbol);
    };

    /// Если нулевой элемент == 0, то выводить ничего не нужно
    extern char         buffer[11];
};
