#include "defines.h"
#include "device.h"
#include "MessageMgr.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Hal.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Osci/Display/Osci_Display.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


#ifdef WIN32
#pragma warning(disable:4310)
#endif


using namespace HAL::ADDRESSES::FPGA;

using FPGA::HAL::GPIO::Pin;
using HAL::FSMC;


// Массив структур описаний масштабов по напряжению.
static const struct RangeStruct
{
    pString name;     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    RangeStruct(pString nRU) : name(nRU) {};
}
ranges[Range::Size][2] =
{
    {RangeStruct("2\x10мВ"),  RangeStruct("20\x10мВ")},
    {RangeStruct("5\x10мВ"),  RangeStruct("50\x10мВ")},
    {RangeStruct("10\x10мВ"), RangeStruct("0.1\x10В")},
    {RangeStruct("20\x10мВ"), RangeStruct("0.2\x10В")},
    {RangeStruct("50\x10мВ"), RangeStruct("0.5\x10В")},
    {RangeStruct("0.1\x10В"), RangeStruct("1\x10В")},
    {RangeStruct("0.2\x10В"), RangeStruct("2\x10В")},
    {RangeStruct("0.5\x10В"), RangeStruct("5\x10В")},
    {RangeStruct("1\x10В"),   RangeStruct("10\x10В")},
    {RangeStruct("2\x10В"),   RangeStruct("20\x10В")},
    {RangeStruct("5\x10В"),   RangeStruct("50\x10В")},
    {RangeStruct("10\x10В"),  RangeStruct("100\x10В")},
    {RangeStruct("20\x10В"),  RangeStruct("200\x10В")}
};


///< Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации.
static bool needDraw = false;
namespace Trig
{
    /// Установленное в true значение означает, что нужно выводить значок синхроимпульса
    bool pulse = false;
}



static uint8 ValueForRange(Chan::E ch);



void Osci::LoadHoldfOff()
{
    FSMC::WriteToFPGA8(WR::TRIG_HOLD_ENABLE, set.trig.holdOffEnabled ? 1U : 0U);

    uint value = (uint)(0 - set.trig.holdOff + 1);

    BitSet32 bs(value);

    FSMC::WriteToFPGA8(WR::TRIG_HOLD_VALUE_LOW, bs.byte0);
    FSMC::WriteToFPGA8(WR::TRIG_HOLD_VALUE_MID, bs.byte1);
    FSMC::WriteToFPGA8(WR::TRIG_HOLD_VALUE_HI, bs.byte2);
}


void TBase::Change(int delta)
{
    TBase::E old = set.time.base;

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&set.time.base), (uint8)(TBase::Size - 1));
    }
    else
    {
        if ((set.time.peakDet == PeakDetMode::Enabled) &&                                   // Если вклюён режим пикового детектора
            set.time.base == TBase::MIN_PEAK_DET)                   // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
        {
            ::Display::ShowWarning(Warning::EnabledPeakDet);    // выводим сообщение об этом
            return;                                             // и выходим
        }

        ::Math::LimitationDecrease<uint8>((uint8 *)(&set.time.base), 0);
    }

    if (old == set.time.base)
    {
        return;
    }

    Load();

    Osci::Restart();

    Osci::Display::SetFlagRedraw();
}


pString TBase::ToString() const
{
    /// Структура для описания диапазона масштаба по времени.
    static const struct StructTBase
    {
        const char *name;    // Название диапазона в текстовом виде, пригодном для вывода на экран.
        StructTBase(pString nRU)
        {
            name = nRU;
        };
    }
    tBases[TBase::Size] =
    {
        StructTBase("2\x10нс"),
        StructTBase("5\x10нс"),
        StructTBase("10\x10нс"),
        StructTBase("20\x10нс"),
        StructTBase("50\x10нс"),
        StructTBase("0.1\x10мкс"),
        StructTBase("0.2\x10мкс"),
        StructTBase("0.5\x10мкс"),
        StructTBase("1\x10мкс"),
        StructTBase("2\x10мкс"),
        StructTBase("5\x10мкс"),
        StructTBase("10\x10мкс"),
        StructTBase("20\x10мкс"),
        StructTBase("50\x10мкс"),
        StructTBase("0.1\x10мс"),
        StructTBase("0.2\x10мс"),
        StructTBase("0.5\x10мс"),
        StructTBase("1\x10мс"),
        StructTBase("2\x10мс"),
        StructTBase("5\x10мс"),
        StructTBase("10\x10мс"),
        StructTBase("20\x10мс"),
        StructTBase("50\x10мс"),
        StructTBase("0.1\x10с"),
        StructTBase("0.2\x10с"),
        StructTBase("0.5\x10с"),
        StructTBase("1\x10с"),
        StructTBase("2\x10с"),
        StructTBase("5\x10с"),
        StructTBase("10\x10с")
    };

    return tBases[value].name;
}


