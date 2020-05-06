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
#define S_MULT_MEASURE                  (set.mult._meas)
#define S_MULT_MEASURE_IS_RESISTANCE    (S_MULT_MEASURE == MultimeterMeasure::Resistance)
#define S_MULT_MEASURE_IS_TEST_DIODE    (S_MULT_MEASURE == MultimeterMeasure::TestDiode)
#define S_MULT_MEASURE_IS_BELL          (S_MULT_MEASURE == MultimeterMeasure::Bell)

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
#define S_MULT_MODE_AVP     (set.mult._avp)
#define S_MULT_AVP_DISABLED (S_MULT_MODE_AVP == ModeAVP::Off)
#define S_MULT_AVP_ENABLED  (S_MULT_MODE_AVP == ModeAVP::On)

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
