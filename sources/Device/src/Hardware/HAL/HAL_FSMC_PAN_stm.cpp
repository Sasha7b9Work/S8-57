#include "defines.h"
#include "common/Decoder_d.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


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
    /// Первоначальная инициализация
    static void Init();
};


/// По активному состоянию этого пина панель определяет, что главный МК готов к взаимодействию
static OutPin pinCS(PIN_CS);
/// По активновному состоянию этого пина панель определяте, что главный МК хочет делать передачу
static OutPin pinWR(PIN_WR);
/// По активному состоянию этого пина панель определяте, что главный МК хочет делать чтение
static OutPin pinRD(PIN_RD);

/// Активным состоянием этого пина панель сообщает о готовности к взаимодействию
static InPin pinReadyPAN(PIN_PAN_READY);
/// Активное состояние этого пина сообщает о том, что панель имеет данные для пеередчи
static InPin pinDataPAN(PIN_PAN_DATA);

/// true означает, что шина находится в процессе обмена с панелью и запись по обычной FSMC в альтеру и память запрещена
static bool interactionWithPanel = false;

void HAL_FSMC::InitPanel()
{
    pinReadyPAN.Init();
    pinDataPAN.Init();
    pinCS.Init();

    DataBus::Init();
}


bool HAL_FSMC::Receive()
{
    //if(pinReadyPAN.IsPassive() || pinDataPAN.IsPassive())
    if((GPIOA->IDR & GPIO_PIN_7) || (GPIOC->IDR & GPIO_PIN_4))
    {
        return false;
    }
   
    interactionWithPanel = true;
    
    if(mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        //pinWR.Init();
        //pinRD.Init();
        GPIOD->MODER &= 0xfffff0ffU;
        GPIOD->MODER |= 0x00000500U;
        GPIOD->BSRR = (GPIO_PIN_5 | GPIO_PIN_4);

        // Конфигурируем ШД на чтение

        GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3

        GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7
    }
    
    //pinRD.SetActive();
    GPIOD->BSRR = GPIO_PIN_4 << 16;

    //pinCS.SetActive();
    GPIOG->BSRR = GPIO_PIN_12 << 16;
    
    //pinReadyPAN.WaitPassive();
    //while(pinReadyPAN.IsActive())
    while((GPIOA->IDR & GPIO_PIN_7) == 0)
    {
    }
    
    //                                                 4,5,6,7              2,3                          0,1
    uint8 data = static_cast<uint8>((GPIOE->IDR >> 3) & 0xF0 | (GPIOD->IDR << 2) & 0x0C | (GPIOD->IDR >> 14));

    DDecoder::AddData(data);
    
    //pinRD.SetPassive();
    GPIOD->BSRR = GPIO_PIN_4;

    //pinCS.SetPassive();
    GPIOG->BSRR = GPIO_PIN_12;
    
    interactionWithPanel = false;

    return true;
}


void HAL_FSMC::SendToPanel(uint8 byte)
{
    SendToPanel(&byte, 1);
}


void HAL_FSMC::SendToPanel(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    
    SendToPanel(buffer, 2);
}


void HAL_FSMC::SendToPanel(uint8 *data, uint size)
{
    interactionWithPanel = true;

    if(mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        //pinWR.Init();
        //pinRD.Init();
        GPIOD->MODER &= 0xfffff0ffU;
        GPIOD->MODER |= 0x00000500U;
        GPIOD->BSRR = (GPIO_PIN_5 | GPIO_PIN_4);

        // Конфигурируем ШД на запись

        GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP

        GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP
    }

    for(uint i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //DataBus::Write(byte);               // Выставляем данные на шину
        //                                                                             биты 0,1                                 биты 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // Записываем данные в выходные пины
        //                                                                          Биты 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

        //pinWR.SetActive();                  // Даём сигнал записи
        // HAL_PIO::Reset(PIN_WR);
        //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
        GPIOD->BSRR = GPIO_PIN_5 << 16;

        //while(pinReadyPAN.IsPassive()) {}   // И ожидаем сигнал панели о том, что она свободна
        //while(HAL_PIO::Read(PIN_PAN_READY) == 1) {}
        //while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET) {}
        while(GPIOA->IDR & GPIO_PIN_7)
        {
        }

        //pinCS.SetActive();                  // Даём признак того, чта данные выставлены и можно их считывать
        //HAL_PIO::Reset(PIN_CS);
        //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
        GPIOG->BSRR = GPIO_PIN_12 << 16;

        //while(pinReadyPAN.IsActive()) {}    // Переключение PIN_PAN_READY в неактивное состояние означает, что панель приняла данные и обрабатывает их
        //while(HAL_PIO::Read(PIN_PAN_READY) == 0) {}
        //while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET) {}
        while((GPIOA->IDR & GPIO_PIN_7) == 0)
        {
        }

        //pinWR.SetPassive();                 // \ Устанавливаем WR и CS в неактивное состояние - элементарный цикл записи окончен
        //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
        GPIOD->BSRR = GPIO_PIN_5;

        //pinCS.SetPassive();                 // /
        //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
        GPIOG->BSRR = GPIO_PIN_12;
    }

    interactionWithPanel = false;
}


bool HAL_FSMC::InteractionWithPanel()
{
    return interactionWithPanel;
}


void DataBus::Init()
{
    // Конфигурируем ШД на чтение

    GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3

    GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7
}
