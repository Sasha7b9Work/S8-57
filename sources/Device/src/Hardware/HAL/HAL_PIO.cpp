#include "defines.h"
#include "HAL_PIO.h"
#include <stm32f4xx_hal.h>


uint16 HPin::_0 = GPIO_PIN_0;
uint16 HPin::_1 = GPIO_PIN_1;
uint16 HPin::_2 = GPIO_PIN_2;
uint16 HPin::_3 = GPIO_PIN_3;
uint16 HPin::_4 = GPIO_PIN_4;
uint16 HPin::_5 = GPIO_PIN_5;
uint16 HPin::_6 = GPIO_PIN_6;
uint16 HPin::_7 = GPIO_PIN_7;
uint16 HPin::_8 = GPIO_PIN_8;
uint16 HPin::_9 = GPIO_PIN_9;
uint16 HPin::_10 = GPIO_PIN_10;
uint16 HPin::_11 = GPIO_PIN_11;
uint16 HPin::_12 = GPIO_PIN_12;
uint16 HPin::_13 = GPIO_PIN_13;
uint16 HPin::_14 = GPIO_PIN_14;
uint16 HPin::_15 = GPIO_PIN_15;


static const uint modes[HMode::Count] =
{
    GPIO_MODE_ANALOG,
    GPIO_MODE_OUTPUT_PP,
    GPIO_MODE_IT_RISING,
    GPIO_MODE_INPUT,
    GPIO_MODE_AF_PP
};

static const uint pulles[HPull::Count] =
{
    GPIO_NOPULL,
    GPIO_PULLDOWN,
    GPIO_PULLUP
};

static const uint speedes[HSpeed::Count] =
{
    GPIO_SPEED_FREQ_LOW,
    GPIO_SPEED_FREQ_VERY_HIGH
};

static const uint alternates[HAlternate::Count] =
{
    GPIO_AF0_MCO,
    GPIO_AF7_USART3
};


static GPIO_TypeDef * const ports[HPort::Size] =
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



void HAL_PIO::Init(HPort::E port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed, HAlternate::E alternate)
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


void HAL_PIO::Set(HPort::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_SET);
}


void HAL_PIO::Reset(HPort::E port, uint16 pin)
{
    HAL_GPIO_WritePin(PORT(port), pin, GPIO_PIN_RESET);
}


void HAL_PIO::Write(HPort::E port, uint16 pin, HState::E state)
{
    HAL_GPIO_WritePin(PORT(port), pin, (state == HState::Enabled) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool HAL_PIO::Read(HPort::E port, uint16 pin)
{
    return (HAL_GPIO_ReadPin(PORT(port), pin) == GPIO_PIN_SET);
}


void HAL_PIO::EXTI_CLEAR_IT_BIT(uint16 pin)
{
    __HAL_GPIO_EXTI_CLEAR_IT(pin);
}
