#include "defines.h"
#include "Utils/DecoderPanel.h"
#include "FSMC.h"
#include "Timer.h"
#include "Display/Display.h"
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PORT_NE4        GPIOC
#define PIN_NE4         GPIO_PIN_13

#define PORT_PAN_0      GPIOC
#define PIN_PAN_0       GPIO_PIN_14

#define PORT_PAN_1      GPIOC
#define PIN_PAN_1       GPIO_PIN_15

#define NE4_IS_HI       ((PORT_NE4->IDR & PIN_NE4) != GPIO_PIN_RESET)
#define NE4_IS_LOW      ((PORT_NE4->IDR & PIN_NE4) == GPIO_PIN_RESET)

#define PAN_0_SET       (PORT_PAN_0->BSRR = PIN_PAN_0)
#define PAN_0_RESET     (PORT_PAN_0->BSRR = (uint)PIN_PAN_0 << 16U)

#define PAN_1_SET       (PORT_PAN_1->BSRR = PIN_PAN_1)
#define PAN_1_RESET     (PORT_PAN_1->BSRR = (uint)PIN_PAN_1 << 16U)

#define SET_BUSY                        PAN_0_RESET; PAN_1_RESET;
#define SET_READY_TRANSMIT              PAN_0_SET;   PAN_1_RESET;
#define SET_READY_RECEIVE               PAN_0_RESET; PAN_1_SET;
#define SET_RECEIVE_TRANSMIT_CONFIRM    PAN_0_SET;   PAN_1_SET;


bool FSMC::inReadState = false;

static bool inModeNow = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FSMC::Init()
{
    /// \todo Надо бы разобраться, как аппаратно параллельную шину наладить. Ну а пока будем делать ручками

    GPIO_InitTypeDef isGPIO =           //  PAN_0, PAN_1
    {
        PIN_PAN_0 | PIN_PAN_1,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(PORT_PAN_0, &isGPIO);

    isGPIO.Pin = PIN_NE4;               // NE4
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PORT_NE4, &isGPIO);

    SET_BUSY;

    ConfigToRead();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::SetNowMode(bool nowMode)
{
    inModeNow = nowMode;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FSMC::InNowMode()
{
    return inModeNow;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::ConfigToRead()
{
    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_INPUT,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::ConfigToWrite()
{
    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(GPIOE, &isGPIO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FSMC::ReadByte()
{
    inReadState = true;
    
    SET_READY_RECEIVE;                  // Устанавливаем признак того, что мы готовы к приёму данных

    while (NE4_IS_HI) {};               // Ждём, пока NE4 установится в ноль, сигнализирую о том, что данные выставлены на шину

    uint8 data = (uint8)GPIOE->IDR;     // Читаем данные с шины

    SET_RECEIVE_TRANSMIT_CONFIRM;       // Выставляем признак приёма информации

    while (NE4_IS_LOW) {};              // И ждём, пока устройство воспримет эту информацию

    SET_BUSY;
        
    inReadState = false;
    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteBuffer(uint8 *data, int length)
{
#define SIZE_BUFFER (1024 * 64)
    static uint8 buffer[SIZE_BUFFER] __attribute__ ((section("CCM_DATA")));
    static int pointer = 0;

    if (inReadState)                                        // Если идёт процесс приёма
    {
        if (data)
        {
            memcpy(buffer + pointer, data, (uint)length);   // то сохраняем информацию для передачи в будущем
            pointer += length;
        }
        return;                                             // и выходим
    }
    
    if (data)                                               // Если есть данные для передачи
    {
        memcpy(buffer + pointer, data, (uint)length);       // то переписываем их в буфер
        pointer += length;
    }

    if (pointer)                            // Если буфер передачи не пуст
    {
        ConfigToWrite();    /// \todo Здесь всё-таки не совсем правлиьно. Во время этой функции RD устанавливается в 1, что будет подсаживать
                            /// этот сигнал от основного процессора, что будет вызывать сбои чтения.

        for (int i = 0; i < pointer; i++)
        {
            SET_READY_TRANSMIT;             // Устанавливаем признак того, что сейчас будет передаваться информация

            while (NE4_IS_HI) {};           // Ждём признак того, что устройство готово к приёму информации

            GPIOE->ODR = buffer[i];         // Записываем байт в выход

            SET_RECEIVE_TRANSMIT_CONFIRM;   // Устанавливаем признак того, что данные установлены

            while (NE4_IS_LOW) {};          // Ждём, что устройство считало информацию
        }

        pointer = 0;

        SET_BUSY;

        ConfigToRead();
    }
}
