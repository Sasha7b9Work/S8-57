#pragma once
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == Trig::Source::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == Trig::Source::B)
#define TRIG_SOURCE_IS_EXT      (TRIG_SOURCE == Trig::Source::Ext)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_HAND     (TRIG_MODE_FIND == Trig::ModeFind::Hand)
#define TRIG_MODE_FIND_AUTO     (TRIG_MODE_FIND == Trig::ModeFind::Auto)

#define START_MODE              (set.trig_startMode)
#define START_MODE_IS_AUTO      (START_MODE == Trig::StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == Trig::StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == Trig::StartMode::Single)

#define TRIG_POLARITY           (set.trig_polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig_input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig_TimeDelay)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace SET
    {
        void SetModeCouple(Chan::E ch, ModeCouple::E couple);
        /// Включить/выключить калибратор.
        void LoadCalibratorMode();
        /// Установить значение удержания синхронизации
        void LoadHoldfOff();
    };
}
