#include "defines.h"
#include "HAL_PORTS.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HAL
{
    namespace PORTS
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

        uint Mode::Analog = GPIO_MODE_ANALOG;
        uint Mode::Output_PP = GPIO_MODE_OUTPUT_PP;
        uint Mode::RisingIT = GPIO_MODE_IT_RISING;
        uint Mode::Input = GPIO_MODE_INPUT;

        uint Pull::No = GPIO_NOPULL;
        uint Pull::Down = GPIO_PULLDOWN;
        uint Pull::Up = GPIO_PULLUP;
    }
}


static GPIO_TypeDef * const ports[HAL::PORTS::Port::Size] =
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

#define PORT(p) (ports[p])


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::PORTS::Init(Port::E port, uint pins, uint mode, uint pull)
{
    GPIO_InitTypeDef isGPIO =
    {
        pins,
        mode,
        pull
    };

    HAL_GPIO_Init(PORT(port), &isGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PORTS::Set(Port::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PORTS::Reset(Port::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::PORTS::Write(Port::E port, uint16 pin, State::E state)
{
    HAL_GPIO_WritePin(PORT(port), pin, (state == State::Enabled) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool HAL::PORTS::Read(Port::E port, uint16 pin)
{
    return (HAL_GPIO_ReadPin(PORT(port), pin) == GPIO_PIN_SET);
}
