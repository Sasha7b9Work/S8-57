#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Cursors_Settings.h"
#include "Osci/Measurements/Measures.h"
#include "PageDisplay.h"
#include "Multimeter/Multimeter.h"


#define MULTI_RANGE_VOLTAGE_AC  (set.mult.rangeAC)
#define MULTI_RANGE_VOLTAGE_DC  (set.mult.rangeDC)
#define MULTI_RANGE_CURRENT_AC  (set.mult.rangeCurrentAC)
#define MULTI_RANGE_CURRENT_DC  (set.mult.rangeCurrentDC)
#define MULTI_RANGE_RESISTANCE  (set.mult.rangeResist)
#define MULTI_AVP               (set.mult.avp)



struct FuncModeDraw
{
    enum E
    {
        Disable,
        Separate,
        Together
    } value;
    explicit FuncModeDraw(E v) : value(v) {};
};

struct ScaleFFT
{
    enum E
    {
        Log,           ///< Это значение означает логарифмическую шкалу вывода спектра.
        Linear         ///< Это значение означает линейную шкалу вывода спектра.
    } value;
    explicit ScaleFFT(E v) : value(v) {};
};

struct SourceFFT
{
    enum E
    {
        A,
        B,
        Both
    } value;
    explicit SourceFFT(E v) : value(v) {};
};

struct WindowFFT
{
    enum E
    {
        Rectangle,
        Hamming,
        Blackman,
        Hann
    } value;
    explicit WindowFFT(E v) : value(v) {};
};

struct MaxDBFFT
{
    enum E
    {
        _40,
        _60,
        _80
    } value;
    explicit MaxDBFFT(E v) : value(v) {};
    static float MaxDBforFFT(MaxDBFFT::E maxDB)
    {
        static const float arrayMAX_DB_FOR_FFT[] = { -40.0F, -60.0F, -80.0F };

        return arrayMAX_DB_FOR_FFT[static_cast<int>(maxDB)];
    }
};

struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    } value;
    explicit MathFunction(E v) : value(v) {};
};

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    } value;
    explicit ModeRegSet(E v) : value(v) {};
};

struct SettingsMath
{ //-V802
    FuncModeDraw::E modeDraw;       ///< Раздельный или общий дисплей в режиме математической функции.
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;     ///< Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    int8            divider;
    uint16          rShift;
    Range::E        range;
};

struct SettingsFFT
{
    bool            enabled;
    uint8           posCur[2];      ///< Позиция курсора спектра. Изменяется 0...256.
    ScaleFFT::E     scale;
    SourceFFT::E    source;
    WindowFFT::E    window;
    MaxDBFFT::E     maxDB;
    uint8           cursor;         ///< Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
};