int TPos::InBytes()
{
    static const int m[][2][3] =
    {
        {{0,   256,  510},  {0,   512,  1022}},
        {{0,   512,  1022}, {0,  1024,  2046}},
        {{0,  1024,  2046}, {0,  2048,  4094}},
        {{0,  2048,  4094}, {0,  4096,  8190}},
        {{0,  4096,  8190}, {0,  8192, 16382}},
        {{0,  8192, 16382}, {0,  8192, 16382}},
        {{0, 16384, 32766}, {0, 16384, 32766}}
    };
    return m[(int)set.mem.enumPoints][set.time.peakDet][value];
}


void TBase::Load()
{
    static const uint8 values[TBase::Size] =
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

    FSMC::WriteToFPGA8(WR::TBASE, values[set.time.base]);

    TShift::Load();

    set.ch[Chan::A].bandwidth.Load();
    set.ch[Chan::B].bandwidth.Load();

    MessageMgr::OsciSettingsEffectOnAverageChanged();
}


void Range::LoadBoth()
{
    uint16 val = (uint16)(ValueForRange(Chan::B) + (ValueForRange(Chan::A) << 8));

    WriteRegisters(Pin::SPI3_CS2, val);

    PAUSE_ON_MS(10);                // Задержка нужна, чтобы импульсные реле успели отработать

    WriteRegisters(Pin::SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

    //DEF__STRUCT(StructRange, uint8) vals[Range::Size] =
    static const uint8 vals[Range::Size] =
    {
        BIN_U8(00000000),  // 2mV      // -V2501
        BIN_U8(00000001),  // 5mV      // -V2501
        BIN_U8(00000010),  // 10mV     // -V2501
        BIN_U8(00000011),  // 20mV     // -V2501
        BIN_U8(00000001),  // 50mV     // -V2501
        BIN_U8(00000010),  // 100mV    // -V2501
        BIN_U8(00000011),  // 200mV    // -V2501
        BIN_U8(00000001),  // 500mV    // -V2501
        BIN_U8(00000010),  // 1V       // -V2501
        BIN_U8(00000011),  // 2V       // -V2501
        BIN_U8(00000001),  // 5V       // -V2501
        BIN_U8(00000010),  // 10V      // -V2501
        BIN_U8(00000011)   // 20V      // -V2501
    };

    uint8 valueA = vals[set.ch[Chan::A].range];

    WritePin(Pin::A1, _GET_BIT(valueA, 1));
    WritePin(Pin::A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[set.ch[Chan::B].range];

    WritePin(Pin::A3, _GET_BIT(valueB, 1));
    WritePin(Pin::A4, _GET_BIT(valueB, 0));

    set.ch[Chan::A].bandwidth.Load();
    set.ch[Chan::B].bandwidth.Load();

    RShift::Load(Chan::A);

    RShift::Load(Chan::B);

    Osci::Restart();

    MessageMgr::OsciSettingsEffectOnAverageChanged();
}


pString Range::ToString(int8 _divider)
{
    return ranges[value][_divider].name;
}


static uint8 ValueForRange(Chan::E ch) // -V2506
{
    static const uint8 datas[ModeCouple::Size] =
    {
        BIN_U8(01000001),  // -V2501  // DC
        BIN_U8(10000001),  // -V2501  // AC
        BIN_U8(00000010)   // -V2501  // GND
    };

    //DEF__STRUCT(StructRange, uint16) values[Range::Size][2] =
    static const uint16 values[Range::Size][2] =
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

    ModeCouple::E couple = (Device::State::InModeRecorder()) ? ModeCouple::DC : set.ch[ch].couple;
    Range::E range = set.ch[ch].range;

    if (Device::State::InModeOsci() && couple == ModeCouple::GND)
    {
        return datas[ModeCouple::GND];
    }

    return (uint8)(values[range][ch] | datas[couple]);
}


uint16 &RShift::Value(Chan::E ch)
{
    return set.ch[ch].rShift;
}


uint ENumPointsFPGA::BytesInChannel(PeakDetMode::E mode) const
{
    uint result = ENUM_TO_REL_POINTS(value);
    if (mode == PeakDetMode::Enabled)
    {
        result *= 2;
    }

    return result;
}


uint ENumPointsFPGA::PointsInChannel() const
{
    return ENUM_TO_REL_POINTS(value);
}


void Trig::Level::Find()
{
    if (DATA)
    {
        Chan::E ch = (Chan::E)set.trig.source;

        if (!ENABLED_DS(ch))
        {
            return;
        }

        const uint8 *data = IN(ch);

        int numBytes = DS->SizeChannel();

        uint8 max = Math::MaxFromArray(data, 0, (int)numBytes - 1);
        uint8 min = Math::MinFromArray(data, 0, (int)numBytes - 1);

        int deltaValue = (int)FPGA::VALUE::AVE - (max + min) / 2;

        int deltaRShift = SET_RSHIFT(ch) - RShift::ZERO;

        float k = 200 / 125.0F;     // Этот коэффициент получается так, что на верхей границе экрана лежит 125-я точка сигнала от центра экрана (нулевого значение),
                                    // а маркер в этой точке смещён на 200 единиц относительно цента экрана

        float additionShift = deltaValue + deltaRShift / k;     // Итоговое смщение, которое нужно добавить к TrigLev::Zero

        Set((int)(ZERO - (additionShift * k + 0.5F)));
    }
}


void RShift::DrawBoth()
{
    Draw(Chan::A);
    Draw(Chan::B);
}


void RShift::Draw(Chan::E ch)
{
    Color::CHAN[ch].SetAsCurrent();

    int delta = (SET_RSHIFT(ch) - ZERO) / STEP_RSHIFT;

    if (set.fft.enabled)
    {
        delta /= 2;
    }

    int y = (Grid::ChannelBottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    Math::Limitation(&y, Grid::Top(), Grid::ChannelBottom());

    Char((char)Symbol8::RSHIFT_NORMAL).Draw(Grid::Left() - 8, y - 4);

    Font::SetCurrent(TypeFont::_5);

    Char(Chan(ch).IsA() ? '1' : '2').Draw(Grid::Left() - 7, y - 6, Color::BACK);

    Font::SetCurrent(TypeFont::_8);
}



int Chan::PointsInChannel() const
{
    //DEF__STRUCT(StructNumPoints, int) numPoints[ENumPointsFPGA::Size] =
    static const int numPoints[ENumPointsFPGA::Size] =
    {
        512,
        1024,
        2048,
        4096,
        8192
    };

    return numPoints[(int)set.mem.enumPoints];
}


int Chan::RequestBytes(DataSettings *) const
{
    return FPGA::MAX_NUM_POINTS;
}


pString Chan::Name() const
{
    static pString names[Chan::Size] =
    {
        "Канал 1",
        "Канал 2"
    };

    return names[value];
}


void Trig::Level::Load()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::Level::MAX + Trig::Level::MIN) - set.trig.lev[set.trig.source]);

    WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));

    Osci::Restart();
}


