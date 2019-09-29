#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"
#include "Osci/Measurements/Measures.h"
#include "PageDisplay.h"
#include "Multimeter/Multimeter.h"


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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageFunction
{
    static const Page * const self;
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
