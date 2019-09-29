#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    class MultimeterWorker;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Multimeter
{
    class DisplayWorker;

    /// Инициализация
    void Init();

    void DeInit();

    void Update();
    /// Сообщает мультиметру, что произошло изменение режима и нужно дождаться результата измерения перед выводом
    void ChangeMode();

    void Calibrate(int calibr);

    void ChangeAVP();
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
            Size
        } value;
        explicit Measure(E v) : value(v) { };
        char Symbol() const
        {
            static const char symbols[Size] = {'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
            return symbols[value]; //-V2006
        }
        /// Получить код измерения из принятого буфера
        static Measure::E GetCode(const char buffer[13]);
    };

    /// Используется для отрисовки
    class Display
    {
    friend class DisplayWorker;
    friend class ::Display::MultimeterWorker;
    public:
        static void Update();
        /// Через эту функцию поступает измерение от прибора
        static void SetMeasure(const uint8 buffer[13]);

        static void ChangedMode();
    };
};
