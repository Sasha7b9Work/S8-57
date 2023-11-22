// 2023/11/22 13:24:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "HAL_PIO.h"
#include <stm32f4xx_hal.h>


const uint16 HPin::_0 = GPIO_PIN_0;
const uint16 HPin::_1 = GPIO_PIN_1;
const uint16 HPin::_2 = GPIO_PIN_2;
const uint16 HPin::_3 = GPIO_PIN_3;
const uint16 HPin::_4 = GPIO_PIN_4;
const uint16 HPin::_5 = GPIO_PIN_5;
const uint16 HPin::_6 = GPIO_PIN_6;
const uint16 HPin::_7 = GPIO_PIN_7;
const uint16 HPin::_8 = GPIO_PIN_8;
const uint16 HPin::_9 = GPIO_PIN_9;
const uint16 HPin::_10 = GPIO_PIN_10;
const uint16 HPin::_11 = GPIO_PIN_11;
const uint16 HPin::_12 = GPIO_PIN_12;
const uint16 HPin::_13 = GPIO_PIN_13;
const uint16 HPin::_14 = GPIO_PIN_14;
const uint16 HPin::_15 = GPIO_PIN_15;


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
    GPIO_SPEED_FREQ_HIGH,
    GPIO_SPEED_FREQ_VERY_HIGH
};

static const uint alternates[HAlternate::Count] =
{
    GPIO_AF0_MCO,
    GPIO_AF5_SPI4,
    GPIO_AF7_USART3,
    GPIO_AF10_OTG_FS,
    GPIO_AF12_OTG_HS_FS
};


static GPIO_TypeDef * const ports[HPort::Count] =
{
    GPIOA, //-V2571
    GPIOB, //-V2571
    GPIOC, //-V2571
    GPIOD, //-V2571
    GPIOE, //-V2571
    GPIOF, //-V2571
    GPIOG, //-V2571
    GPIOH //-V2571
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


void HAL_PIO::Write(HPort::E port, uint16 pin, int state)
{
    HAL_GPIO_WritePin(PORT(port), pin, (state == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}


void HAL_PIO::Write(const StructPIN &pin, int state)
{
    HAL_GPIO_WritePin(PORT(pin.port), pin.pin, (state == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}


uint8 HAL_PIO::Read(HPort::E port, uint16 pin)
{
    uint8 result = static_cast<uint8>(HAL_GPIO_ReadPin(PORT(port), pin));

    return result;
}


void HAL_PIO::EXTI_CLEAR_IT_BIT(uint16 pin)
{
    __HAL_GPIO_EXTI_CLEAR_IT(pin); //-V2571
}
