#pragma once
#include "Osci/ParametersOsci.h"


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

    static LinkingTShift::E &Ref();
};


 struct SettingsTime
{
    int                 shift;
    TBase::E            base;
    PeakDetMode::E      peakDet;
    TPos::E             tPos;
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
