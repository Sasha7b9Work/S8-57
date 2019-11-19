#pragma once
#include "Settings/SettingsOsci.h"


struct PeakDetMode
{
    enum E
    {
        Disabled,
        Enabled,
        Average
    } value;
    explicit PeakDetMode(E v) : value(v) {};
};

/// Положение точки синхронизация на сигнале.
struct TPos
{
    enum E
    {
        Left,          ///< Привязка к левому краю.
        Center,        ///< Привязка к центру.
        Right          ///< Привязка к правому краю.
    } value;
    explicit TPos(E v) : value(v) {};
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
    } value;
    explicit SampleType(E v) : value(v) {};
};

/// Функция ВР/ДЕЛ.
struct FunctionTime
{
    enum E
    {
        Time,
        ShiftInMemory
    } value;
};

/// Тип привязки к смещению по горизонтали
struct LinkingTShift
{
    enum E
    {
        Time,     ///< Смещение привязано к времени
        Position  ///< Смещение привязано к позиции
    } value;
};


 struct SettingsTime
{
    int                 shift;
    TBase::E            _base;
    PeakDetMode::E      peakDet;
    TPos::E             TPos;
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
