#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


TrigStartMode::E &TrigStartMode::Ref()
{
    return set.trig.startMode;
}


void TrigStartMode::Set(TrigStartMode::E v)
{
    set.trig.startMode = v;
    Osci::ChangedTrigStartMode();
}


void TrigSource::Set(Chan::E ch)
{
    if (ch != Chan::Count)
    {
        set.trig.source = ch;
    }

    TrigInput().Load();
}


Chan::E &TrigSource::Ref()
{
    return set.trig.source;
}


void TrigInput::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // ÏÑ 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // Â×
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // Í×
    };

    HAL_PIO::Write(PIN_LF3, _GET_BIT(datas[TrigInput()][TrigSource()], 2));
    HAL_PIO::Write(PIN_A0S, _GET_BIT(datas[TrigInput()][TrigSource()], 1));
    HAL_PIO::Write(PIN_LFS, _GET_BIT(datas[TrigInput()][TrigSource()], 0));
}


TrigInput::E &TrigInput::Ref()
{
    return set.trig.input;
}


static bool NeedLoadRShift(Chan::E ch)
{
    bool result = true;

    static Range::E prevRanges[Chan::Count] = { Range::Count, Range::Count };
    static int16 prevShift[Chan::Count] = { -1000, -1000 };

    if((prevShift[ch] == RShift(ch)) && (prevRanges[ch] == Range(ch)))
    {
        result = false;
    }

    prevRanges[ch] = Range(ch);
    prevShift[ch] = RShift(ch);

    return result;
}


void RShift::Load(bool force)
{
    if(!force && !NeedLoadRShift(ch))
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    int16 shift = RShift(ch) + HARDWARE_ZERO;

    shift += ExtraShift::GetValue(ch, Range(ch));

    if (Chan(ch).IsA() && Device::InModeTester())
    {
        shift -= Tester::DeltaRShiftA();
    }

    Osci::InputController::Write(PIN_SPI3_CS1, static_cast<uint16>(mask[ch] | (shift << 2)));

    Osci::Restart();

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        DataSettings *ds = RAM::Get();

        if(ds)
        {
            ds->Fill();
        }
    }
}


void FPGA::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}


void TShift::LoadReal()
{
    FPGA::post = static_cast<uint16>(set.time.shift - TShift().Min());
    int Pred = static_cast<int>(ENumPointsFPGA::PointsInChannel()) - static_cast<int>(FPGA::post);

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = static_cast<uint16>(Pred);

    FPGA::post = static_cast<uint16>(~(FPGA::post + 1));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred + 3));

    HAL_BUS::FPGA::Write16(WR::PRED_LO, FPGA::post);
    HAL_BUS::FPGA::Write16(WR::POST_LO, FPGA::pred);
}


static uint GetK()
{
    return (-TShift().Min()) % TBase().RandK();
}


void TShift::LoadRandomize()
{
    uint k = TBase().RandK();

    FPGA::post = static_cast<uint16>((set.time.shift - TShift().Min() - GetK()) / k);

    if(set.time.shift - static_cast<int>(GetK()) < TShift().Min())
    {
        FPGA::post = 0;
    }

    int Pred = static_cast<int>(ENumPointsFPGA::PointsInChannel()) / static_cast<int>(k) - static_cast<int>(FPGA::post);

    if (Pred < 5)
    {
        Pred = 5;
    }

    FPGA::pred = static_cast<uint16>(Pred);

//    LOG_WRITE("pred = %d, post = %d", FPGA::pred, FPGA::post + 1);

    FPGA::post = static_cast<uint16>(~(FPGA::post + 1));
    FPGA::pred = static_cast<uint16>(~(FPGA::pred));

    HAL_BUS::FPGA::Write16(WR::PRED_LO, FPGA::pred);
    HAL_BUS::FPGA::Write16(WR::POST_LO, FPGA::post);

    Osci::addShift = static_cast<int>(set.time.shift % k);

    if (Osci::addShift < 0)
    {
        Osci::addShift = static_cast<int>(k + Osci::addShift);
    }
}


