#include "defines.h"
#include "Transceiver.h"
#include "Utils/DecoderPanel.h"


#define PORT_SELECT         GPIOC
#define PIN_SELECT          GPIO_PIN_13
#define SELECT              PORT_SELECT, PIN_SELECT

#define PORT_MODE           GPIOC
#define PIN_MODE            GPIO_PIN_14
#define MODE                PORT_MODE, PIN_MODE

#define PORT_WRITE_READY    GPIOD
#define PIN_WRITE_READY     GPIO_PIN_5
#define WRITE_READY         PORT_WRITE_READY, PIN_WRITE_READY

#define PORT_READ_READY     GPIOC
#define PIN_READ_READY      GPIO_PIN_15
#define READ_READY          PORT_READ_READY, PIN_READ_READY

#define PORT_CONFIRM        GPIOD
#define PIN_CONFIRM         GPIO_PIN_4
#define CONFIRM             PORT_CONFIRM, PIN_CONFIRM


namespace Transceiver
{
    /// /// Эту функцию нужно вызывать всякий раз при инициализации пинов на приём или передачу.
    void(*CallbackOnInitPins)();
    /// В этой функции все пины должны быть инициализированы на вход, чтобы не блокировать шины.
    void DeInitPins();

    namespace Transmitter
    {
        void Init();
        void InitDataPins();
    }

    namespace Receiver
    {
        void Update();

        void InitPins();
        void InitDataPins();

        uint8 ReadData();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_SELECT;              // SELECT - "0" на этом выходе означает, что устройство инициирует обмен с панелью
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_SELECT, &gpio);

    gpio.Pin = PIN_MODE;                // MODE - режим работы. "0" - приём от устройства, "1" - запись в устройство
    HAL_GPIO_Init(PORT_MODE, &gpio);

    gpio.Pin = PIN_READ_READY;          // READ_READ - сюда будем записывать признак того, что данные уже считаны
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_READ_READY, &gpio);

    DeInitPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::DeInitPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_WRITE_READY;         // WRITE_READY инициализируем на прослушивание - чтобы не мешать работе шины
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_WRITE_READY, &gpio);

    gpio.Pin = PIN_CONFIRM;
    HAL_GPIO_Init(PORT_CONFIRM, &gpio);

    Receiver::InitDataPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPins()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitDataPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 * /*data*/, uint /*size*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Transceiver::Receiver::ReadData()
{
    int bit[8];

    bit[0] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0);
    bit[1] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1);
    bit[2] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
    bit[3] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3);
    bit[4] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
    bit[5] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
    bit[6] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
    bit[7] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7);

    uint8 result = 0;

    for (int i = 0; i < 8; i++)
    {
        result += (bit[i] << i) & 0x01;
    }

    return result;
}
