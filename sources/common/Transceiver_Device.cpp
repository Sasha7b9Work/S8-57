#include "defines.h"
#include "Transceiver.h"
#include <stm32f4xx_hal.h>


#define PORT_SELECT         GPIOG
#define PIN_SELECT          GPIO_PIN_12
#define SELECT              PORT_SELECT, PIN_SELECT

#define PORT_MODE           GPIOA
#define PIN_MODE            GPIO_PIN_7
#define MODE                PORT_MODE, PIN_MODE

#define PORT_WRITE_READY    GPIOD
#define PIN_WRITE_READY     GPIO_PIN_5
#define WRITE_READY         PORT_WRITE_READY, PIN_WRITE_READY

#define PORT_READ_READY     GPIOC
#define PIN_READ_READY      GPIO_PIN_4
#define READ_READY          PORT_READ_READY, PIN_READ_READY

#define PORT_CONFIRM        GPIOD
#define PIN_CONFIRM         GPIO_PIN_4
#define CONFIRM             PORT_CONFIRM, PIN_CONFIRM


namespace Transceiver
{
    /// Эту функцию нужно вызывать всякий раз при инициализации пинов на приём или передачу.
    void(*CallbackOnInitPins)();

    namespace Transmitter
    {
        /// Инициализировать выходы в режим передачи.
        void InitPins();
        /// Установка режима работы - приём или передача; "0" - запись в панель, "1" - чтение из панели.
        void Write_MODE(int);
        /// Выбор панели для обмена. Активный уровень - "0".
        void Write_SELECT(int);
        /// Установка признака того, что данные выставлены на ШД.
        void Write_READY_WR(int);
        /// Считать подтверждение того, чо панель прочитала данные.
        int Read_READY_RD();
        /// Записываем в панель признак того, что подтверждение принято.
        void Write_CONFIRM(int);
        /// Установить состояние линий ШД в соответствии с битами data 
        void SetData(uint8 data);
    }

    namespace Receiver
    {
        void InitPinsReceive();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_SELECT;              // SELECT - выбор панели для работы
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_SELECT, &gpio);

    Transmitter::Write_SELECT(1);       // Отключаем обмен с панелью

    gpio.Pin = PIN_MODE;                // MODE - режим работы с панелью
    HAL_GPIO_Init(PORT_MODE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPins()
{
    CallbackOnInitPins();

    Write_MODE(0);                      // Будем записывать в панель

    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_WRITE_READY;         // WRITE_READY - сюда будем выставлять признак того, что данные выставлены и готовы для чтения
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_WRITE_READY, &gpio);

    Write_READY_WR(1);                  // Устанавливаем признак того, что данные на шину не выставлены

    gpio.Pin = PIN_CONFIRM;             // CONFIRM_READ - сюда выставляем признак того, что подтверждение от панели получено
    HAL_GPIO_Init(PORT_CONFIRM, &gpio);

    Write_CONFIRM(1);                   // Устанавливаем признак того, что чтение не подтверждено

    gpio.Pin = PIN_READ_READY;          // READ_REDY - отсюда считываем подтверждение чтения от панели
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PORT_READ_READY, &gpio);

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin =  GPIO_PIN_0  |           // D2
                GPIO_PIN_1  |           // D3
                GPIO_PIN_14 |           // D0
                GPIO_PIN_15;            // D1
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin =  GPIO_PIN_7 |            // D4
                GPIO_PIN_8 |            // D5
                GPIO_PIN_9 |            // D6
                GPIO_PIN_10;            // D7
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPinsReceive()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    InitPins();

    Write_READY_WR(1);

    Write_SELECT(0);                     // Выбираем панель устройством, с которым будет производиться обмен

    for (uint i = 0; i < size; i++)
    {
        Write_CONFIRM(1);                // Устанавливаем запрет флага подтверждеия прёма данных панелью

        SetData(data[i]);                // Устанавливаем линии ШД в соответствии с очередным передаваемым байтом

        Write_READY_WR(0);               // Даём знак панели, что данные выставлены и можно их считывать. 

        while (Read_READY_RD() == 1) {}; // Ждём подтверждение чтения от панели. Панель выставляет "0" на этом выводе, когда прочитала данные

        Write_CONFIRM(0);                // Выставляем признак того, что получили подтверждения чтения.

        while (Read_READY_RD() == 0) {}; // И ждём до тех, пор, пока панель его не воспримет. После того, как панель восприняла CONFIRM == 0, она выставляет READY_RD в единицу и идёт на
                                         // на обработку принятой информации
        Write_READY_WR(1);
    }

    Write_SELECT(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_MODE(int mode)
{
    HAL_GPIO_WritePin(MODE, (GPIO_PinState)mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_SELECT(int select)
{
    HAL_GPIO_WritePin(SELECT, (GPIO_PinState)select);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_CONFIRM(int confirm)
{
    HAL_GPIO_WritePin(CONFIRM, (GPIO_PinState)confirm);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_READY_WR(int ready)
{
    HAL_GPIO_WritePin(WRITE_READY, (GPIO_PinState)ready);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::SetData(uint8 data)
{
                                    // D0          D1           D2          D3          D4          D5          D6          D7
    static GPIO_TypeDef *ports[] = { GPIOD,       GPIOD,       GPIOD,      GPIOD,      GPIOE,      GPIOE,      GPIOE,      GPIOE };
    static uint16 pins[] =         { GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 };

    for (int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], (GPIO_PinState)((data >> i) & 0x01));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Transceiver::Transmitter::Read_READY_RD()
{
    return HAL_GPIO_ReadPin(READ_READY);
}
