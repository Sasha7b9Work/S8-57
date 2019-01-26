#pragma once


namespace FPGA
{
    namespace HAL
    {
        void LoadRegUPR();

        void ReadFlag();

        extern uint16 flag;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        namespace Interrupt
        {
            namespace P2P
            {
                /// Инициализировать прерывание от чтение точки. При возниконовении прерывания будет выполняться функция func
                void Init(pFuncVV func);
                /// Включить прерывание
                void Enable();
                /// Выключить прерывание
                void Disable();
            };
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Flag
        {
            enum E
            {
                _DATA_READY        = 0,  ///< Данные готовы для считывания (окончание счётчика послезапуска)
                _TRIG_READY        = 1,  ///< Флаг синхроимпульса
                _PRED              = 2,  ///< Если 1, то предзапуск отсчитал, можно давать принудительный запуск (окончание
                _P2P               = 3,  ///< 
                _FREQ_READY        = 4,  ///< Флаг готовности измерения частоты
                _PERIOD_READY      = 5,  ///< Флаг готовности измерения периода
                _HOLD_OFF_FLAG     = 7,  ///< Принудительный запуск можно давать только когда этот флаг в единице
                _FREQ_OVERFLOW     = 8,  ///< Признак переполнения счётчика частоты
                _PERIOD_OVERFLOW   = 9,  ///< Признак переполнения счётчика периода
                _FREQ_IN_PROCESS   = 10, ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
                _PERIOD_IN_PROCESS = 11  ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
            } value;
        };

        struct GetFlag
        {
            static bool DATA_READY();
            static bool TRIG_READY();
            static bool PRED();
            static bool FREQ_READY();
            static bool HOLD_OFF();
            static bool PERIOD_READY();
            static bool FREQ_OVERFLOW();
            static bool PERIOD_OVERFLOW();
            static bool FREQ_IN_PROCESS();
            static bool PERIOD_IN_PROCESS();
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        namespace GPIO
        {
            void Init();

            struct Pin
            {
                enum E
                {
                    SPI3_SCK,
                    SPI3_DAT,
                    SPI3_CS1,
                    SPI3_CS2,
                    A1,
                    A2,
                    A3,
                    A4,
                    LF1,
                    LF2,
                    A1S,
                    A0S,
                    LFS,
                    Number
                } value;
                explicit Pin(E v) : value(v) {};
            };

            GPIO_TypeDef *GetPort(Pin::E pin);

            uint GetPin(Pin::E pin);

            void WritePin(Pin::E pin, int enable);

            void SetPin(Pin::E pin);

            void ResetPin(Pin::E pin);

            void WriteRegisters(Pin::E cs, uint16 value);
        }
    }
}
