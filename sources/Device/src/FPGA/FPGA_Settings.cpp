#include "defines.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Font/Font.h"
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "FPGA_Settings.h"
#include "FPGA_Math.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"

#include "Osci/Display/Osci_Display.h"
#include "Recorder/Recorder.h"


using namespace Display::Primitives;
using namespace FPGA::HAL::GPIO;
using namespace FPGA::Settings;
using namespace HAL::ADDRESSES::FPGA;
using namespace Osci::Settings;
using namespace Osci::Settings::Memory;

using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TShift::value = 0;

namespace Osci
{
    namespace Settings
    {
        namespace Trig
        {
            /// Установленное в true значение означает, что нужно выводить значок синхроимпульса
            bool pulse = false;
            ///< Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации.
            static bool needDraw = false;
            /// Отключает отображение уровня синхронизации поверх сигнала
            static void DisableDrawing();
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Trig::Source::Load()
{
    Input::Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Input::Load()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // ПС 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ВЧ
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // НЧ
    };

    WritePin(Pin::A1S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 2));
    WritePin(Pin::A0S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 1));
    WritePin(Pin::LFS, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 0));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::Load(Chan::E ch)
{
    LAST_AFFECTED_CH = ch;

    static const uint16 mask[2] = { 0x2000, 0x6000 };

    uint16 shift = SET_RSHIFT(ch);

    int8 add = set.addRShift[ch][SET_RANGE(ch)];

    shift += add;

    if (Chan(ch).IsA() && Device::State::InModeTester())
    {
        shift = (uint16)((int)shift - Tester::DeltaRShiftA());
    }

    WriteRegisters(Pin::SPI3_CS1, (uint16)(mask[ch] | (shift << 2)));

    Osci::Restart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Load()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::Level::MAX + Trig::Level::MIN) - SET_TRIGLEV_SOURCE);

    WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));

    Osci::Restart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Settings::LoadCalibratorMode()
{
    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void LoadReal()
{
    FPGA::post = (uint16)(SET_TSHIFT - TShift::Min());
    int Pred = (int)FPGA_NUM_POINTS - (int)FPGA::post;

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred + 3));

    FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::post);
    FSMC::WriteToFPGA16(WR::POST_LO, FPGA::pred);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetK()
{
    return (-TShift::Min()) % Osci::Kr[SET_TBASE];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void LoadRandomize()
{
    int k = Osci::Kr[SET_TBASE];

    FPGA::post = (uint16)((SET_TSHIFT - TShift::Min() - GetK()) / k);

    int Pred = (int)FPGA_NUM_POINTS / k - (int)FPGA::post;

    if (Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred));

    FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);

    Osci::addShift = (SET_TSHIFT) % k;

    if (Osci::addShift < 0)
    {
        Osci::addShift = k + Osci::addShift;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TShift::Change(int delta)
{
    if (Device::State::InModeRecorder())
    {
        return;
    }

    TShift::Set(SET_TSHIFT + delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Range::Change(Chan::E ch, int delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&SET_RANGE(ch)), (uint8)(Range::Size - 1)); // -V206
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&SET_RANGE(ch)), 0);  // -V206
    }
    Range::LoadBoth();

    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Range::Set(Chan::E ch, E range)
{
    SET_RANGE(ch) = range;
    LoadBoth();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::Change(Chan::E ch, int delta)
{
    ::Math::AdditionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShift::MIN, RShift::MAX);

    Load(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Change(int delta)
{
    ::Math::AdditionThisLimitation<uint16>(&SET_TRIGLEV_SOURCE, STEP_TRIGLEV * delta, Trig::Level::MIN, Trig::Level::MAX);

    Load();

    NeedForDraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Set(int level)
{
    SET_TRIGLEV_SOURCE = (uint16)(level);

    ::Math::Limitation<uint16>(&SET_TRIGLEV_SOURCE, Trig::Level::MIN, Trig::Level::MAX);

    Load();

    NeedForDraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::Set(Chan::E ch, uint16 rShift)
{
    ::Math::Limitation<uint16>(&rShift, MIN, MAX);
    SET_RSHIFT(ch) = rShift;
    Load(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Polarity::Load()
{
    FPGA::GiveStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::Min()
{
#define k 0
#define mul 2

    static const struct StructENumPoints
    {
        int m[3];
        StructENumPoints(int m0, int m1, int m2) { m[0] = m0; m[1] = m1; m[2] = m2; }
    }
    m[ENumPointsFPGA::Size] =
    {
        StructENumPoints(-256 * mul + k,  -128 * mul + k, 0 * mul + k),  // 512
        StructENumPoints(-512 * mul + k,  -256 * mul + k, 0 * mul + k),  // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),  // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),  // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)   // 8192
    };

    return m[(int)FPGA_ENUM_POINTS].m[TPOS];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::Zero()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::Max()
{
    return 60000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::InPoints()
{
    return SET_PEAKDET_EN ? value : (value * 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TShift::TShift(int tshift)
{
    value = tshift;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TShift::Set(int tShift)
{
    LIMITATION(tShift, Min(), Max()); //-V2516

    value = tShift;
    SET_TSHIFT.value = tShift;

    TShift::Load();

    Osci::Restart();

    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String TShift::ToString(TBase::E tBase) const
{
    float time = FPGA::Math::TShift2Abs(value, tBase);
    return Time(time).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String RShift::ToString(uint16 rShiftRel, Osci::Settings::Range::E range, int8 _divider)
{
    float rShiftVal = FPGA::Math::RShift2Abs(rShiftRel, range) * Divider((uint)_divider).ToAbs();
    return Voltage(rShiftVal).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Trig::SyncPulse()
{
    return pulse;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::DrawOnGrid()
{
    if (needDraw)
    {
        int width = 85;
        int height = 18;

        int x = (Grid::Right() - Grid::Left()) / 2 + Grid::Left() - width / 2;
        int y = Grid::ChannelBottom() - height - 20;

        Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

        float trigLevVal = FPGA::Math::RShift2Abs(SET_TRIGLEV_SOURCE, SET_RANGE(TRIG_SOURCE)) * Divider((uint8)SET_DIVIDER(TRIG_SOURCE)).ToAbs();

        Voltage voltage(trigLevVal);

        String("Синхр %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::NeedForDraw()
{
    if (!FFT_ENABLED && TRIG_MODE_FIND_IS_HAND)
    {
        needDraw = true;
        Timer::SetAndStartOnce(Timer::Type::ShowLevelTrigLev, DisableDrawing, 2000);
        Osci::Display::SetFlagRedraw();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Osci::Settings::Trig::DisableDrawing()
{
    needDraw = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString TBase::Name() const
{
    static pString names[TBase::Size] =
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

    return names[value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    names[Range::Size] =
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

    return names[value].name;
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Settings::ModeCouple::Set(Chan::E ch, ModeCouple::E modeCoupe)
{
    SET_COUPLE(ch) = modeCoupe;
    ::Osci::Settings::Range::LoadBoth();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString ModeCouple::UGO() const
{
    static pString couple[] = { "\x92", "\x91", "\x90" };
    return couple[value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Bandwidth::Load()
{
    Chan::E ch = GetChannel();
    static const Pin::E pinsLF[2] = { Pin::LF1, Pin::LF2 };

    WritePin(pinsLF[ch], (SET_BANDWIDTH(ch).value == Bandwidth::_20MHz));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Chan::E Bandwidth::GetChannel() const
{
    return (&SET_BANDWIDTH_A.value == &this->value) ? Chan::A : Chan::B;
}
