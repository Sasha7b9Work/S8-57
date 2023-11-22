// 2023/11/22 13:25:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "common/Decoder_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>
#ifdef DEVICE
#include "Osci/Osci.h"
#endif


static void (*funcAfterInteraction)() = nullptr;      // Эта функция будет вызвана после окончания взаимодействия с панелью


struct OutPin
{
    OutPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init()       { HAL_PIO::Init(port, pin, HMode::Output_PP, HPull::Up); SetPassive(); }
    void SetActive()  { HAL_PIO::Reset(port, pin); }
    void SetPassive() { HAL_PIO::Set(port, pin); }

    HPort::E port;
    uint16 pin;
};


struct InPin
{
    InPin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}

    void Init()        { HAL_PIO::Init(port, pin, HMode::Input, HPull::Up); }
    bool IsActive()    { return HAL_PIO::Read(port, pin) == 0; };
    bool IsPassive()   { return HAL_PIO::Read(port, pin) == 1; };
    void WaitActive()  { while(IsPassive()) { } }
    void WaitPassive() { while(IsActive()) { } }

    HPort::E port;
    uint16 pin;
};


struct DataBus
{
    // Первоначальная инициализация
    static void Init();
};


static bool interactionWithPanel = false;       // true означает, что шина находится в процессе обмена с панелью и запись по обычной FSMC в альтеру и память запрещена
static bool otherActionsAllowed = true;         // true означает, что во время приёма/передачи разрешены и другие действия, выполняемые в процессе транзакция - чтение точек в поточечном
                                                // режиме и регистраторе


// По активному состоянию этого пина панель определяет, что главный МК готов к взаимодействию
static OutPin pinCS(PIN_CS);
// По активновному состоянию этого пина панель определяте, что главный МК хочет делать передачу
static OutPin pinWR(PIN_WR);
// По активному состоянию этого пина панель определяте, что главный МК хочет делать чтение
static OutPin pinRD(PIN_RD);

// Активным состоянием этого пина панель сообщает о готовности к взаимодействию
static InPin pinReadyPAN(PIN_PAN_READY);
// Активное состояние этого пина сообщает о том, что панель имеет данные для пеередчи
static InPin pinDataPAN(PIN_PAN_DATA);


void HAL_BUS::InitPanel()
{
    pinReadyPAN.Init();
    pinDataPAN.Init();
    pinCS.Init();

    DataBus::Init();
}


bool HAL_BUS::Panel::Receive() //-V2506
{
    //if(pinReadyPAN.IsPassive() || pinDataPAN.IsPassive())
    //if((GPIOA->IDR & GPIO_PIN_7) != GPIO_PIN_RESET || (GPIOC->IDR & GPIO_PIN_4) != GPIO_PIN_RESET)
    if((GPIOA->IDR & GPIO_PIN_7) || (GPIOC->IDR & GPIO_PIN_4)) //-V2570 //-V2571
    {
        return false;
    }
   
    interactionWithPanel = true;
    
    if(mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        pinRD.Init();

        // Конфигурируем ШД на чтение

        GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3 //-V2571

        GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7 //-V2571
    }
    
    pinRD.SetActive();

    pinCS.SetActive();
    
    pinReadyPAN.WaitPassive();

    while(pinReadyPAN.IsActive()) {};
    
    uint8 data = 0;

    while(pinReadyPAN.IsActive())
    {
        if(pinDataPAN.IsPassive())
        {
            goto exit;
        }
    }
    
    //                                                 4,5,6,7              2,3                          0,1
    data = static_cast<uint8>((GPIOE->IDR >> 3) & 0xF0 | (GPIOD->IDR << 2) & 0x0C | (GPIOD->IDR >> 14)); //-V2571

    DDecoder::AddData(data);
    
exit:
    
    pinRD.SetPassive();

    pinCS.SetPassive();
    
    interactionWithPanel = false;

    ///
    // \todo Если убрать отсюда эти чтения, то нужно делать задержку, потому что без такой задержки зависает
    // 

    if(otherActionsAllowed)
    {
        if(Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if(OSCI_IN_MODE_P2P)
        {
            Roller::ReadPoint();
        }
        else
        {
            Timer::PauseOnTicks(10);
        }
    }
    else
    {
        Timer::PauseOnTicks(10);
    }

    return true;
}


void HAL_BUS::Panel::Send(uint8 byte)
{
    Send(&byte, 1);
}


void HAL_BUS::Panel::Send(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    
    Send(buffer, 2);
}

void HAL_BUS::Panel::Send(const uint8 *data, int size)
{
    if(!(GPIOA->IDR & GPIO_PIN_7) && !(GPIOC->IDR & GPIO_PIN_4)) //-V2570 //-V2571
    {
        while(Receive()) { }
    }

    interactionWithPanel = true;

    if(mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        pinWR.Init();

        // Конфигурируем ШД на запись

        GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3 //-V2571
        GPIOD->MODER |= 0x50000005U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP //-V2571

        GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7 //-V2571
        GPIOE->MODER |= 0x00154000U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP //-V2571
    }

    for(int i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //                                                                             биты 0,1                                 биты 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // Записываем данные в выходные пины //-V2571
        //                                                                          Биты 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3); //-V2571

        //pinWR.SetActive();                  // Даём сигнал записи
        GPIOD->BSRR = (uint)GPIO_PIN_5 << 16U; //-V2571
        
        //while(pinReadyPAN.IsPassive()) {}   // И ожидаем сигнал панели о том, что она свободна
        //while(HAL_PIO::Read(HPort::_A, HPin::_7) == 1) { };
        volatile uint state = GPIOA->IDR & GPIO_PIN_7; //-V2571
        while(state)
        {
            state = GPIOA->IDR & GPIO_PIN_7; //-V2571
        }

        //pinCS.SetActive();                  // Даём признак того, чта данные выставлены и можно их считывать
        GPIOG->BSRR = (uint)GPIO_PIN_12 << 16U; //-V2571

        //while(pinReadyPAN.IsActive()) {}    // Переключение PIN_PAN_READY в неактивное состояние означает, что панель приняла данные и обрабатывает их
        state = GPIOA->IDR & GPIO_PIN_7; //-V2571
        while(state == 0)
        {
            state = GPIOA->IDR & GPIO_PIN_7; //-V2571
        }

        //pinWR.SetPassive();                 // \ Устанавливаем WR и CS в неактивное состояние - элементарный цикл записи окончен
        GPIOD->BSRR = GPIO_PIN_5; //-V2571

        //pinCS.SetPassive();                 // /
        GPIOG->BSRR = GPIO_PIN_12; //-V2571
    }

    interactionWithPanel = false;

    if(otherActionsAllowed)
    {
        if(Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if(OSCI_IN_MODE_P2P) //-V2516
        {
            Roller::ReadPoint();
        }

        if(funcAfterInteraction)
        {
            funcAfterInteraction();
            funcAfterInteraction = nullptr;
        }
    }
}


bool HAL_BUS::Panel::InInteraction()
{
    return interactionWithPanel;
}


void HAL_BUS::Panel::RunAfterInteraction(void (*func)())
{
    funcAfterInteraction = func;
}


void DataBus::Init()
{
    // Конфигурируем ШД на чтение

    GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3 //-V2571

    GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7 //-V2571
}


void HAL_BUS::Panel::ProhibitOtherActions()
{
    otherActionsAllowed = false;
}


void HAL_BUS::Panel::AllowOtherActions()
{
    otherActionsAllowed = true;
}
