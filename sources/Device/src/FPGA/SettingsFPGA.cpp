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

    HAL_PIO::Write(PIN_LF3, _GET_BIT(datas[set.trig.input][set.trig.source], 2)); //-V525
    HAL_PIO::Write(PIN_A0S, _GET_BIT(datas[set.trig.input][set.trig.source], 1));
    HAL_PIO::Write(PIN_LFS, _GET_BIT(datas[set.trig.input][set.trig.source], 0));
}


static bool NeedLoadRShift(Chan::E ch)
{
    bool result = true;

    static Range::E prevRanges[Chan::Count] = { Range::Count, Range::Count };
    static int16 prevShift[Chan::Count] = { -1000, -1000 };

    if((prevShift[ch] == RShift(ch)) && (prevRanges[ch] == set.ch[ch].range))
    {
        result = false;
    }

    prevRanges[ch] = set.ch[ch].range;
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

    shift += setNRST.exShift[ch][set.ch[ch].range];

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


static int GetK()
{
    return (-TShift::Min()) % TBase::DeltaPoint();
}


void TShift::LoadRandomize()
{
    int k = TBase::DeltaPoint();

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

    if(set.dbg.showRandPredPost)
    {
        LOG_WRITE("pred = %d, post = %d", FPGA::pred, FPGA::post);
    }

    FPGA::post = static_cast<uint16>(~(FPGA::post + 5));
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


// Ограничить range для режима тестер-компонента
static void LimitForTester(Range::E *range)
{
    if(*range > Range::_5V)
    {
        *range = Range::_5V;
    }
    else if(*range < Range::_200mV)
    {
        *range = Range::_200mV;
    }
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

    if(Device::InModeTester())
    {
        LimitForTester(&set.ch[ch].range);
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

    return m[ENumPointsFPGA()].m[set.time.tPos];
}


int TShift::Max()
{
    return 60000;
}


String TShift::ToString(const TBase::E _base)
{
    TBase::E tBase = _base;

    if(tBase == TBase::Count)
    {
        tBase = set.time.base;
    }

    return Time(TShift::ToAbs(set.time.shift, tBase)).ToString(true);
}


String RShift::ToString(int16 rShiftRel, Range::E range, int8 _divider)
{
    float rShiftVal = ToAbs(rShiftRel, range) * Divider::ToAbs(static_cast<Divider::E>(_divider));
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

    return names[set.time.base];
}


pString Range::Name(Chan::E ch)
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

    return names[set.ch[ch].range].name;
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
