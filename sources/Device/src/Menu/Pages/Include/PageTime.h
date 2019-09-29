#pragma once
#include "Osci/Osci_Settings.h"


 struct SettingsTime
{
    int                 shift;
    TBase::E            base;
    PeakDetMode::E      peakDet;
    TPos::E             TPos;
    SampleType::E       sampleType;
    FunctionTime::E     timeDivXPos;
    LinkingTShift::E    linkingTShift;  ///< Тип привязки смещения по горизонтали
    SampleType::E       sampleTypeOld;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageTime
{
    static void OnChanged_PeakDet(bool active);

    static void OnChanged_TPos(bool active);

    static const Page * const self;
};
