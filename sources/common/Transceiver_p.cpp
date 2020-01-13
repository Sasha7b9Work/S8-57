#include "defines.h"
#include "Decoder_p.h"
#include "Transceiver.h"


#define PORT_BUSY        GPIOC
#define PIN_BUSY         GPIO_PIN_14
#define BUSY             PORT_BUSY, PIN_BUSY

#define PORT_DATA_READY  GPIOC
#define PIN_DATA_READY   GPIO_PIN_15
#define DATA_READY       PORT_DATA_READY, PIN_DATA_READY

#define PORT_CS          GPIOC
#define PIN_CS           GPIO_PIN_13
#define CS               PORT_CS, PIN_CS

#define PORT_WR          GPIOD
#define PIN_WR           GPIO_PIN_5
#define WR               PORT_WR, PIN_WR

#define PORT_RD          GPIOD
#define PIN_RD           GPIO_PIN_4
#define RD               PORT_RD, PIN_RD



namespace PinBusy
{
    /// Установить признак того, что панель занята
    void SetActive()   { HAL_GPIO_WritePin(BUSY, GPIO_PIN_RESET); }
    /// Снять признак того, что панель занята
    void SetPassive() { HAL_GPIO_WritePin(BUSY, GPIO_PIN_SET); }

    void Init()
    {
        GPIO_InitTypeDef gpio;
        gpio.Pin = PIN_BUSY;
        gpio.Mode = GPIO_MODE_OUTPUT_OD;
        gpio.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(PORT_BUSY, &gpio);
    }
};


namespace PinDataReady
{
    void SetActive()   { HAL_GPIO_WritePin(DATA_READY, GPIO_PIN_RESET); }
    void SetPassive() { HAL_GPIO_WritePin(DATA_READY, GPIO_PIN_SET); }

    void Init()
    {
        GPIO_InitTypeDef gpio;
        gpio.Pin = PIN_DATA_READY;
        gpio.Mode = GPIO_MODE_OUTPUT_OD;
        gpio.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(PORT_DATA_READY, &gpio);
    }
};


namespace PinCS
{
    void Init()
    {
        GPIO_InitTypeDef gpio;
        gpio.Pin = PIN_CS;
        gpio.Mode = GPIO_MODE_INPUT;
        gpio.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(PORT_CS, &gpio);
    }

    bool IsActive()   { return HAL_GPIO_ReadPin(CS) == GPIO_PIN_RESET; }
    bool IsPassive() { return HAL_GPIO_ReadPin(CS) == GPIO_PIN_SET; }
};


namespace PinWR
{
    void Init()
    {
        GPIO_InitTypeDef gpio;
        gpio.Pin = PIN_WR;
        gpio.Mode = GPIO_MODE_INPUT;
        gpio.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(PORT_WR, &gpio);
    }

    bool IsActive()   { return HAL_GPIO_ReadPin(WR) == GPIO_PIN_RESET; }
    bool IsPassive() { return HAL_GPIO_ReadPin(WR) == GPIO_PIN_SET; }
};


namespace PinRD
{
    void Init()
    {
        GPIO_InitTypeDef gpio;
        gpio.Pin = PIN_CS;
        gpio.Mode = GPIO_MODE_INPUT;
        gpio.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(PORT_CS, &gpio);
    }

    bool IsActive()   { return HAL_GPIO_ReadPin(RD) == GPIO_PIN_RESET; }
    bool IsPassive() { return HAL_GPIO_ReadPin(RD) == GPIO_PIN_SET; }
};


struct DataBus
{
    static void Init();
    static void InitReceive();
    static void InitTransmit();

private:
    static bool inModeRecive;
};


void Transceiver::Init()
{
    PinBusy::Init();
    PinBusy::SetPassive();

    PinDataReady::Init();
    PinDataReady::SetPassive();

    PinCS::Init();
    PinRD::Init();
    PinWR::Init();

    DataBus::Init();
}


static void SetData(uint8 data)
{
    static const uint16 pins[8] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

    for(int i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(GPIOE, pins[i], static_cast<GPIO_PinState>(data & 0x01));
        data >>= 1;
    }
}


static void SendByte(uint8 data)
{
    SetData(data);                          // Выставляем шину данных

    PinBusy::SetPassive();                  // Выставляем признак того, что данные выставлены

    while(PinCS::IsActive()) {};            // Ждём, пока устройство считает данные

    PinBusy::SetActive();                   // Выставляем признак того, что подтерждение получено
}


void Transceiver::Send(const uint8 *data, uint size)
{
    return;
    
    PinBusy::SetActive();                   // Устанавливаем признак того, что панель занята

    PinDataReady::SetActive();              // Устанавливаем признак того, что у панели есть данные для передачи

    while(PinRD::IsPassive())               // Ждём, когда устройство будет готово к чтению
    {
        int i = 0;
    }

    DataBus::InitTransmit();                // Настраиваем шину данных на передачу

    for(uint i = 0; i < size; i++)
    {
        SendByte(*data++);
    }

    PinDataReady::SetPassive();             // Снимаем признак готовности данных для передачи

    while(PinRD::IsActive()) {}             // Ожидаем пока устройсво выйдет из режима чтения

    PinBusy::SetPassive();                  // Устанавливаем признак тогО, что панель готова к приёму данных

    DataBus::InitReceive();                 // Возвращаем шину данных в обычное состояние - режим приёма
}


bool Transceiver::Receive()
{
    static int counter = 0;

    static uint8 buffer[32];

    if(PinCS::IsActive() && PinWR::IsActive())
    {
        uint8 data = (uint8)GPIOE->IDR;     // Просто читаем данные 

        buffer[counter++] = data;

        if(counter == 32)
        {
            counter = counter;
        }

        PinBusy::SetActive();               // Устанавливаем признак того, что данные приняты

        PDecoder::AddData(data);            // Обрабатываем данные

        PinBusy::SetPassive();              // И устанавливаем признак того, что панель снова свободна

        return true;
    }

    return false;
}


void DataBus::InitReceive()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void DataBus::InitTransmit()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);
}


void DataBus::Init()
{
    InitReceive();
}
