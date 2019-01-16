#pragma once


#define SET_TSHIFT           (set.time_shift)
#define SET_TBASE            (set.time_base)
#define TIME_DIV_XPOS        (set.time_timeDivXPos)

#define SET_RECORDER_SCALE_X (set.rec_scaleX)

#define SET_PEAKDET          (set.time_peakDet)
#define SET_PEAKDET_EN       (SET_PEAKDET == PeakDetMode::Enabled)
#define SET_PEAKDET_DIS      (SET_PEAKDET == PeakDetMode::Disabled)
                             
#define IN_RANDOM_MODE       (SET_TBASE <= TBase::_50ns)
#define IN_P2P_MODE          (SET_TBASE >= TBase::MIN_P2P)
                             
#define TPOS                 (set.time_TPos)
#define TPOS_IS_RIGHT        (TPOS == TPos::Right)
#define TPOS_IS_LEFT         (TPOS == TPos::Left)
#define TPOS_IS_CENTER       (TPOS == TPos::Center)
                             
#define SAMPLE_TYPE          (set.time_sampleType)
#define SAMPLE_TYPE_IS_REAL  (SAMPLE_TYPE == SampleType::Real)
#define SAMPLE_TYPE_OLD      (set.time_sampleTypeOld)
                             
#define LINKING_TSHIFT       (set.time_linkingTShift)