void TShift::Load()
{
    if (OSCI_IN_MODE_RANDOMIZER)
    {
        LoadRandomize();
    }
    else
    {
        LoadReal();
    }
}


void TShift::Change(const int delta)
{
    if (Device::InModeRecorder())
    {
        return;
    }

    TShift::Set(set.time.shift + delta);
}


void Range::Change(int16 delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    set.disp.SetLastAffectedChannel(ch);

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&Range::Ref(ch)), static_cast<uint8>(Range::Count - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&Range::Ref(ch)), 0);  // -V206
    }
    Range::LoadBoth();

    DisplayOsci::SetFlagRedraw();
}


void RShift::Set(int16 rShift)
{
    ::Math::Limitation(&rShift, MIN, MAX);
    set.ch[ch].rShift = rShift;
    Load();
}

RShift::operator int16()
{
    return set.ch[ch].rShift;
}


void RShift::Change(int16 delta)
{
    ::Math::AdditionThisLimitation<int16>(&set.ch[ch].rShift, STEP * delta, MIN, MAX);

    RShift(ch).Load();
}


void TrigPolarity::Load()
{
    FPGA::ForcedStart();
}


TrigPolarity::E &TrigPolarity::Ref()
{
    return set.trig.polarity;
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

    return m[ENumPointsFPGA()].m[TPos()];
}


int TShift::Max()
{
    return 60000;
}


String TShift::ToString(const TBase::E _base) const
{
    TBase::E tBase = _base;

    if(tBase == TBase::Count)
    {
        tBase = TBase();
    }

    return Time(TShift::ToAbs(set.time.shift, tBase)).ToString(true);
}


String RShift::ToString(int16 rShiftRel, Range::E range, int8 _divider)
{
    float rShiftVal = ToAbs(rShiftRel, range) * Divider::ToAbs(static_cast<Divider::E>(_divider));
    return Voltage(rShiftVal).ToString(true);
}


TBase::E &TBase::Ref()
{
    return set.time.base;
}


pString TBase::Name() const
{
    static pString names[TBase::Count] =
    {
        "2íñ",
        "5íñ",
        "10íñ",
        "20íñ",
        "50íñ",
        "0.1ìêñ",
        "0.2ìêñ",
        "0.5ìêñ",
        "1ìêñ",
        "2ìêñ",
        "5ìêñ",
        "10ìêñ",
        "20ìêñ",
        "50ìêñ",
        "0.1ìñ",
        "0.2ìñ",
        "0.5ìñ",
        "1ìñ",
        "2ìñ",
        "5ìñ",
        "10ìñ",
        "20ìñ",
        "50ìñ",
        "0.1ñ",
        "0.2ñ",
        "0.5ñ",
        "1ñ",
        "2ñ",
        "5ñ",
        "10ñ"
    };

    return names[TBase()];
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
        StructRange("2ìÂ"),
        StructRange("5ìÂ"),
        StructRange("10ìÂ"),
        StructRange("20ìÂ"),
        StructRange("50ìÂ"),
        StructRange("0.1Â"),
        StructRange("0.2Â"),
        StructRange("0.5Â"),
        StructRange("1Â"),
        StructRange("2Â"),
        StructRange("5Â"),
        StructRange("10Â"),
        StructRange("20Â")
    };

    return names[Ref(ch)].name;
};


pString ModeCouple::UGO() const
{
    static const pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[Ref(ch)];
}


ModeCouple::E &ModeCouple::Ref(Chan::E ch)
{
    return set.ch[ch].couple;
}


Bandwidth::E &Bandwidth::Ref(Chan::E ch)
{
    return set.ch[ch].bandwidth;
}


void Bandwidth::Load()
{
    static const StructPIN pinsLF[2] = { {PIN_LF1}, {PIN_LF2} };

    HAL_PIO::Write(pinsLF[ch], Bandwidth(ch).Is20MHz());
}
