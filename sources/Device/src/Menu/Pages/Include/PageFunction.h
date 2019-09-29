#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Cursors_Settings.h"


#define SOURCE_FFT                  (set.math.sourceFFT)
#define SOURCE_FFT_IS_A             (SOURCE_FFT == SourceFFT::A)
#define SOURCE_FFT_IS_B             (SOURCE_FFT == SourceFFT::B)

#define WINDOW_FFT                  (set.math.windowFFT)
#define WINDOW_FFT_IS_HAMMING       (WINDOW_FFT == WindowFFT::Hamming)
#define WINDOW_FFT_IS_BLACKMAN      (WINDOW_FFT == WindowFFT::Blackman)
#define WINDOW_FFT_IS_HANN          (WINDOW_FFT == WindowFFT::Hann)
#define MAX_DB_FFT                  (set.math.FFTmaxDB)
#define MAX_DB_FOR_FFT              FFTmaxDB::MaxDBforFFT(MAX_DB_FFT)
#define FFT_ENABLED                 (set.math.enableFFT)
#define MATH_ENABLED                (FUNC_MODE_DRAW_IS_ENABLED || FFT_ENABLED)

#define SCALE_FFT                   (set.math.scaleFFT)
#define SCALE_FFT_IS_LOG            (SCALE_FFT == ScaleFFT::Log)
#define SCALE_FFT_IS_LINEAR         (SCALE_FFT == ScaleFFT::Linear)

#define MATH_FUNC                   (set.math.function)

#define FUNC_MODE_DRAW              (set.math.modeDraw)
#define FUNC_MODE_DRAW_IS_ENABLED   (FUNC_MODE_DRAW != FuncModeDraw::Disable)
#define FUNC_MODE_DRAW_IS_SEPARATE  (FUNC_MODE_DRAW == FuncModeDraw::Separate)
#define FUNC_MODE_DRAW_IS_TOGETHER  (FUNC_MODE_DRAW == FuncModeDraw::Together)

#define FFT_POS_CURSOR(num)         (set.math.posCur[num])
#define FFT_POS_CURSOR_0            (FFT_POS_CURSOR(0))
#define FFT_POS_CURSOR_1            (FFT_POS_CURSOR(1))

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
    FuncModeDraw::E          modeDraw;             ///< Раздельный или общий дисплей в режиме математической функции.
    bool                     enableFFT;
    ScaleFFT::E              scaleFFT;
    SourceFFT::E             sourceFFT;
    WindowFFT::E             windowFFT;
    uint8                    currentCursor;        ///< Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
    uint8                    posCur[2];            ///< Позиция курсора спектра. Изменяется 0...256.
    FFTmaxDB::E              FFTmaxDB;
    MathFunction::E          function;
    ModeRegSet::E            modeRegSet;           ///< Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Osci::Settings::Range::E range;
    int8                     divider;
    uint16                   rShift;
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageRecorder
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSource
    {
        static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageShow
    {
        static const Page * const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageOperations
        {
            static const Page * const self;
        };

        struct PageView
        {
            static const Page * const self;

            //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            struct PageCursors
            {
                static const Page * const self;
            };
        };
    };
};
