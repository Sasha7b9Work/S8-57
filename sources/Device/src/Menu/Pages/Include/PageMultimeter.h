#pragma once


/// ѕредел измерени€ сопротивлен€ посто€нному току
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
    Multimeter::RangeAC::E        rangeVoltageAC;
    Multimeter::RangeCurrentAC::E rangeCurrentAC;     ///< ѕредел измерени€ переменного тока
    Multimeter::RangeCurrentDC::E rangeCurrentDC;     ///< ѕредел измерени€ посто€нного тока
    MultimeterRangeResistance::E  rangeResist;
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    /// —юда поступают событи€ клавиатуры дл€ доступка к меню калибровки
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct PageCalibration
    {
        static const Page *const self;
    };
};
