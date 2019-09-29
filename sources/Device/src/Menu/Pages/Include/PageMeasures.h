#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Measures.h"


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
struct PageMeasures
{
    static const Page * const self;

    static const Page *GetPageFrequencyCounter();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresAuto
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageTune
    {
    public:
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresCursors
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSet
    {
        static const Page * const self;

        static void Draw_Channel(int x, int y);

        static void OnPress_Channel();
        /// Выбрать следующий курсор
        static void IncCursCntrlU(Chan::E ch);
        /// Выбрать следующий курсор
        static void IncCursCntrlT(Chan::E ch);

        static void OnPress_U();

        static void OnPress_T();

        static void SetCursPos100(Chan::E ch);
        /// Изменить значение позиции курсора напряжения на delta точек
        static void SetShiftCursPosU(Chan::E ch, int numCur, float delta);
        /// Изменить значение позиции курсора времени на delta точек
        static void SetShiftCursPosT(Chan::E ch, int numCur, float delta);

        static bool OnArrows(const KeyEvent &event);

        static void UpdateCursorsForLook();
        /// Установить позицию курсора напряжения
        static void SetCursorU(Chan::E ch, int numCur, float pos);
        /// Установить позицию курсора времени
        static void SetCursorT(Chan::E ch, int numCur, float pos);
        /// Возвращает true, если вращение ручки УСТАНОВКА будет перемещать курсоры/курсоры
        static bool IsRegSetActiveOnCursors();

    private:
        static const float MAX_POS_U;
        static const float MAX_POS_T;

        /// Установить источник курсорных измерений
        static void SetCursSource(Chan::E ch);
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresMath
{
    static const Page * const self;
};
