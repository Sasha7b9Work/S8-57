#pragma once



/// АВП - автовыбор предела
struct MultimeterAVP
{
    enum E
    {
        Off,
        On
    };
};

/// Предел имзерения постоянного напряжения
struct MultimeterRangeDC
{
    enum E
    {
        _2V,
        _20V,
        _500V
    };
};

/// Предел измерения переменного напряжения
struct MultimeterRangeAC
{
    enum E
    {
        _2V,
        _20V,
        _400V
    };
};

/// Предел измерения тока
struct MultimeterRangeCurrent
{
    enum E
    {
        _20mA,
        _2A
    };
};

/// Предел измерения сопротивленя постоянному току
struct MultimeterRangeResistance
{
    enum E
    {
        _2k,
        _20k,
        _200k,
        _10M
    };
};

/// Режим измерений мультиметра
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
        Size
    } value;
    explicit MultimeterMeasure(E v) : value(v) { };
    char Symbol() const
    {
        static const char symbols[Size] = { 'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
        return symbols[value]; //-V2006
    }
    /// Получить код измерения из принятого буфера
    static MultimeterMeasure::E GetCode(const char buffer[13]);
};

struct SettingsMultimeter
{
    MultimeterAVP::E              avp;
    MultimeterMeasure::E          meas;
    MultimeterRangeDC::E          rangeVoltageDC;
    MultimeterRangeAC::E          rangeVoltageAC;
    MultimeterRangeCurrent::E     rangeCurrentAC;     ///< Предел измерения переменного тока
    MultimeterRangeCurrent::E     rangeCurrentDC;     ///< Предел измерения постоянного тока
    MultimeterRangeResistance::E  rangeResist;
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    /// Сюда поступают события клавиатуры для доступка к меню калибровки
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCalibration
    {
        static const Page *const self;
    };
};
