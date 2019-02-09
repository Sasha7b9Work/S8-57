#include "defines.h"
#include "HAL_PIO.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HAL
{
    namespace PIO
    {
        uint16 Pin::_0 = GPIO_PIN_0;
        uint16 Pin::_1 = GPIO_PIN_1;
        uint16 Pin::_2 = GPIO_PIN_2;
        uint16 Pin::_3 = GPIO_PIN_3;
        uint16 Pin::_4 = GPIO_PIN_4;
        uint16 Pin::_5 = GPIO_PIN_5;
        uint16 Pin::_6 = GPIO_PIN_6;
        uint16 Pin::_7 = GPIO_PIN_7;
        uint16 Pin::_8 = GPIO_PIN_8;
        uint16 Pin::_9 = GPIO_PIN_9;
        uint16 Pin::_10 = GPIO_PIN_10;
        uint16 Pin::_11 = GPIO_PIN_11;
        uint16 Pin::_12 = GPIO_PIN_12;
        uint16 Pin::_13 = GPIO_PIN_13;
        uint16 Pin::_14 = GPIO_PIN_14;
        uint16 Pin::_15 = GPIO_PIN_15;

        static const uint modes[Mode::Size] =
        {
            GPIO_MODE_ANALOG,
            GPIO_MODE_OUTPUT_PP,
            GPIO_MODE_IT_RISING,
            GPIO_MODE_INPUT,
            GPIO_MODE_AF_PP
        };

        static const uint pulles[Pull::Size] =
        {
            GPIO_NOPULL,
            GPIO_PULLDOWN,
            GPIO_PULLUP
        };

        static const uint speedes[Speed::Size] =
        {
            GPIO_SPEED_FREQ_LOW,
            GPIO_SPEED_FREQ_VERY_HIGH
        };

        static const uint alternates[Alternate::Speed] =
        {
            GPIO_AF0_MCO,
            GPIO_AF7_USART3
        };
    }
}


static GPIO_TypeDef * const ports[HAL::PIO::Port::Size] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH
};

#define PORT(p)      (ports[p])
#define MODE(m)      (modes[m])
#define PULL(p)      (pulles[p])
#define SPEED(s)     (speedes[s])
#define ALTERNATE(a) (alternates[a])


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::PIO::Init(Port::E port, uint pins, Mode::E mode, Pull::E pull, Speed::E speed, Alternate::E alternate)
{
    GPIO_InitTypeDef isGPIO =
    {
        pins,
        MODE(mode),
        PULL(pull),
        SPEED(speed),
        ALTERNATE(alternate)
    };

    HAL_GPIO_Init(PORT(port), &isGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PIO::Set(Port::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PIO::Reset(Port::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PIO::Write(Port::E port, uint16 pin, State::E state)
{
    HAL_GPIO_WritePin(PORT(port), pin, (state == State::Enabled) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool HAL::PIO::Read(Port::E port, uint16 pin)
{
    return (HAL_GPIO_ReadPin(PORT(port), pin) == GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PIO::EXTI_CLEAR_IT_BIT(uint16 pin)
{
    __HAL_GPIO_EXTI_CLEAR_IT(pin);
}
