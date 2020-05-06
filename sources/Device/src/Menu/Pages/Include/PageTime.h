#pragma once
#include "Osci/ParametersOsci.h"


#define S_PEAK_DET          (set.time._peakDet)
#define S_PEAK_DET_ENABLED  (S_PEAK_DET == PeakDetMode::Enabled)
struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled
    };

    static bool IsEnabled();
};


// Положение точки синхронизация на сигнале.
#define S_TPOS              (set.time._tPos)
#define S_TPOS_IS_LEFT      (S_TPOS == TPos::Left)
#define S_TPOS_IS_CENTER    (S_TPOS == TPos::Center)
struct TPos
{
    enum E
    {
        Left,          // Привязка к левому краю.
        Center,        // Привязка к центру.
        Right          // Привязка к правому краю.
    };

    // Узнать привязку отсительно уровня синхронизации в байтах
    static int InBytes();

    static void Draw();

    static int PosX();
};


// Тип выборки для режима рандомизатора.
struct SampleType
{
    enum E
    {
        Real,    // реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается.
        Equal    // эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора.
    };
};


// Функция ВР/ДЕЛ.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    };
};

// Тип привязки к смещению по горизонтали
struct LinkingTShift
{
    enum E
    {
        Time,     // Смещение привязано к времени
        Position  // Смещение привязано к позиции
    };
};


#define S_TIME_SHIFT    (set.time._shift)

 struct SettingsTime
{
    int                 _shift;
    TBase::E            _base;
    PeakDetMode::E      _peakDet;
    TPos::E             _tPos;
    SampleType::E       sampleType;
    FunctionTime::E     _;
    LinkingTShift::E    linkingTShift;  // Тип привязки смещения по горизонтали
    SampleType::E       sampleTypeOld;
};



struct PageTime
{
    static void OnChanged_PeakDet(bool active);

    static void OnChanged_TPos(bool active);

    static const Page * const self;
};
