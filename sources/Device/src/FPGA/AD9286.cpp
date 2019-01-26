#include "defines.h"
#include "AD9286.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


using namespace Hardware;

using Hardware::Timer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_SCK     GPIO_PIN_10
#define PORT_SCK    GPIOB

#define PIN_DAT     GPIO_PIN_3
#define PORT_DAT    GPIOC

#define PIN_CS      GPIO_PIN_11
#define PORT_CS     GPIOE

#define SCK         PORT_SCK, PIN_SCK
#define DAT         PORT_DAT, PIN_DAT
#define CS          PORT_CS,  PIN_CS


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SetPin(GPIO_TypeDef *gpio, uint16 pin);
static void ResetPin(GPIO_TypeDef *gpio, uint16 pin);
static void  WriteByte(uint8 address, uint8 byte);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD9286::Init()
{
    /*
        Для чипселекта используется вывод
        А вот для тактов и данных используются те же, выводы, которые в дальнейшем управляют микросхемой EEPROM AT25160

        CS  - 64 - PE11
        DAT - 29 - PC3
        SCK - 69 - PB10
    */
   
    GPIO_InitTypeDef isGPI0 =                   // Инициализация CS
    {
        PIN_CS,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(PORT_CS, &isGPI0);

    isGPI0.Pin = PIN_DAT;                       // Инициализация DAT
    HAL_GPIO_Init(PORT_DAT, &isGPI0);
    
    isGPI0.Pin = PIN_SCK;                       // Инициализация SCK
    HAL_GPIO_Init(PORT_SCK, &isGPI0);

    SetPin(CS); //-V525
    ResetPin(DAT);
    ResetPin(SCK);

    Tune();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::Tune()
{
    WriteByte(0x09, 0x09);
    /// \todo Это должно быть в функции записи
    WriteByte(0xff, 0x01);   // Пишем бит подтверждения.
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteByte(uint8 address, uint8 byte)
{
    ResetPin(CS);

    uint value = (uint)((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            SetPin(DAT);
        }
        else
        {
            ResetPin(DAT);
        }
        PAUSE_ON_TICKS(100);

        SetPin(SCK);
        PAUSE_ON_TICKS(100);

        ResetPin(SCK);
    }

    SetPin(CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetPin(GPIO_TypeDef *_gpio, uint16 pin)
{
    HAL_GPIO_WritePin(_gpio, pin, GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ResetPin(GPIO_TypeDef *_gpio, uint16 pin)
{
    HAL_GPIO_WritePin(_gpio, pin, GPIO_PIN_RESET);
}
