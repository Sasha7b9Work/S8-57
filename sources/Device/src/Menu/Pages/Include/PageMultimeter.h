#pragma once


struct SettingsMultimeter
{
    ModeAVP::E              avp;
    MultimeterMeasure::E    meas;
    RangeVoltageDC::E       rangeVoltageDC;
    RangeVoltageAC::E       rangeVoltageAC;
    RangeCurrentAC::E       rangeCurrentAC;     // Предел измерения переменного тока
    RangeCurrentDC::E       rangeCurrentDC;     // Предел измерения постоянного тока
    RangeResistance::E      rangeResist;
    bool                    showAlways;         // true - показывать в режиме осциллографа тоже
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
