#pragma once


#define S_MULT_SHOW_ALWAYS  (set.mult._showAlways)


struct SettingsMultimeter
{
    ModeAVP::E              _avp;
    MultimeterMeasure::E    _meas;
    RangeVoltageDC::E       _rangeVoltageDC;
    RangeVoltageAC::E       _rangeVoltageAC;
    RangeCurrentAC::E       _rangeCurrentAC;     // Предел измерения переменного тока
    RangeCurrentDC::E       _rangeCurrentDC;     // Предел измерения постоянного тока
    RangeResistance::E      _rangeResist;
    bool                    _showAlways;         // true - показывать в режиме осциллографа тоже
};



struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool = true);

    // Возвращает true, если "Нуль" - "Вкл"
    static bool ZeroEnabled();
    
    // Сюда поступают события клавиатуры для доступка к меню калибровки
    static void DecodePassword(const KeyEvent &event);

    static const Page *const self;


    struct Calibration
    {
        static const Page *const self;
    };
};
