#pragma once
#include "Settings/SettingsOsci.h"


struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled
    };

    PeakDetMode() {};

    operator PeakDetMode::E();

    bool IsEnabled() const;
};

/// Положение точки синхронизация на сигнале.
struct TPos
{
    enum E
    {
        Left,          ///< Привязка к левому краю.
        Center,        ///< Привязка к центру.
        Right          ///< Привязка к правому краю.
    };

    TPos() {};

    operator TPos::E();
    /// Узнать привязку отсительно уровня синхронизации в байтах
    int InBytes();
};

/// Тип выборки для режима рандомизатора.
struct SampleType
{
    enum E
    {
        Real,    ///< реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается.
        Equal    ///< эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора.
    };

    SampleType() {};

    void Set(SampleType::E type);

    operator SampleType::E();
};

/// Функция ВР/ДЕЛ.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    };
};

/// Тип привязки к смещению по горизонтали
struct LinkingTShift
{
    enum E
    {
        Time,     ///< Смещение привязано к времени
        Position  ///< Смещение привязано к позиции
    };
};


 struct SettingsTime
{
    int                 shift;
    TBase::E            base;
    PeakDetMode::E      peakDet;
    TPos::E             tPos;
    SampleType::E       sampleType;
    FunctionTime::E     timeDivXPos;
    LinkingTShift::E    linkingTShift;  ///< Тип привязки смещения по горизонтали
    SampleType::E       sampleTypeOld;
};



struct PageTime
{
    static void OnChanged_PeakDet(bool active);

    static void OnChanged_TPos(bool active);

    static const Page * const self;
};
