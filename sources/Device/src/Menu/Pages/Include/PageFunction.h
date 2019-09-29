#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Cursors_Settings.h"
#include "Osci/Measurements/Measures.h"
#include "PageDisplay.h"
#include "Multimeter/Multimeter.h"


#define WINDOW_FFT                  (set.math.windowFFT)
#define WINDOW_FFT_IS_HAMMING       (WINDOW_FFT == WindowFFT::Hamming)
#define WINDOW_FFT_IS_BLACKMAN      (WINDOW_FFT == WindowFFT::Blackman)
#define WINDOW_FFT_IS_HANN          (WINDOW_FFT == WindowFFT::Hann)
#define MAX_DB_FFT                  (set.math.FFTmaxDB)
#define MAX_DB_FOR_FFT              FFTmaxDB::MaxDBforFFT(MAX_DB_FFT)
#define FFT_ENABLED                 (set.math.enableFFT)
#define MATH_ENABLED                (FUNC_MODE_DRAW_IS_ENABLED || FFT_ENABLED)

#define MATH_CURRENT_CUR            (set.math.currentCursor)
#define MATH_CURRENT_CUR_IS_0       (MATH_CURRENT_CUR == 0)

#define MATH_DIVIDER                (set.math.divider)

#define MATH_MODE_REG_SET           (set.math.modeRegSet)
#define MATH_MODE_REG_SET_IS_RSHIFT (MATH_MODE_REG_SET == ModeRegSet::RShift)
#define MATH_MODE_REG_SET_IS_RANGE  (MATH_MODE_REG_SET == ModeRegSet::Range)

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

