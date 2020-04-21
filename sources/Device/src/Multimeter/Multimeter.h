#pragma once


struct DisplayMultimeter
{
    static void Update();
    // Через эту функцию поступает измерение от прибора
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
};


// Режим измерений мультиметра
struct MultimeterMeasure
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
        Count
    };
    MultimeterMeasure()
    {
    }
    static MultimeterMeasure::E &Ref();
    operator MultimeterMeasure::E()
    {
        return Ref();
    }
    static char Symbol()
    {
        static const char symbols[Count] = { 'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
        return symbols[Ref()]; //-V2006
    }
    // Получить код измерения из принятого буфера
    static MultimeterMeasure::E GetCode(const char buffer[13]);
    static bool IsVoltageDC()
    {
        return Ref() == VoltageDC;
    }
    static bool IsVoltageAC()
    {
        return Ref() == VoltageAC;
    }
    static bool IsResistance()
    {
        return Ref() == Resistance;
    }
    static bool IsCurrentDC()
    {
        return Ref() == CurrentDC;
    }
};


struct Multimeter
{
    // Инициализация
    static void Init();

    static void DeInit();

    static void Update();
    // Сообщает мультиметру, что произошло изменение режима и нужно дождаться результата измерения перед выводом
    static void ChangeMode();

    static void Calibrate(int calibr);

    static void ChangeAVP();
    // Если (zero != 0) - заслать режим нуля
    static void LoadZero(int zero);

    // АВП - автовыбор предела
    struct AVP
    {
        enum E
        {
            Off,
            On
        };

        static AVP::E &Ref();
        static bool IsEnabled() { return Ref() == On; }
    };

    // Предел имзерения постоянного напряжения
    struct RangeDC
    {
        enum E
        {
            _2V,
            _20V,
            _500V
        };
        static RangeDC::E &Ref();
        operator RangeDC::E() { return Ref(); }
    };

    // Предел измерения переменного напряжения
    struct RangeAC
    {
        enum E
        {
            _2V,
            _20V,
            _400V
        };
        static RangeAC::E &Ref();
        operator RangeAC::E() { return Ref(); }
    };

    // Предел измерения переменного тока
    struct RangeCurrentAC
    {
        enum E
        {
            _20mA,
            _2A
        };
        static RangeCurrentAC::E &Ref();
        operator RangeCurrentAC::E() { return Ref(); }
    };

    // Предел измерения постоянного тока
    struct RangeCurrentDC
    {
        enum E
        {
            _20mA,
            _2A
        };
        static RangeCurrentDC::E &Ref();
        operator RangeCurrentDC::E() { return Ref(); }
    };

    // Предел измерения сопротивленя постоянному току
    struct RangeResistance
    {
        enum E
        {
            _2k,
            _20k,
            _200k,
            _10M
        };
        static RangeResistance::E &Ref();
        operator RangeResistance::E() { return Ref(); }
    };
};
