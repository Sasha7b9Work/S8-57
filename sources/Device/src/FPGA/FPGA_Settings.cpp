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

    if (Chan(ch).IsA() && Device::State::InModeTester())
    {
        shift = (uint16)((int)shift - Tester::DeltaRShiftA());
    }

    WriteRegisters(Pin::SPI3_CS1, (uint16)(mask[ch] | (shift << 2)));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Load()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::Level::MAX + Trig::Level::MIN) - SET_TRIGLEV_SOURCE);

    WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Settings::LoadCalibratorMode()
{
    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TShift::Load()
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
void TShift::Change(int delta)
{
    TShift::Set(SET_TSHIFT + delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Range::Change(Chan::E ch, int delta)
{
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

    NeedForDraw(2000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Set(int level)
{
    SET_TRIGLEV_SOURCE = (uint16)(level);

    ::Math::Limitation<uint16>(&SET_TRIGLEV_SOURCE, Trig::Level::MIN, Trig::Level::MAX);

    Load();

    if (TRIG_MODE_FIND_IS_HAND)
    {
        NeedForDraw(2000);
    }
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


    if (SET_PEAKDET_DIS)
    {
        return m[(int)FPGA_ENUM_POINTS].m[TPOS];
    }

    return m[(int)FPGA_ENUM_POINTS].m[TPOS] * 2; // Для пикового детектора это значение должно быть в два раза больше

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

    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String TShift::ToString(TBase::E tBase) const
{
    float time = FPGA::Math::TShift2Abs(value, tBase);
    return Time(time).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::DrawBoth()
{
    Draw(Chan::A);
    Draw(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::Draw(Chan::E ch)
{
    Color::SetCurrent(Color::Channel(ch));

    int delta = (SET_RSHIFT(ch) - ZERO) / STEP_RSHIFT;

    int y = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    Char((char)SYMBOL_RSHIFT_NORMAL).Draw(Grid::Left() - 8, y - 4);

    Font::SetCurrent(Font::Type::_5);

    Char(Chan(ch).IsA() ? '1' : '2').Draw(Grid::Left() - 7, y - 6, Color::BACK);

    Font::SetCurrent(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String RShift::ToString(uint16 rShiftRel, Osci::Settings::Range::E range, Divider::E divider)
{
    float rShiftVal = FPGA::Math::RShift2Abs(rShiftRel, range) * Divider(divider).ToAbs();
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
        int y = Grid::Bottom() - height - 20;

        Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

        float trigLevVal = FPGA::Math::RShift2Abs(SET_TRIGLEV_SOURCE, SET_RANGE(TRIG_SOURCE)) * Divider(SET_DIVIDER(TRIG_SOURCE)).ToAbs();

        Voltage voltage(trigLevVal);

        String("Синхр %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::NeedForDraw(uint timeMS)
{
    needDraw = true;
    Timer::SetAndStartOnce(Timer::Type::ShowLevelTrigLev, DisableDrawing, timeMS);
    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Osci::Settings::Trig::DisableDrawing()
{
    needDraw = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString TBase::Name() const
{
    static pString names[TBase::Size][Language::Size] =
    {
        {"2нс",     "2ns"},
        {"5нс",     "5ns"},
        {"10нс",    "10ns"},
        {"20нс",    "20ns"},
        {"50нс",    "50ns"},
        {"0.1мкс",  "0.1us"},
        {"0.2мкс",  "0.2us"},
        {"0.5мкс",  "0.5us"},
        {"1мкс",    "1us"},
        {"2мкс",    "2us"},
        {"5мкс",    "5us"},
        {"10мкс",   "10us"},
        {"20мкс",   "20us"},
        {"50мкс",   "50us"},
        {"0.1мс",   "0.1ms"},
        {"0.2мс",   "0.2ms"},
        {"0.5мс",   "0.5ms"},
        {"1мс",     "1ms"},
        {"2мс",     "2ms"},
        {"5мс",     "5ms"},
        {"10мс",    "10ms"},
        {"20мс",    "20ms"},
        {"50мс",    "50ms"},
        {"0.1с",    "0.1s"},
        {"0.2с",    "0.2s"},
        {"0.5с",    "0.5s"},
        {"1с",      "1s"},
        {"2с",      "2s"},
        {"5с",      "5s"},
        {"10с",     "10s"}
    };

    return names[value][LANG];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Range::Name() const
{
    static const struct StructRange
    {
        const char * names[Language::Size];
        StructRange(pString nRU, pString nEN)
        {
            names[Language::RU] = nRU;
            names[Language::EN] = nEN;
        }
    }
    names[Range::Size] =
    {
        StructRange("2мВ",  "2mV"),
        StructRange("5мВ",  "5mV"),
        StructRange("10мВ", "10mV"),
        StructRange("20мВ", "20mV"),
        StructRange("50мВ", "50mV"),
        StructRange("0.1В", "0.1V"),
        StructRange("0.2В", "0.2V"),
        StructRange("0.5В", "0.5V"),
        StructRange("1В",   "1V"),
        StructRange("2В",   "2V"),
        StructRange("5В",   "5V"),
        StructRange("10В",  "10V"),
        StructRange("20В",  "20V")
    };

    return names[value].names[LANG];
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

    WritePin(pinsLF[ch], 
        IsFixed() ? 
                    true : 
                    (SET_BANDWIDTH(ch).value == Bandwidth::_20MHz)
    );
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Chan::E Bandwidth::GetChannel() const
{
    return (&SET_BANDWIDTH_A.value == &this->value) ? Chan::A : Chan::B;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Bandwidth::IsFixed() const
{
    Chan::E ch = GetChannel();

    return (SET_RANGE(ch) <= Range::_10mV) && (SET_TBASE <= TBase::_500us);
}
