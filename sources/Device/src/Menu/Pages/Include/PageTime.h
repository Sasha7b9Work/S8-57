#pragma once
#include "Osci/Osci_Settings.h"


#define TPOS                (set.time.TPos)
#define TPOS_IS_RIGHT       (TPOS == TPos::Right)
#define TPOS_IS_LEFT        (TPOS == TPos::Left)
#define TPOS_IS_CENTER      (TPOS == TPos::Center)

#define SAMPLE_TYPE         (set.time.sampleType)
#define SAMPLE_TYPE_IS_REAL (SAMPLE_TYPE == SampleType::Real)
#define SAMPLE_TYPE_OLD     (set.time.sampleTypeOld)

#define LINKING_TSHIFT      (set.time.linkingTShift)


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
