#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Display/DisplayOsci.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


const float TShift::absStep[TBase::Count] =
{
    /* 2 нс    */ 2e-9F / 20,
    /* 5 нс    */ 5e-9F / 20,
    /* 10 нс   */ 10e-9F / 20,
    /* 20 нс   */ 20e-9F / 20,
    /* 50 нс   */ 50e-9F / 20,   // 1.0  Это коэффициенты для реализации алгоритма прореживания отсчётов
    /* 100 нс  */ 100e-9F / 20,   // 2.0
    /* 200 нс  */ 200e-9F / 20,   // 4.0
    /* 500 нс  */ 500e-9F / 20,   // 10.0
    /* 1 мкс   */ 1e-6F / 20,   // 20.0
    /* 2 мкс   */ 2e-6F / 20,   // 40.0
    /* 5 мкс   */ 5e-6F / 20,   // 100.0
    /* 10 мкс  */ 10e-6F / 20,   // 200.0
    /* 20 мкс  */ 20e-6F / 20,   // 400.0
    /* 50 мкс  */ 50e-6F / 20,   // 1e3
    /* 100 мкс */ 100e-6F / 20,   // 2e3
    /* 200 мкс */ 200e-6F / 20,   // 4e3
    /* 500 мкс */ 500e-6F / 20,   // 10e3
    /* 1 мс    */ 1e-3F / 20,   // 20e3
    /* 2 мс    */ 2e-3F / 20,   // 40e3
    /* 5 мс    */ 5e-3F / 20,   // 100e3
    /* 10 мс   */ 10e-3F / 20,   // 200e3
    /* 20 мс   */ 20e-3F / 20,   // 400e3
    /* 50 мс   */ 50e-3F / 20,   // 1e4
    /* 100 мс  */ 100e-3F / 20,   // 2e4
    /* 200 мс  */ 200e-3F / 20,   // 4e4
    /* 500 мс  */ 500e-3F / 20,   // 10e4
    /* 1 с     */ 1.0F / 20,   // 20e4
    /* 2 с     */ 2.0F / 20,   // 40e4
    /* 5 с     */ 5.0F / 20,   // 100e4
    /* 10 с    */ 10.0F / 20    // 200e4
};


void TrigSource::Load()
{
    TrigInput::Load();
}


void TrigInput::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // ПС 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ВЧ
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // НЧ
    };

    GPIO::WritePin(FPin::A1S, _GET_BIT(datas[static_cast<int>(set.trig.input)][static_cast<int>(set.trig.source)], 2));
    GPIO::WritePin(FPin::A0S, _GET_BIT(datas[static_cast<int>(set.trig.input)][static_cast<int>(set.trig.source)], 1));
    GPIO::WritePin(FPin::LFS, _GET_BIT(datas[static_cast<int>(set.trig.input)][static_cast<int>(set.trig.source)], 0));
}


static bool NeedLoadRShift(Chan::E ch)
{
    bool result = true;

    static Range::E prevRanges[Chan::Count] = { Range::Count, Range::Count };
    static int16 prevShift[Chan::Count] = { -1000, -1000 };

    if((prevShift[ch] == RShift::Value(ch)) && (prevRanges[ch] == set.ch[ch].range))
    {
        result = false;
    }

    prevRanges[ch] = set.ch[ch].range;
    prevShift[ch] = RShift::Value(ch);

    return result;
}


void RShift::Load(Chan::E ch)
{
    if(!NeedLoadRShift(ch))
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    int16 shift = Value(ch) + HARDWARE_ZERO;

    int8 add = set.dbg.addRShift[static_cast<int>(ch)][static_cast<int>(set.ch[static_cast<int>(ch)].range)];

    shift += add;

    if (Chan(ch).IsA() && Device::State::InModeTester())
    {
        shift -= Tester::DeltaRShiftA();
    }

    GPIO::WriteRegisters(FPin::SPI3_CS1, static_cast<uint16>(mask[static_cast<int>(ch)] | (shift << 2)));

    Osci::Restart();
}


void FPGA::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}


void TShift::LoadReal()
{
    FPGA::post = static_cast<uint16>(set.time.shift - TShift::Min());
    int Pred = static_cast<int>FPGA_NUM_POINTS - static_cast<int>(FPGA::post);

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = static_cast<uint16>(Pred);

    FPGA::post = static_cast<uint16>(~(FPGA::post + 1));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred + 3));

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::post);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, FPGA::pred);
}


static uint GetK()
{
    return (-TShift::Min()) % Osci::Kr[static_cast<int>(set.time.base)];
}


void TShift::LoadRandomize()
{
    uint k = Osci::Kr[static_cast<int>(set.time.base)];

    FPGA::post = static_cast<uint16>((set.time.shift - TShift::Min() - GetK()) / k);

    int Pred = static_cast<int>(FPGA_NUM_POINTS) / static_cast<int>(k) - static_cast<int>(FPGA::post);

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = static_cast<uint16>(Pred);

    FPGA::post = static_cast<uint16>(~(FPGA::post + 1));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred));

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);

    Osci::addShift = static_cast<int>((set.time.shift) % k);

    if (Osci::addShift < 0)
    {
        Osci::addShift = static_cast<int>(k + Osci::addShift);
    }
}


