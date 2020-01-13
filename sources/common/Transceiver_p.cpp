#include "defines.h"
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



struct FlagBusy
{
    /// Установить признак того, что панель занята
    static void Set()   { HAL_GPIO_WritePin(BUSY, GPIO_PIN_SET); }
    /// Снять признак того, что панель занята
    static void Reset() { HAL_GPIO_WritePin(BUSY, GPIO_PIN_RESET); }
};


struct FlagDataReady
{
    static void Set()   { HAL_GPIO_WritePin(DATA_READY, GPIO_PIN_RESET); }
    static void Reset() { HAL_GPIO_WritePin(DATA_READY, GPIO_PIN_SET); }
};


struct FlagCS
{
    static bool IsSet()   { return HAL_GPIO_ReadPin(CS) == GPIO_PIN_RESET; }
    static bool IsReset() { return HAL_GPIO_ReadPin(CS) == GPIO_PIN_SET; }
};


struct FlagWR
{
    static bool IsSet()   { return HAL_GPIO_ReadPin(WR) == GPIO_PIN_RESET; }
    static bool IsReset() { return HAL_GPIO_ReadPin(WR) == GPIO_PIN_SET; }
};


struct FlagRD
{
    static bool IsSet()   { return HAL_GPIO_ReadPin(RD) == GPIO_PIN_RESET; }
    static bool IsReset() { return HAL_GPIO_ReadPin(RD) == GPIO_PIN_SET; }
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
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_BUSY |        // BUSY - здесь выставляется признак занятости
        PIN_DATA_READY;          // DATA_READY - здесь выставляется признак того, что есть данные для передачи
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PORT_BUSY, &gpio);

    FlagBusy::Reset();
    FlagDataReady::Reset();

    gpio.Pin = PIN_CS |         // CS от устройства. По этому сигналу будем начинать операцию чтения или записи
        PIN_WR |                // WR - Признак того, что устройство выполняет запись
        PIN_RD;                 // RD - Признак того, что устройство выполняет чтение
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PORT_CS, &gpio);
}


void Transceiver::Send(const uint8 *, uint)
{

}


bool Transceiver::Receive()
{
    if(FlagCS::IsReset() && FlagWR::IsReset())
    {
        return false;
    }

    return true;
}


void DataBus::InitReceive()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);
}
