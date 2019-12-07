#pragma once


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

struct SettingsMultimeter
{
    Multimeter::AVP::E            avp;
    Multimeter::Measure::E        meas;
    Multimeter::RangeDC::E        rangeVoltageDC;
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


    struct PageCalibration
    {
        static const Page *const self;
    };
};
