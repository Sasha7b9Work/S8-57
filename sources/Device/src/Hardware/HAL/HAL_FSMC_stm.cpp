#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stm32f4xx_hal.h>
#include <cstdlib>
#include <cstring>


#define ADDR_ALTERA1    ((uint8 *)NOR_MEMORY_ADRESS1)
//#define ADDR_ALTERA2    ((uint8 *)NOR_MEMORY_ADRESS2)
//#define ADDR_ALTERA3    ((uint8 *)NOR_MEMORY_ADRESS3)
//#define ADDR_DISPLAY    ((uint8 *)NOR_MEMORY_ADRESS4)


uint8 *HAL_BUS::addrData0 = nullptr;
uint8 *HAL_BUS::addrData1 = nullptr;


HAL_BUS::Mode::E HAL_BUS::mode = HAL_BUS::Mode::FPGA;


void HAL_BUS::Init()
{
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef is =
    {//     D2           D3           D0            D1
        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    HAL_GPIO_Init(GPIOD, &is);

    is.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &is);

    // Настроим адресные выводы для ПЛИС

    //               A0           A1           A2           A3           A4           A5
    //isGPIO.Pin = GPIO _PIN_0 | GPIO _PIN_1 | GPIO _PIN_2 | GPIO _PIN_3 | GPIO _PIN_4 | GPIO _PIN_5;
    //HAL_GPIO_Init(GPIOF, &isGPIO);

    GPIOF->AFR[0] &= HEX_FROM_2(ff00, 0000);
    GPIOF->AFR[0] |= HEX_FROM_2(00cc, cccc);    // Устанавливаем GPIO_AF12_FMC

    GPIOF->MODER &= HEX_FROM_2(ffff, f000);
    GPIOF->MODER |= HEX_FROM_2(0000, 0aaa);     // Устанавливаем Alternate function mode

    GPIOF->OSPEEDR |= HEX_FROM_2(0000, 0fff);   // Устанавливаем very high speed

    GPIOF->OTYPER &= HEX_FROM_2(ffff, c000);    // Устанавливаем output push-pull

    GPIOF->PUPDR &= HEX_FROM_2(ffff, f000);
    GPIOF->PUPDR |= HEX_FROM_2(0000, 0aaa);     // Устанавливаем pull-down


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
        4,                 // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
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

    InitPanel();

    InitRAM();
}


void HAL_BUS::InitRAM()
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitTypeDef is =
    {//     A16           A17           A18 
        GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    HAL_GPIO_Init(GPIOD, &is);

    //           A10          A11         A12           A13          A14         A15           NE3
    is.Pin =  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10;

    HAL_GPIO_Init(GPIOG, &is);

    //           A6            A7          A8            A9
    is.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

    HAL_GPIO_Init(GPIOF, &is);

//    HAL_PIO::Init(HPort::_G, HPin::_10, )

    static SRAM_HandleTypeDef gSramHandle =
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
        3,                 // FSMC_DataSetupTime   При значении 9 32кБ записываются в RAM за 1000мкс. Уменьшение
                           // на одну единцу уменьшает этот параметр на 90 мкс. Если 3 - 32кБ запишутся за 460 мкс.
        0,                 // FSMC_BusTurnAroundDuration
        0,                 // FSMC_CLKDivision
        0,                 // FSMC_DataLatency
        FMC_ACCESS_MODE_C  // FSMC_AccessMode
    };

    FMC_NORSRAM_TimingTypeDef *timing = const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming);

    HAL_SRAM_Init(&gSramHandle, timing, timing);
}


void HAL_BUS::ConfigureToFSMC()
{
    if(mode == Mode::FPGA)
    {
        return;
    }

    mode = Mode::FPGA;

    static const GPIO_InitTypeDef isGPIO =
    {   //    NOE          NWE          NE1
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };
    
    /// \todo Здесь не довеедно - не хотит, почему-то
    
    HAL_GPIO_Init(GPIOD, const_cast<GPIO_InitTypeDef *>(&isGPIO));

    // Инициализируем GPIOD 0, 1, 4, 5, 7, 14, 15 - D2, D3, NOE, NWE, NE1, D0, D1

    // Инициализируем GPIOD 0, 1, 4, 5 - D2, D3, NOE, NWE
    GPIOD->AFR[0] &= HEX_FROM_2(ffff, ff00);
    GPIOD->AFR[0] |= HEX_FROM_2(0000, 00cc);

    GPIOD->MODER &= HEX_FROM_2(ffff, fff0);
    GPIOD->MODER |= HEX_FROM_2(0000, 000a);

    // Инициализируем GPIOD 14, 15 - D0, D1
    GPIOD->AFR[1] &= HEX_FROM_2(00ff, ffff);    // PIN_14, PIN_15
    GPIOD->AFR[1] |= HEX_FROM_2(cc00, 0000);    // GPIO_AF12_FMC

    GPIOD->MODER &= HEX_FROM_2(0fff, ffff);
    GPIOD->MODER |= HEX_FROM_2(a000, 0000);     // Alternate function mode

    // Инициализируем GPIOE 7...8 - D4...D7
    GPIOE->AFR[0] &= HEX_FROM_2(0fff, ffff);
    GPIOE->AFR[0] |= HEX_FROM_2(c000, 0000);
    GPIOE->AFR[1] &= HEX_FROM_2(ffff, f000);
    GPIOE->AFR[1] |= HEX_FROM_2(0000, 0ccc);    // GPIO_AF12_FMC

    GPIOE->MODER &= HEX_FROM_2(ffc0, 3fff);
    GPIOE->MODER |= HEX_FROM_2(002a, 8fff);     // Alternate function mode
}