void Trig::Level::Change(int delta)
{
    Math::AdditionThisLimitation<uint16>(&set.trig.lev[set.trig.source], STEP_TRIGLEV * delta, Trig::Level::MIN, Trig::Level::MAX);

    Load();

    NeedForDraw();
}


static void DisableDrawing()
{
    needDraw = false;
}


void Trig::NeedForDraw()
{
    if (!set.fft.enabled && (set.trig.modeFind == TrigModeFind::Hand))
    {
        needDraw = true;
        Timer::SetAndStartOnce(Timer::Type::ShowLevelTrigLev, DisableDrawing, 2000);
        Osci::Display::SetFlagRedraw();
    }
}


void Trig::Level::Set(int level)
{
    set.trig.lev[set.trig.source] = (uint16)(level);

    Math::Limitation<uint16>(&set.trig.lev[set.trig.source], Trig::Level::MIN, Trig::Level::MAX);

    Load();

    NeedForDraw();
}


bool Trig::SyncPulse()
{
    return pulse;
}


void Trig::DrawOnGrid()
{
    if (needDraw)
    {
        int width = 85;
        int height = 18;

        int x = (Grid::Right() - Grid::Left()) / 2 + Grid::Left() - width / 2;
        int y = Grid::ChannelBottom() - height - 20;

        Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

        float trigLevVal = FPGA::Math::RShift2Abs(set.trig.lev[set.trig.source], set.ch[set.trig.source].range) * Divider((uint8)set.ch[set.trig.source].divider).ToAbs();

        Voltage voltage(trigLevVal);

        String("Синхр %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}
