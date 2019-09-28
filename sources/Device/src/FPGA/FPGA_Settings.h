#pragma once
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define START_MODE              (set.trig.startMode)
#define START_MODE_IS_AUTO      (START_MODE == Osci::Settings::Trig::StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == Osci::Settings::Trig::StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == Osci::Settings::Trig::StartMode::Single)

#define TRIG_POLARITY           (set.trig.polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig.input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig.TimeDelay)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace Settings
    {
        /// Включить/выключить калибратор.
        void LoadCalibratorMode();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Режим канала по входу.
        struct ModeCouple
        {
            enum E
            {
                DC,      ///< Закрытый вход
                AC,      ///< Открытый вход
                GND,     ///< Вход заземлён.
                Size
            } value;
            explicit ModeCouple(E v) : value(v) {};
            pString UGO() const;

            static void Set(Chan::E ch, ModeCouple::E couple);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Bandwidth
        {
            enum E
            {
                Full,     ///< Если это значение выбрано в меню КАНАЛ, то при этом положение устанавливается полоса из ОТЛАДКА-КАНАЛЫ-Полоса.
                _20MHz,
            } value;

            explicit Bandwidth(E v) : value(v) {};
            void Load();

        private:
            /// Возвращает канал, для которого является действующей эта настройка
            Chan::E GetChannel() const;
        };
    }
}
