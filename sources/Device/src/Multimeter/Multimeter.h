#pragma once


struct DisplayMultimeter
{
    static void Update();
    
    // Через эту функцию поступает измерение от прибора
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
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
    static char Symbol();

    // Получить код измерения из принятого буфера
    static MultimeterMeasure::E GetCode(const char buffer[13]);
};

// АВП - автовыбор предела
struct ModeAVP
{
    enum E
    {
        Off,
        On
    };
};


// Предел имзерения постоянного напряжения
struct RangeVoltageDC
{
    enum E
    {
        _2V,
        _20V,
        _500V
    };
};


// Предел измерения переменного напряжения
struct RangeVoltageAC
{
    enum E
    {
        _2V,
        _20V,
        _400V
    };
};


// Предел измерения переменного тока
struct RangeCurrentAC
{
    enum E
    {
        _20mA,
        _2A
    };
};


// Предел измерения постоянного тока
struct RangeCurrentDC
{
    enum E
    {
        _20mA,
        _2A
    };
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
};
