#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "device.h"
#include "FPGA.h"
#include "FPGA_Settings.h"
#include "FPGA/FPGATypes.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 ValueForRange(Chan::E ch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::SET::Load()
{
    Range::LoadBoth();
    RShift::Load(Chan::A);
    RShift::Load(Chan::B);
    Trig::Input::Load();
    Trig::Level::Load();
    TBase::Load();
    LoadTShift();
    LoadCalibratorMode();
    LoadHoldfOff();

    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Trig::Source::Load()
{
    Input::Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Trig::Input::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // ПС 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ВЧ
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // НЧ
    };

    FPGA::WritePin(Pin::A1S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 2));
    FPGA::WritePin(Pin::A0S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 1));
    FPGA::WritePin(Pin::LFS, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 0));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Range::LoadBoth()
{
    uint16 val = (uint16)(ValueForRange(Chan::B) + (ValueForRange(Chan::A) << 8));

    FPGA::WriteRegisters(Pin::SPI3_CS2, val);

    PAUSE_ON_MS(10);                // Задержка нужна, чтобы импульсные реле успели отработать

    FPGA::WriteRegisters(Pin::SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

    DEF_STRUCT(StructRange, uint8) vals[FPGA::SET::Range::Number] =
    {
        StructRange(BIN_U8(00000000)),  // 2mV      // -V2501
        StructRange(BIN_U8(00000001)),  // 5mV      // -V2501
        StructRange(BIN_U8(00000010)),  // 10mV     // -V2501
        StructRange(BIN_U8(00000011)),  // 20mV     // -V2501
        StructRange(BIN_U8(00000001)),  // 50mV     // -V2501
        StructRange(BIN_U8(00000010)),  // 100mV    // -V2501
        StructRange(BIN_U8(00000011)),  // 200mV    // -V2501
        StructRange(BIN_U8(00000001)),  // 500mV    // -V2501
        StructRange(BIN_U8(00000010)),  // 1V       // -V2501
        StructRange(BIN_U8(00000011)),  // 2V       // -V2501
        StructRange(BIN_U8(00000001)),  // 5V       // -V2501
        StructRange(BIN_U8(00000010)),  // 10V      // -V2501
        StructRange(BIN_U8(00000011))   // 20V      // -V2501
    };

    uint8 valueA = vals[SET_RANGE_A].val;

    FPGA::WritePin(Pin::A1, _GET_BIT(valueA, 1));
    FPGA::WritePin(Pin::A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[SET_RANGE_B].val;

    FPGA::WritePin(Pin::A3, _GET_BIT(valueB, 1));
    FPGA::WritePin(Pin::A4, _GET_BIT(valueB, 0));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::RShift::Load(Chan::E ch)
{
    LAST_AFFECTED_CH = ch;

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    uint16 shift = SET_RSHIFT(ch);

    if (Chan(ch).IsA() && Device::CurrentMode() == Device::Mode::Tester)
    {
        shift = (uint16)((int)shift - Tester::DeltaRShiftA());
    }

    FPGA::WriteRegisters(Pin::SPI3_CS1, (uint16)(mask[ch] | (shift << 2)));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Trig::Level::Load()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::MAX + Trig::MIN) - SET_TRIGLEV_SOURCE);

    FPGA::WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TBase::Load()
{
    static const uint8 values[TBase::Number] =
    {
        BIN_U8(00000000),  // -V2501  // 2ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 5ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 10ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 20ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 50ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 100ns   1       200MHz
        BIN_U8(00100000),  // -V2501  // 200ns   2       100MHz
        BIN_U8(00100001),  // -V2501  // 500ns   5       40MHz
        BIN_U8(00100010),  // -V2501  // 1us     10      20MHz
        BIN_U8(00100011),  // -V2501  // 2us     20      10MHz
        BIN_U8(01000101),  // -V2501  // 5us     50      4MHz
        BIN_U8(01000110),  // -V2501  // 10us    100     2MHz
        BIN_U8(01000111),  // -V2501  // 20us    200     1MHz
        BIN_U8(01001001),  // -V2501  // 50us    500     400kHz
        BIN_U8(01001010),  // -V2501  // 100us   1k      200kHz
        BIN_U8(01001011),  // -V2501  // 200us   2k      100kHz
        BIN_U8(01001101),  // -V2501  // 500us   5k      40kHz
        BIN_U8(01001110),  // -V2501  // 1ms     10k     20kHz
        BIN_U8(01001111),  // -V2501  // 2ms     20k     10kHz
        BIN_U8(01010001),  // -V2501  // 5ms     50k     4kHz
        BIN_U8(01010010),  // -V2501  // 10ms    100k    2kHz
        BIN_U8(01010011),  // -V2501  // 20ms    200k    1kHz
        BIN_U8(01010101),  // -V2501  // 50ms    500k    400Hz
        BIN_U8(01010110),  // -V2501  // 100ms   1M      200Hz
        BIN_U8(01010111),  // -V2501  // 200ms   2M      100Hz
        BIN_U8(01011001),  // -V2501  // 500ms   5M      40Hz
        BIN_U8(01011010),  // -V2501  // 1s      10M     20Hz
        BIN_U8(01011011),  // -V2501  // 2s      20M     10Hz
        BIN_U8(01011101),  // -V2501  // 5s      50M     4Hz
        BIN_U8(01011110)   // -V2501  // 10s     100M    2Hz
    };

    FPGA::ClearDataRand();

    FSMC::WriteToFPGA8(WR_TBASE, values[SET_TBASE]);

    LoadTShift();
}

#ifdef _WIN32
#pragma warning(pop)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::LoadHoldfOff()
{
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_ENABLE, TRIG_HOLDOFF_ENABLED ? 1U : 0U);

    uint value = (uint)(0 - TRIG_HOLDOFF + 1);

    BitSet32 bs(value);

    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_LOW, bs.byte0);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_MID, bs.byte1);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_HI, bs.byte2);
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ValueForRange(Chan::E ch) // -V2506
{
    static const uint8 datas[ModeCouple::Size] =
    {
        BIN_U8(01000001),  // -V2501  // DC
        BIN_U8(10000001),  // -V2501  // AC
        BIN_U8(00000010)   // -V2501  // GND
    };

    if (SET_COUPLE(ch) == ModeCouple::GND && Device::CurrentMode() == Device::Mode::Osci)
    {
        return datas[ModeCouple::GND];
    }

    DEF_STRUCT(StructRange, uint16) values[FPGA::SET::Range::Number][2] =
    {   //             A                    B
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 2mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 5mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 10mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 20mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 50mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 100mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 200mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 500mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 1V
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 2V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 5V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 10V
        { BIN_U8(00011000), BIN_U8(00011000) }  // -V2501  // 20V
    };

    return (uint8)(values[SET_RANGE(ch)][ch].val | datas[SET_COUPLE(ch)]);
}