void HAL_BUS::WriteToFPGA16(uint8 *address, uint16 value)
{
    if(mode != Mode::FPGA)
    {
        ConfigureToFSMC();
    }

    PAUSE_ON_TICKS(100);    /// \todo Без этой строки замедлен вывод при включённой оптимизации и TBase >= 0.5мс

    *address = static_cast<uint8>(value);
    *(address + 1) = static_cast<uint8>(value >> 8);
}


void HAL_BUS::WriteToFPGA8(uint8 *address, uint8 value)
{
    if(mode != Mode::FPGA)
    {
        ConfigureToFSMC();
    }

    *address = value;
}


uint8 HAL_BUS::ReadFromFPGA(const uint8 *address)
{
    if(mode != Mode::FPGA)
    {
        ConfigureToFSMC();
    }

    return *address;
}


void HAL_BUS::SetAddrData(uint8 *address0, uint8 *address1)
{
    addrData0 = address0;
    addrData1 = address1;

    if(mode != Mode::FPGA)
    {
        ConfigureToFSMC();
    }
}


uint8 HAL_BUS::ReadData0()
{
    int delta = VALUE::AVE - static_cast<int>(*addrData0);

    uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData0)));

    Math::Limitation(&result, VALUE::MIN, VALUE::MAX);

    return result;
}


uint8 HAL_BUS::ReadData1()
{
    int delta = VALUE::AVE - static_cast<int>(*addrData1);

    uint8 result = static_cast<uint8>(VALUE::AVE - static_cast<int>(delta * GetStretch(addrData1)));

    Math::Limitation(&result, VALUE::MIN, VALUE::MAX);

    return result;
}


float HAL_BUS::GetStretch(const uint8 *address)
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


void HAL_BUS::RAM::Read(uint8 *buffer, uint size, uint8 *address)
{
    if(mode != Mode::FPGA)
    {
        ConfigureToFSMC();
    }

    std::memcpy(buffer, address, size);
}


float HAL_BUS::RAM::Test1()
{
    if(mode != Mode::FPGA)
    {
        ConfigureToFSMC();
    }

    int SIZE = 1024 * 512;

    int bad = 0;

    uint8 *address = reinterpret_cast<uint8 *>(NOR_MEMORY_ADRESS3);

    for(int i = 0; i < SIZE; i++)
    {
        *address = static_cast<uint8>(i);

        uint8 out = *address;

        if(out != static_cast<uint8>(i))
        {
            bad++;
        }

        address++;
    }

    return bad * 100.0F / SIZE;
}


float HAL_BUS::RAM::Test2()
{
#define SIZE 1024

    int bad = 0;

    uint8 bufferIN[SIZE];
    uint8 bufferOUT[SIZE];
    std::memset(bufferOUT, 0, SIZE);

    for(int x = 0; x < SIZE; x++)
    {
        bufferIN[x] = static_cast<uint8>(std::rand());
    }

    uint8 *address = ExtRAM::Begin() + (std::rand() % (500 * 1024));

    ExtRAM::Write(bufferIN, SIZE, address);

    Read(bufferOUT, SIZE, address);

    for(int z = 0; z < SIZE; z++)
    {
        volatile uint8 in = bufferIN[z];
        volatile uint8 out = bufferOUT[z];

        if(in != out)
        {
            bad++;
        }
    }

    return (bad * 100.0F) / SIZE;
}


float HAL_BUS::RAM::TestTime(uint sizekB)
{
    float result = 0;

    for(uint i = 0; i < sizekB; i++)
    {
        float time = TestTime1kB(ExtRAM::Begin() + i * 1024);

        if(time == -1.0F)
        {
            return -1.0F;
        }

        result += time;
    }

    return result;
}


float HAL_BUS::TestTime1kB(uint8 *address)
{
#define SIZE_BUFFER 1024

    uint8 data[SIZE_BUFFER];
    uint8 out[SIZE_BUFFER];

    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        data[i] = static_cast<uint8>(std::rand());
    }

    uint start = Timer::TimeUS();

    ExtRAM::Write(data, SIZE_BUFFER, address);

    RAM::Read(out, SIZE_BUFFER, address);

    float time = (Timer::TimeUS() - start) / 1e6F;

    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        if(data[i] != out[i])
        {
            return -1.0F;
        }
    }

    return time;
}
