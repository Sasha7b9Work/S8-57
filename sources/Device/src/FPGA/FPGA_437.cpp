#include "defines.h"
#include <stm32f4xx.h>
#include "FPGA.h"
#include "AD9286.h"
#include "Data/DataStorage.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    givingStart = false;

    GPIO_Init();

    SetPin(Pin::SPI3_CS1);
    SetPin(Pin::SPI3_CS2);

    ResetPin(Pin::SPI3_SCK);
    ResetPin(Pin::SPI3_DAT);

    AD9286::Init();

    // Теперь настроим вход АЦП от рандомизатора
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // Режим работы:
    // - измерение по 1 регулярному каналу
    // - одиночное измерение по фронту внешнего запуска (прерывание от PB11)

    __ADC3_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_10,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOF, &isGPIOadc);

    static GPIO_InitTypeDef isGPIOexti =
    {
        GPIO_PIN_11,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOB, &isGPIOexti);

    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    handleADC.Instance = ADC3;
    handleADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handleADC.Init.Resolution = ADC_RESOLUTION12b;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.ScanConvMode = DISABLE;
    handleADC.Init.EOCSelection = ENABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DMAContinuousRequests = DISABLE;
    handleADC.Init.NbrOfConversion = 1;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handleADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    if (HAL_ADC_Init(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start_IT(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    DataStorage::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadSettings()
{
    LoadRanges();
    LoadRShift(Chan::A);
    LoadRShift(Chan::B);
    LoadTrigSourceInput();
    LoadTrigLev();
    LoadTBase();
    LoadTShift();

    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRShift(Chan ch)
{
    LAST_AFFECTED_CH = ch;

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    WriteRegisters(Pin::SPI3_CS1, (uint16)(mask[ch] | (SET_RSHIFT(ch) << 2)));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRanges()
{
    uint16 value = (uint16)(ValueForRange(Chan::B) + (ValueForRange(Chan::A) << 8));

    WriteRegisters(Pin::SPI3_CS2, value);

    PAUSE_ON_MS(10);                // Задержка нужна, чтобы импульсные реле успели отработать

    WriteRegisters(Pin::SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

    static struct StructRange
    {
        uint8 value;
        StructRange(uint8 v) : value(v) {};
    } vals[Range::Number] =
    {
        StructRange(BIN_U8(00000000)),  // 2mV
        StructRange(BIN_U8(00000001)),  // 5mV
        StructRange(BIN_U8(00000010)),  // 10mV
        StructRange(BIN_U8(00000011)),  // 20mV
        StructRange(BIN_U8(00000001)),  // 50mV
        StructRange(BIN_U8(00000010)),  // 100mV
        StructRange(BIN_U8(00000011)),  // 200mV
        StructRange(BIN_U8(00000001)),  // 500mV
        StructRange(BIN_U8(00000010)),  // 1V
        StructRange(BIN_U8(00000011)),  // 2V
        StructRange(BIN_U8(00000001)),  // 5V
        StructRange(BIN_U8(00000010)),  // 10V
        StructRange(BIN_U8(00000011))   // 20V
    };

    uint8 valueA = vals[SET_RANGE_A].value;

    WritePin(Pin::A1, _GET_BIT(valueA, 1));
    WritePin(Pin::A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[SET_RANGE_B].value;

    WritePin(Pin::A3, _GET_BIT(valueB, 1));
    WritePin(Pin::A4, _GET_BIT(valueB, 0));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    Buffer dataA(FPGA_NUM_POINTS);
    Buffer dataB(FPGA_NUM_POINTS);

    ReadDataChanenl(Chan::A, dataA.Data());
    ReadDataChanenl(Chan::B, dataB.Data());

    DataSettings ds;
    ds.Fill(dataA.Data(), dataB.Data());

    DataStorage::Push(&ds);
}
