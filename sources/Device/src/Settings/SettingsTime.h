#pragma once


#define SET_TSHIFT          (TimeShift)
#define SET_TBASE           (set.time_base)
#define TIME_DIV_XPOS       (set.time_timeDivXPos)

#define SET_PEAKDET         (set.time_peakDet)
#define SET_PEAKDET_EN      (SET_PEAKDET == Osci::Settings::PeakDetMode::Enabled)
#define SET_PEAKDET_DIS     (SET_PEAKDET == Osci::Settings::PeakDetMode::Disabled)
                            
#define TPOS                (set.time_TPos)
#define TPOS_IS_RIGHT       (TPOS == TPos::Right)
#define TPOS_IS_LEFT        (TPOS == TPos::Left)
#define TPOS_IS_CENTER      (TPOS == TPos::Center)
                            
#define SAMPLE_TYPE         (set.time_sampleType)
#define SAMPLE_TYPE_IS_REAL (SAMPLE_TYPE == SampleType::Real)
#define SAMPLE_TYPE_OLD     (set.time_sampleTypeOld)
                            
#define LINKING_TSHIFT      (set.time_linkingTShift)
