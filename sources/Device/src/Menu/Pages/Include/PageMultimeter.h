#pragma once


struct SettingsMultimeter
{
    AVP::E                          avp;
    MultimeterMeasure::E            meas;
    RangeVoltageDC::E               rangeVoltageDC;
    RangeVoltageAC::E               rangeVoltageAC;
    Multimeter::RangeCurrentAC::E   rangeCurrentAC;     ///< Предел измерения переменного тока
    Multimeter::RangeCurrentDC::E   rangeCurrentDC;     ///< Предел измерения постоянного тока
    Multimeter::RangeResistance::E  rangeResist;
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    // Сюда поступают события клавиатуры для доступка к меню калибровки
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct Calibration
    {
        static const Page *const self;
    };
};
