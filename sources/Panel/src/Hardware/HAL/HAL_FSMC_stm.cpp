#include "defines.h"
#include "common/Decoder_p.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include <stm32f4xx_hal.h>


#define PORT_READY  GPIOC
#define PIN_READY   GPIO_PIN_14
#define READY       PORT_READY, PIN_READY

#define PORT_DATA   GPIOC
#define PIN_DATA    GPIO_PIN_15
#define DATA        PORT_DATA, PIN_DATA

#define PORT_CS     GPIOC
#define PIN_CS      GPIO_PIN_13
#define CS          PORT_CS, PIN_CS

#define PORT_WR     GPIOD
#define PIN_WR      GPIO_PIN_5
#define WR          PORT_WR, PIN_WR

#define PORT_RD     GPIOD
#define PIN_RD      GPIO_PIN_4
#define RD          PORT_RD, PIN_RD


struct OutPin
{
    OutPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

    void Init()
    {
        GPIO_InitTypeDef is = { pin, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP };

        HAL_GPIO_Init(gpio, &is);

        SetPassive();
    }

    void SetActive()  { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET); }

    void SetPassive() { HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET); }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


struct InPin
{
    InPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

    void Init()
    {
        GPIO_InitTypeDef is = { pin, GPIO_MODE_INPUT, GPIO_PULLUP };

        HAL_GPIO_Init(gpio, &is);
    }

    bool IsActive()    { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET; }

    bool IsPassive()   { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET; }

    void WaitPassive() { while(IsActive()) { } }

    void WaitActive()  { while(IsPassive()) { } }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


/// На этом выводе будем выставлять признак готовности к коммуникации и признак подтверждения
static OutPin pinReady(READY);
/// Здесь будем выставлять признак готовности данных для передачи в устройство
static OutPin pinData(DATA);
/// По этому сигналу от основого МК начинаем транзакцию чтения/записи
static InPin  pinCS(CS);
/// Признак того, что основной МК осуществляет операцию записи в панель
static InPin  pinWR(WR);
/// Признак того, что основной МК осуществляет операцию чтения из панели
static InPin  pinRD(RD);

static Queue<uint8> queueData;


struct DataBus
{
    /// Первоначальная инициализация
    static void Init();
};


void HAL_FSMC::Init()
{
    pinReady.Init();
    pinReady.SetActive();

    pinData.Init();
    pinData.SetPassive();

    pinCS.Init();
    pinRD.Init();
    pinWR.Init();

    DataBus::Init();
}


void HAL_FSMC::SendToDevice(uint8 *data, uint size)
{
    while(size > 0)
    {
        queueData.Push(*data++);
        size--;
    }

    if(queueData.Size())
    {
        pinData.SetActive();
    }
}


void HAL_FSMC::Update()
{
    //while(pinCS.IsActive())
    //while(HAL_GPIO_ReadPin(PORT_CS, PIN_CS) == GPIO_PIN_RESET)
    while((PORT_CS->IDR & PIN_CS) == 0)
    {
        // Чтение байта из устройства

        //if(pinWR.IsActive())
        //if(HAL_GPIO_ReadPin(PORT_WR, PIN_WR) == GPIO_PIN_RESET)
        if((PORT_WR->IDR & PIN_WR) == 0)
        {
            //uint8 data = DataBus::Read();
            uint8 data = (uint8)GPIOE->IDR;

            //pinReady.SetPassive();
            //HAL_GPIO_WritePin(PORT_READY, PIN_READY, GPIO_PIN_SET);
            PORT_READY->BSRR = PIN_READY;

            PDecoder::AddData(data);        /// \todo Сейчас недостаток - пока не отработает PDecoder::AddData(), устройство не пойдёт дальше

            //while(pinCS.IsActive());
            //while(HAL_GPIO_ReadPin(PORT_CS, PIN_CS) == GPIO_PIN_RESET) {}
            while((PORT_CS->IDR & PIN_CS) == 0)
            {
            }

            //pinReady.SetActive();
            PORT_READY->BSRR = PIN_READY << 16;
        }

        // Запись байта в устройсто

        if((PORT_RD->IDR & PIN_RD) == 0 && queueData.Size())
        {
            // Конфигурируем ШД на запись
            GPIOE->MODER &= 0xffff0000U;
            GPIOE->MODER |= 0x00005555U;

            // Устанавливаем данные на ШД
            GPIOE->ODR = (GPIOD->ODR & 0xffff0000) + static_cast<uint16>(queueData.Front());

            //pinReady.SetPassive();
            PORT_READY->BSRR = PIN_READY;

            //pinCS.WaitPassive();
            //while(pinCS.IsActive()) { }
            //while(HAL_GPIO_ReadPin(PORT_CS, PIN_CS) == GPIO_PIN_RESET) { }
            while((PORT_CS->IDR & PIN_CS) == 0) { }

            //pinReady.SetActive();
            PORT_READY->BSRR = PIN_READY << 16;

            int size = queueData.Size();

            size = size;

            if(queueData.Size() == 0)
            {
                pinData.SetPassive();
            }

            // Конфигурируем ШД на чтение
            GPIOE->MODER &= 0xffff0000U;
        }
    }
}


void DataBus::Init()
{
    // Конфигурируем ШД на чтение
    GPIOE->MODER &= 0xffff0000U;
}
