#include "defines.h"
#include "common/Decoder_p.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include <stm32f4xx_hal.h>


#define PORT_READ_READY  GPIOC
#define PIN_READ_READY   GPIO_PIN_14
#define READ_READY       PORT_READ_READY, PIN_READ_READY

#define PORT_SEND_READY  GPIOC
#define PIN_SEND_READY   GPIO_PIN_15
#define SEND_READY       PORT_SEND_READY, PIN_SEND_READY

#define PORT_CS          GPIOC
#define PIN_CS           GPIO_PIN_13
#define CS               PORT_CS, PIN_CS

#define PORT_TYPE        GPIOE
#define PIN_TYPE         GPIO_PIN_4
#define TYPE             PORT_TYPE, PIN_TYPE

#define PORT_CONF_PAN    GPIOE
#define PIN_CONF_PAN     GPIO_PIN_5
#define CONF_PAN         PORT_CONF_PAN, PIN_CONF_PAN

#define PORT_NIBBLE      GPIOE
#define PIN_NIBBLE       GPIO_PIN_6
#define NIBBLE           PORT_NIBBLE, PIN_NIBBLE

#define PORT_CONF_MCU    GPIOE
#define PIN_CONF_MCU     GPIO_PIN_7
#define CONF_MCU         PORT_CONF_MCU, PIN_CONF_MCU


struct OutPin
{
    OutPin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) {};

    void Init()
    {
        GPIO_InitTypeDef is = { pin, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP };

        HAL_GPIO_Init(gpio, &is);

        SetPassive();
    }

    // "Выходные" пины нужно переводить в состояние прослушивания, когда не идёт процесс обмена во избежание вредного влияния на ШД
    void DeInit()
    {

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

    bool IsLow()       { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET; }

    bool IsHi()        { return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET; }

    GPIO_TypeDef *gpio;
    uint16 pin;
};


// Здесь выставляем "0", когда панель готова к приёмму данных
static OutPin pinReadReady(READ_READY);
// Здесь выставляем "1", когда панель готова к передаче данных
static OutPin pinSendReady(SEND_READY);
// На этом пине нужно выставлять "0" для подтверждения передачи/приёма
static OutPin pinConfPanel(CONF_PAN);

// Переключение в "1" означает начало транзакции со стороны ЦП. Переключение в "0" - конец транзакции (данные выставлены или считаны)
static InPin pinCS(CS);
// "0" - ЦП читает из панели, "1" - ЦП записывает в панель
static InPin pinType(TYPE);
// "0" - идёт работа с младшим полубайтом, "1" - со старшим
static InPin pinNibble(NIBBLE);
// Здесь ЦП выставялет нулём признак подтверждения операции
static InPin pinConfCPU(CONF_MCU);


static Queue<uint8> queueData;


struct DataBus
{
    // Инициализировать на чтение
    static void InitRead();
    // Инициализровть на запись
    static void InitWrite();
};


void HAL_BUS::Init()
{
    pinReadReady.Init();    // Здесь "0", если панель готова к приёму информации
    pinSendReady.Init();    // Здесь "0", если у панели есть информация для передачи
    pinCS.Init();           // Здесь ЦП выставляет "0" в начале транзакции и "1" в конце

    pinType.Init();         // "0" - ЦП читает из панели, "1" - записывает в панель
    pinNibble.Init();       // "0" - идёт работа с млашшим полубайтом, "1" - со старшим
    pinConfCPU.Init();      // Переходом с "1" в "0 ЦП даёт подтверждение завершения операции

    pinConfPanel.DeInit();  // Переводим пин в режим прослушивания чтобы не шунтировать шину данных

    DataBus::InitRead();
}


void HAL_BUS::SendToDevice(uint8 *data, uint size)
{
    while(size > 0)
    {
        queueData.Push(*data++);
        size--;
    }

//    pinSendReady.SetActive();
}


void HAL_BUS::Update()
{
    pinReadReady.SetActive();   // Если мы оказались в этой точке, значит, готовы к приёму данных

    if(pinCS.IsActive())       // Если панель начала транзакцию
    {
        if(pinType.IsLow())     // То анализируем тип транзакции
        {
            TransmitData();     // Передача
        }
        else
        {
            ReceiveData();      // Или приём
        }
    }
}


void HAL_BUS::TransmitData()
{
    // Сюда попадаем, когда ЦП уже начал транзакцию
}


void HAL_BUS::ReceiveData()
{
    // Сюда попадаем, когда ЦП уже начал транзакцию

    pinConfPanel.Init();


}


void DataBus::InitRead()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
        GPIO_MODE_INPUT,
        GPIO_PULLUP
    };

    HAL_GPIO_Init(GPIOE, &is);
}


void DataBus::InitWrite()
{
    GPIO_InitTypeDef is
    {
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };

    HAL_GPIO_Init(GPIOE, &is);
}