#ifdef WIN32
#pragma warning(pop)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::LoadTShift()
{
    FPGA::post = (uint16)(SET_TSHIFT - TShift::Min());
    int Pred = (int)FPGA::NumPoints() - (int)FPGA::post;

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred + 3));

    FSMC::WriteToFPGA16(WR_PRED_LO, FPGA::post);
    FSMC::WriteToFPGA16(WR_POST_LO, FPGA::pred);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TShiftChange(int delta)
{
    SetTShift(SET_TSHIFT + delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::SetTShift(int tShift)
{
    SET_TSHIFT.Set(tShift);

    LoadTShift();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Range::Change(Chan::E ch, int delta)
{
    if (delta > 0)
    {
        Math::LimitationIncrease<uint8>((uint8 *)(&SET_RANGE(ch)), (uint8)(Range::Number - 1)); // -V206
    }
    else
    {
        Math::LimitationDecrease<uint8>((uint8 *)(&SET_RANGE(ch)), 0);  // -V206
    }
    Range::LoadBoth();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TBase::Change(int delta)
{
    if (delta > 0)
    {
        Math::LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBase::Number - 1)); // -V206
    }
    else
    {
        if (SET_PEAKDET_EN &&                               // Если вклюён режим пикового детектора
            SET_TBASE == TBase::MIN_PEAK_DET)               // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
        {
            Display::ShowWarning(Warning::EnabledPeakDet);  // выводим сообщение об этом
            return;                                         // и выходим
        }

        Math::LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0); // -V206
    }

    TBase::Load();
    Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::RShift::Change(Chan::E ch, int delta)
{
    Math::AdditionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShift::MIN, RShift::MAX);

    RShift::Load(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Trig::Level::Change(int delta)
{
    Math::AdditionThisLimitation<uint16>(&SET_TRIGLEV_SOURCE, STEP_TRIGLEV * delta, Trig::MIN, Trig::MAX);

    Trig::Level::Load();

    Trig::NeedForDraw(2000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::RShift::Set(Chan::E ch, uint16 rShift)
{
    Math::Limitation<uint16>(&rShift, RShift::MIN, RShift::MAX);
    SET_RSHIFT(ch) = rShift;
    RShift::Load(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::Trig::Polarity::Load()
{
    GiveStart();
}