struct FFTmaxDB
{
    enum E
    {
        _40,
        _60,
        _80
    } value;
    explicit FFTmaxDB(E v) : value(v) {};
    static float MaxDBforFFT(FFTmaxDB::E maxDB)
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
{
    FuncModeDraw::E modeDraw;             ///< Раздельный или общий дисплей в режиме математической функции.
    bool            enableFFT;
    ScaleFFT::E     scaleFFT;
    SourceFFT::E    sourceFFT;
    WindowFFT::E    windowFFT;
    uint8           currentCursor;        ///< Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
    FFTmaxDB::E     FFTmaxDB;
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;           ///< Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    int8            divider;
    uint16          rShift;
    Range::E        range;
};

struct SettingsFFT
{
    uint8           posCur[2];            ///< Позиция курсора спектра. Изменяется 0...256.
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
/// Сколько автоматических измерений помещается на экран
struct MeasuresOnDisplay
{
    enum E
    {
        _1,      ///< 1 измерение слева внизу.
        _2,      ///< 2 измерения слева внизу.
        _1_5,    ///< 1 строка с 5 измерениями.
        _2_5,    ///< 2 строки по 5 измерений.
        _3_5,    ///< 3 строки по 5 измерений.
        _6_1,    ///< 6 строк по 1 измерению.
        _6_2     ///< 6 строк по 2 измерения.
    } value;
    explicit MeasuresOnDisplay(E v) : value(v) {};
};

struct MeasuresSource
{
    enum E
    {
        A,
        B,
        A_B
    } value;
    explicit MeasuresSource(E v) : value(v) {};
};

/// Сжимать ли сигналы при выводе измерений.
struct MeasuresModeViewSignals
{
    enum E
    {
        AsIs,       ///< Показывать сигналы как есть.
        Compress    ///< Сжимать сетку с сигналами.
    } value;
    explicit MeasuresModeViewSignals(E v) : value(v) {};
};

#define NUM_MEASURES                    (set.meas.number)
#define NUM_MEASURES_IS_1_5             (NUM_MEASURES == MeasuresOnDisplay::_1_5)
#define NUM_MEASURES_IS_2_5             (NUM_MEASURES == MeasuresOnDisplay::_2_5)
#define NUM_MEASURES_IS_3_5             (NUM_MEASURES == MeasuresOnDisplay::_3_5)
#define NUM_MEASURES_IS_6_1             (NUM_MEASURES == MeasuresOnDisplay::_6_1)
#define NUM_MEASURES_IS_6_2             (NUM_MEASURES == MeasuresOnDisplay::_6_2)

/// Сжимать ли сетку при выводе измерений
#define MODE_VIEW_SIGNALS               (set.meas.modeViewSignals) 
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == MeasuresModeViewSignals::Compress)

/// По какому каналу производить автоматические измерения
#define SOURCE_MEASURES                 (set.meas.source)
/// Автоматические измерения производятся только по каналу A
#define SOURCE_MEASURES_IS_A            (SOURCE_MEASURES == MeasuresSource::A)
/// Автоматические измерения производятся только по каналу B
#define SOURCE_MEASURES_IS_B            (SOURCE_MEASURES == MeasuresSource::B)
/// Автоматические измерения производятся по каналам A и B
#define SOURCE_MEASURES_IS_BOTH         (SOURCE_MEASURES == MeasuresSource::A_B)
/// Выводить автоматические измерения по каналу A
#define VIEW_MEASURES_A                 (set.ch[Chan::A].enabled && (SOURCE_MEASURES_IS_A || SOURCE_MEASURES_IS_BOTH))
/// Выводить автоматические измерения по каналу B
#define VIEW_MEASURES_B                 (set.ch[Chan::B].enabled && (SOURCE_MEASURES_IS_B || SOURCE_MEASURES_IS_BOTH))
/// Выводить автоматические измерения по обоим каналам
#define VIEW_MEASURES_BOTH              (SET_ENABLED_BOTH && SOURCE_MEASURES_IS_BOTH)

#define SHOW_MEASURES                   (set.meas.show)

#define MEAS_MARKED                     (set.meas.marked)

struct SettingsMeasures
{
    bool                        show;              ///< Показывать ли измерения.
    MeasuresOnDisplay::E        number;            ///< Сколько измерений выводить.
    MeasuresSource::E           source;            ///< Для каких каналов выводить измерения.
    MeasuresModeViewSignals::E  modeViewSignals;   ///< Сжимать ли сигналы при выводе измерений.
    Measure::Type::E            measures[15];      ///< Выбранные для индикации измерения.
    Measure::Type::E            marked;            ///< Измерение, на которое нужно выводить маркеры.
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

#define FREQ_METER_TIMECOUNTING             (set.freq.timeCounting)
#define FREQ_METER_FREQ_CLC                 (set.freq.freqClc)
#define FREQ_METER_NUM_PERIODS              (set.freq.numberPeriods)

#define FREQ_METER_ENABLED                  (set.freq.enabled)
#define FREQ_METER_IS_ENABLED               (FREQ_METER_ENABLED == FreqMeterEnabled::On)

#define FREQ_METER_MODE_VIEW                (set.freq.modeView)
#define FREQ_METER_MODE_VIEW_IS_PERIOD      (FREQ_METER_MODE_VIEW == FreqMeterModeView::Period)
#define FREQ_METER_MODE_VIEW_IS_FREQUENCY   (FREQ_METER_MODE_VIEW == FreqMeterModeView::Frequency)

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

#define TESTER_STEP_U                   (set.test.stepU)
#define TESTER_STEP_I                   (set.test.stepI)
#define TESTER_VIEW_MODE                (set.test.viewMode)
#define TESTER_ENUM_AVERAGE             (set.test.ave)

#define TESTER_POLARITY                 (set.test.polarity)
#define TESTER_POLARITY_IS_POSITITVE    (TESTER_POLARITY == TesterPolarity::Positive)

#define TESTER_CONTROL                  (set.test.control)
#define TESTER_CONTROL_IS_U             (TESTER_CONTROL == TesterControl::Voltage)


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

#define MULTI_MEASURE           (set.mult.meas)
#define MULTI_RANGE_VOLTAGE_AC  (set.mult.rangeAC)
#define MULTI_RANGE_VOLTAGE_DC  (set.mult.rangeDC)
#define MULTI_RANGE_CURRENT_AC  (set.mult.rangeCurrentAC)
#define MULTI_RANGE_CURRENT_DC  (set.mult.rangeCurrentDC)
#define MULTI_RANGE_RESISTANCE  (set.mult.rangeResist)
#define MULTI_AVP               (set.mult.avp)

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
