#pragma once
#include <stm32f4xx_hal.h>


namespace FPGA
{
    namespace HAL
    {
        void LoadRegUPR();

        ADC_HandleTypeDef *HandleADC();

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
