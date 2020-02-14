#include "defines.h"
#include "common/Decoder_d.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


#define ADDR_PANEL ((uint8 *)NOR_MEMORY_ADRESS4)


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


/// Активным состоянием этого пина панель сообщает о готовности к взаимодействию
static InPin pinReadyWR(PIN_READY_WR);
/// Активное состояние этого пина сообщает о том, что панель имеет данные для пеередчи
static InPin pinReadyRD(PIN_READY_RD);

/// true означает, что шина находится в процессе обмена с панелью и запись по обычной FSMC в альтеру и память запрещена
static bool interactionWithPanel = false;

void HAL_BUS::InitPANEL()
{
    pinReadyWR.Init();
    pinReadyRD.Init();

    static SRAM_HandleTypeDef handle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK4,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_DISABLE,      // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE            // Init.WriteBurst
        }
    };

    static const FMC_NORSRAM_TimingTypeDef timing =
    {
        15,                // FSMC_AddressSetupTime
        15,                // FSMC_AddressHoldTime
        255,               // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
                           // на одну единцу уменьшает этот параметр на 90 мкс. Если 3 - 32кБ запишутся за 460 мкс.
        15,                // FSMC_BusTurnAroundDuration
        2,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    FMC_NORSRAM_TimingTypeDef *tim = const_cast<FMC_NORSRAM_TimingTypeDef *>(&timing);

    HAL_SRAM_Init(&handle, tim, tim);
}


void HAL_BUS::PANEL::Receive()
{
    interactionWithPanel = true;

    while(pinReadyRD.IsActive())
    {
        DDecoder::AddData(*ADDR_PANEL);
    }
   
    interactionWithPanel = false;
}


void HAL_BUS::PANEL::Send(uint8 byte)
{
    Send(&byte, 1);
}


void HAL_BUS::PANEL::Send(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    
    Send(buffer, 2);
}

void HAL_BUS::PANEL::Send(uint8 *data, uint size)
{
    interactionWithPanel = true;

    while(size)
    {
        while(pinReadyWR.IsPassive())
        {
            if(pinReadyRD.IsActive())
            {
                DDecoder::AddData(*ADDR_PANEL);
            }
        }

        *ADDR_PANEL = *data++;

        size--;
    }

    interactionWithPanel = false;
}


bool HAL_BUS::PANEL::InInteraction()
{
    return interactionWithPanel;
}