struct SettingsCursors
{
    bool                                     showCursors;             ///< Показывать ли курсоры.
    Osci::Measurements::Cursors::LookMode::E lookMode[2];             ///< Режимы слежения за курсорами для двух пар курсоров.
    bool                                     showFreq;                ///< Установленное в true значение, что нужно показывать на экране 1/dT между курсорами.
    Osci::Measurements::Cursors::Active::E   active;                  ///< Какие курсоры сейас активны.
    Chan::E                                  source;                  ///< Источник - к какому каналу относятся курсоры.
    Osci::Measurements::Cursors::Control::E  cntrlU[Chan::Size];      ///< Активные курсоры напряжения.
    Osci::Measurements::Cursors::Control::E  cntrlT[Chan::Size];      ///< Активные курсоры времени.
    Osci::Measurements::Cursors::Movement::E movement;                ///< Как перемещаться курсорам - по точкам или процентам.
    float                                    deltaU100percents[2];    ///< Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float                                    deltaT100percents[2];    ///< Расстояние между курсорами времени для 100%, для обоих каналов.
    float                                    posCurU[Chan::Size][2];  ///< Текущие позиции курсоров напряжения обоих каналов.
    float                                    posCurT[Chan::Size][2];  ///< Текущие позиции курсоров времени обоих каналов.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct FreqMeterEnabled
{
    enum E
    {
        Off,
        On
    } value;
    explicit FreqMeterEnabled(E v) : value(v) {};
};

/// Что показывать - период или частоту
struct FreqMeterModeView
{
    enum E
    {
        Frequency,
        Period,
        Number
    } value;
    explicit FreqMeterModeView(E v) : value(v) {};
};

/// Время счёта периода.
struct FreqMeterTimeCounting
{
    enum E
    {
        _100ms,
        _1s,
        _10s
    } value;
    explicit FreqMeterTimeCounting(E v) : value(v) {};
};

/// Частота заполняющих импульсов для счёта частоты.
struct FreqMeterFreqClc
{
    enum E
    {
        _100kHz,
        _1MHz,
        _10MHz,
        _100MHz,
        Number
    } value;
    explicit FreqMeterFreqClc(E v) : value(v) {};
};

/// Количество периодов.
struct FreqMeterNumberPeriods
{
    enum E
    {
        _1,
        _10,
        _100,
        _1k,
        _10k,
        _100k,
        Number
    } value;
    explicit FreqMeterNumberPeriods(E v) : value(v) {};
};

struct SettingsFreqMeter
{
    FreqMeterEnabled::E        enabled;
    FreqMeterModeView::E       modeView;
    FreqMeterTimeCounting::E   timeCounting;  ///< Время счёта частоты.
    FreqMeterFreqClc::E        freqClc;       ///< Частота заполнения.
    FreqMeterNumberPeriods::E  numberPeriods; ///< Количество периодов.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Чем будем управлять в тестер-компоненте - напряжением или током
struct TesterControl
{
    enum E
    {
        Voltage,
        Current
    } value;
    explicit TesterControl(E v) : value(v) {};
};

struct TesterPolarity
{
    enum E
    {
        Positive,
        Negative
    } value;
    explicit TesterPolarity(E v) : value(v) {};
};

struct TesterStepU
{
    enum E
    {
        _100mV,
        _500mV
    } value;
    explicit TesterStepU(E v) : value(v) {};
};

struct TesterStepI
{
    enum E
    {
        _4mA,
        _20mA
    } value;
    explicit TesterStepI(E v) : value(v) {};
};

struct TesterViewMode
{
    enum E
    {
        Lines,
        Points
    } value;
    explicit TesterViewMode(E v) : value(v) {};
};

struct SettingsTester
{
    TesterControl::E    control;
    TesterPolarity::E   polarity;
    TesterStepU::E      stepU;
    TesterStepI::E      stepI;
    ENumAverage::E      ave;                   ///< Количество усреднений
    TesterViewMode::E   viewMode;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// АВП - автовыбор предела
struct MultimeterAVP
{
    enum E
    {
        Off,
        On
    } value;
    explicit MultimeterAVP(E v) : value(v) { };
};

/// Предел имзерения постоянного напряжения
struct MultimeterRangeDC
{
    enum E
    {
        _2V,
        _20V,
        _500V
    } value;
    explicit MultimeterRangeDC(E v) : value(v) { };
};

/// Предел измерения переменного напряжения
struct MultimeterRangeAC
{
    enum E
    {
        _2V,
        _20V,
        _400V
    } value;
    explicit MultimeterRangeAC(E v) : value(v) {};
};

/// Предел измерения тока
struct MultimeterRangeCurrent
{
    enum E
    {
        _20mA,
        _2A
    } value;
    explicit MultimeterRangeCurrent(E v) : value(v) {};
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
    } value;
    explicit MultimeterRangeResistance(E v) : value(v) {};
};

struct SettingsMultimeter
{
    MultimeterAVP::E              avp;
    Multimeter::Measure::E        meas;
    MultimeterRangeDC::E          rangeDC;
    MultimeterRangeAC::E          rangeAC;
    MultimeterRangeCurrent::E     rangeCurrentAC;     ///< Предел измерения переменного тока
    MultimeterRangeCurrent::E     rangeCurrentDC;     ///< Предел измерения постоянного тока
    MultimeterRangeResistance::E  rangeResist;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFunction
{
    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFrequencyCounter
{
    static const Page * const self;

    static void Init();

    /// \todo Здесь страшенный говнокод. Нельзя опускаться до персональных проверок

    /// Возвращает указатель на ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    static const Choice *GetChoiceTimeF();
    /// Возвращает указаетль на ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    static const Choice *GetChoiceFreqClc();
    /// Возвращает указатель на ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
    static const Choice *GetChoiceNumPeriods();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFFT
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCursors
    {
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMultimeter
{
    static void Init();

    static void OnChanged_Mode(bool);
    /// Сюда поступают события клавиатуры для доступка к меню калибровки
    static void DecodePassword(const KeyEvent &event);

    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCalibration
    {
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageTester
{
    static void Init();

    static void OnChanged_Control(bool);

    static const Page * const self;
};
