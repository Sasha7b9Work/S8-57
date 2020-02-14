#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Math.h"
#include <stm32f4xx_hal.h>


#define ADDR_ALTERA1    ((uint8 *)NOR_MEMORY_ADRESS1)
//#define ADDR_ALTERA2    ((uint8 *)NOR_MEMORY_ADRESS2)
//#define ADDR_ALTERA3    ((uint8 *)NOR_MEMORY_ADRESS3)
//#define ADDR_DISPLAY    ((uint8 *)NOR_MEMORY_ADRESS4)


uint8 *HAL_BUS::FPGA::addrData0 = nullptr;
uint8 *HAL_BUS::FPGA::addrData1 = nullptr;


void HAL_BUS::Init()
{
    __HAL_RCC_FMC_CLK_ENABLE();

    static GPIO_InitTypeDef is =
    {
        GPIO_PIN_0,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    is.Pin = GPIO_PIN_7;        // NE1 - FPGA
    HAL_GPIO_Init(GPIOD, &is);

    is.Pin = GPIO_PIN_10 |      // NE3 - RAM
             GPIO_PIN_12;       // NE4 - PANEL
    HAL_GPIO_Init(GPIOG, &is);

    //           NOE          NWE          D0            D1            D3           D2    
    is.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_1 | GPIO_PIN_0;
    HAL_GPIO_Init(GPIOD, &is);

    //           D4           D5           D6           D7
    is.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &is);

    //           A0           A1           A2           A3           A4           A5           A6            A7            A8            A9
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &is);

    //           A10          A11         A12           A13          A14         A15
    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOG, &is);

    //           A16           A17           A18
    is.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOD, &is);

    InitFPGA();

    InitPANEL();

    InitRAM();
}


void HAL_BUS::InitFPGA()
{
    static SRAM_HandleTypeDef gSramHandle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK1,                 // Init.NSBank
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

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        6,                 // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
                           // на одну единцу уменьшает этот параметр на 90 мкс. Если 3 - 32кБ запишутся за 460 мкс.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    SRAM_HandleTypeDef *hsram = &gSramHandle;
    FMC_NORSRAM_TimingTypeDef *Timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming);

    FMC_NORSRAM_Init(hsram->Instance, &(hsram->Init));

    FMC_NORSRAM_Timing_Init(hsram->Instance, Timing, hsram->Init.NSBank);

    hsram->Extended->BWTR[hsram->Init.NSBank] = 0x0FFFFFFFU;

    __FMC_NORSRAM_ENABLE(hsram->Instance, hsram->Init.NSBank);
}


void HAL_BUS::InitRAM()
{
    static SRAM_HandleTypeDef handleSRAM =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK3,                 // Init.NSBank
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

    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        0,                 // FSMC_AddressSetupTime
        0,                 // FSMC_AddressHoldTime
        6,                 // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
                           // на одну единцу уменьшает этот параметр на 90 мкс. Если 3 - 32кБ запишутся за 460 мкс.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    FMC_NORSRAM_TimingTypeDef *timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming);

    HAL_SRAM_Init(&handleSRAM, timing, timing);
}


void HAL_BUS::FPGA::Write16(uint8 *address, uint16 value)
{
    PAUSE_ON_TICKS(100);    /// \todo Без этой строки замедлен вывод при включённой оптимизации и TBase >= 0.5мс

    *address = static_cast<uint8>(value);
    *(address + 1) = static_cast<uint8>(value >> 8);
}


void HAL_BUS::FPGA::Write8(uint8 *address, uint8 value)
{
    *address = value;
}


uint8 HAL_BUS::FPGA::Read(const uint8 *address)
{
    return *address;
}


void HAL_BUS::FPGA::SetAddrData(uint8 *address0, uint8 *address1)
{
    addrData0 = address0;
    addrData1 = address1;
}


uint8 HAL_BUS::FPGA::ReadA0()
{
    int delta = VALUE::AVE - static_cast<int>(*addrData0);

    uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData0)));

    Math::Limitation(&result, VALUE::MIN, VALUE::MAX);

    return result;
}


uint8 HAL_BUS::FPGA::ReadA1()
{
    int delta = VALUE::AVE - static_cast<int>(*addrData1);

    uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData1)));

    Math::Limitation(&result, VALUE::MIN, VALUE::MAX);

    return result;
}


float HAL_BUS::FPGA::GetStretch(const uint8 *address)
{
    if (StretchADC::IsDisabled())
    {
        return 1.0F;
    }

    static const float *stretchs[4] =
    {
        &StretchADC::Ref().stretch[Chan::A],
        &StretchADC::Ref().stretch[Chan::A],
        &StretchADC::Ref().stretch[Chan::B],
        &StretchADC::Ref().stretch[Chan::B]
    };

    int delta = address - RD::DATA_A;

    float result = *stretchs[delta];

    return result;
}