void TShift::Load()
{
    if (Osci::InModeRandomizer())
    {
        LoadRandomize();
    }
    else
    {
        LoadReal();
    }
}


void TShift::Change(int delta)
{
    if (Device::State::InModeRecorder())
    {
        return;
    }

    TShift::Set(set.time.shift + delta);
}


void Range::Change(Chan::E ch, int16 delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&set.ch[ch].range), static_cast<uint8>(Range::Count - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&set.ch[ch].range), 0);  // -V206
    }
    Range::LoadBoth();

    DisplayOsci::SetFlagRedraw();
}


void Range::Set(Chan::E ch, E range)
{
    set.disp.SetLastAffectedChannel(ch);

    set.ch[static_cast<int>(ch)].range = range;

    LoadBoth();
}


void RShift::Change(Chan::E ch, int16 delta)
{
    ::Math::AdditionThisLimitation<int16>(&RShift::Value(ch), STEP * delta, MIN, MAX);

    Load(ch);
}


void RShift::Set(Chan::E ch, int16 rShift)
{
    ::Math::Limitation(&rShift, MIN, MAX);
    Value(ch) = rShift;
    Load(ch);
}


void TrigPolarity::Load()
{
    FPGA::GiveStart();
}


int TShift::Min()
{
#define k 0
#define mul 2

    static const struct StructENumPoints
    {
        int m[3];
        StructENumPoints(int m0, int m1, int m2) { m[0] = m0; m[1] = m1; m[2] = m2; }
    }
    m[ENumPointsFPGA::Count] =
    {
        StructENumPoints(-256 * mul + k,  -128 * mul + k, 0 * mul + k),  // 512
        StructENumPoints(-512 * mul + k,  -256 * mul + k, 0 * mul + k),  // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),  // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),  // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)   // 8192
    };

    return m[static_cast<int>(set.mem.enumPoints)].m[static_cast<int>(set.time.TPos)];
}


int TShift::Zero()
{
    return 0;
}


int TShift::Max()
{
    return 60000;
}

void TShift::Set(int tShift)
{
    LIMITATION(tShift, Min(), Max());

    set.time.shift = tShift;

    TShift::Load();

    Osci::Restart();

    DisplayOsci::SetFlagRedraw();
}


String TShift::ToString(TBase::E tBase)
{
    return Time(TShift::ToAbs(set.time.shift, tBase)).ToString(true);
}


float TShift::ToAbs(int tShift, TBase::E tBase)
{
    return absStep[tBase] * tShift;
}


String RShift::ToString(int16 rShiftRel, Range::E range, int8 _divider)
{
    float rShiftVal = ToAbs(rShiftRel, range) * Divider(static_cast<uint>(_divider)).ToAbs();
    return Voltage(rShiftVal).ToString(true);
}


pString TBase::Name() const
{
    static pString names[TBase::Count] =
    {
        "2нс",
        "5нс",
        "10нс",
        "20нс",
        "50нс",
        "0.1мкс",
        "0.2мкс",
        "0.5мкс",
        "1мкс",
        "2мкс",
        "5мкс",
        "10мкс",
        "20мкс",
        "50мкс",
        "0.1мс",
        "0.2мс",
        "0.5мс",
        "1мс",
        "2мс",
        "5мс",
        "10мс",
        "20мс",
        "50мс",
        "0.1с",
        "0.2с",
        "0.5с",
        "1с",
        "2с",
        "5с",
        "10с"
    };

    return names[static_cast<int>(value)];
}


pString Range::Name() const
{
    static const struct StructRange
    {
        const char * name;
        StructRange(pString nRU)
        {
            name = nRU;
        }
    }
    names[Range::Count] =
    {
        StructRange("2мВ"),
        StructRange("5мВ"),
        StructRange("10мВ"),
        StructRange("20мВ"),
        StructRange("50мВ"),
        StructRange("0.1В"),
        StructRange("0.2В"),
        StructRange("0.5В"),
        StructRange("1В"),
        StructRange("2В"),
        StructRange("5В"),
        StructRange("10В"),
        StructRange("20В")
    };

    return names[static_cast<int>(value)].name;
};


void ModeCouple::Set(Chan::E ch, ModeCouple::E modeCoupe)
{
    set.ch[static_cast<int>(ch)].couple = modeCoupe;
    Range::LoadBoth();
}


pString ModeCouple::UGO() const
{
    static const pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[static_cast<int>(value)];
}


void Bandwidth::Load()
{
    Chan::E ch = GetChannel();
    static const FPin::E pinsLF[2] = { FPin::LF1, FPin::LF2 };

    GPIO::WritePin(pinsLF[static_cast<int>(ch)], (set.ch[static_cast<int>(ch)].bandwidth.value == Bandwidth::_20MHz));
}


Chan::E Bandwidth::GetChannel() const
{
    return (&set.ch[Chan::A].bandwidth.value == &this->value) ? Chan::A : Chan::B;
}
