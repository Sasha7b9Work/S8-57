#include "defines.h"
#include "device.h"
#include "MessageMgr.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


#ifdef WIN32
#pragma warning(disable:4310)
#endif


static const int voltsInPixelInt[] =   // Коэффициент 20000
{
    2,      // 2
    5,      // 5
    10,     // 10
    20,     // 20
    50,     // 50
    100,    // 100
    200,    // 200
    500,    // 500
    1000,   // 1
    2000,   // 2
    5000,   // 5
    1000,   // 10
    20000   // 20
};


// Массив структур описаний масштабов по напряжению.
static const struct RangeStruct
{
    pString name;     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    RangeStruct(pString nRU) : name(nRU) {};
}
ranges[Range::Count][2] =
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


const float RShift::absStep[Range::Count] =
{
    2e-3F   / 20 / RShift::STEP,
    5e-3F   / 20 / RShift::STEP,
    10e-3F  / 20 / RShift::STEP,
    20e-3F  / 20 / RShift::STEP,
    50e-3F  / 20 / RShift::STEP,
    100e-3F / 20 / RShift::STEP,
    200e-3F / 20 / RShift::STEP,
    500e-3F / 20 / RShift::STEP,
    1.0F    / 20 / RShift::STEP,
    2.0F    / 20 / RShift::STEP,
    5.0F    / 20 / RShift::STEP,
    10.0F   / 20 / RShift::STEP,
    20.0F   / 20 / RShift::STEP
};


const float TShift::absStep[TBase::Count] =
{
    /* 2 нс    */ 2e-9F   / 20,
    /* 5 нс    */ 5e-9F   / 20,
    /* 10 нс   */ 10e-9F  / 20,
    /* 20 нс   */ 20e-9F  / 20,
    /* 50 нс   */ 50e-9F  / 20,   // 1.0  Это коэффициенты для реализации алгоритма прореживания отсчётов
    /* 100 нс  */ 100e-9F / 20,   // 2.0
    /* 200 нс  */ 200e-9F / 20,   // 4.0
    /* 500 нс  */ 500e-9F / 20,   // 10.0
    /* 1 мкс   */ 1e-6F   / 20,   // 20.0
    /* 2 мкс   */ 2e-6F   / 20,   // 40.0
    /* 5 мкс   */ 5e-6F   / 20,   // 100.0
    /* 10 мкс  */ 10e-6F  / 20,   // 200.0
    /* 20 мкс  */ 20e-6F  / 20,   // 400.0
    /* 50 мкс  */ 50e-6F  / 20,   // 1e3
    /* 100 мкс */ 100e-6F / 20,   // 2e3
    /* 200 мкс */ 200e-6F / 20,   // 4e3
    /* 500 мкс */ 500e-6F / 20,   // 10e3
    /* 1 мс    */ 1e-3F   / 20,   // 20e3
    /* 2 мс    */ 2e-3F   / 20,   // 40e3
    /* 5 мс    */ 5e-3F   / 20,   // 100e3
    /* 10 мс   */ 10e-3F  / 20,   // 200e3
    /* 20 мс   */ 20e-3F  / 20,   // 400e3
    /* 50 мс   */ 50e-3F  / 20,   // 1e4
    /* 100 мс  */ 100e-3F / 20,   // 2e4
    /* 200 мс  */ 200e-3F / 20,   // 4e4
    /* 500 мс  */ 500e-3F / 20,   // 10e4
    /* 1 с     */ 1.0F    / 20,   // 20e4
    /* 2 с     */ 2.0F    / 20,   // 40e4
    /* 5 с     */ 5.0F    / 20,   // 100e4
    /* 10 с    */ 10.0F   / 20    // 200e4
};


///< Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации.
static bool needDraw = false;
/// Установленное в true значение означает, что нужно выводить значок синхроимпульса
bool Trig::pulse = false;


static uint8 ValueForRange(Chan::E ch);


void Osci::LoadHoldfOff()
{
    HAL_FSMC::WriteToFPGA8(WR::TRIG_HOLD_ENABLE, set.trig.holdOffEnabled ? 1U : 0U);

    uint value = static_cast<uint>(0 - set.trig.holdOff + 1);

    BitSet32 bs(value);

    HAL_FSMC::WriteToFPGA8(WR::TRIG_HOLD_VALUE_LOW, bs.byte0);
    HAL_FSMC::WriteToFPGA8(WR::TRIG_HOLD_VALUE_MID, bs.byte1);
    HAL_FSMC::WriteToFPGA8(WR::TRIG_HOLD_VALUE_HI, bs.byte2);
}


void TBase::Change(int delta)
{
    TBase::E old = set.time.base;

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>(reinterpret_cast<uint8 *>(&set.time.base), static_cast<uint8>(TBase::Count - 1));
    }
    else
    {
        if ((set.time.peakDet == PeakDetMode::Enabled) &&           // Если вклюён режим пикового детектора
            set.time.base == TBase::MIN_PEAK_DET)                   // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
        {
            ::Display::ShowWarning("ВКЛЮЧЕН ПИКОВЫЙ ДЕТЕКТОР");		// выводим сообщение об этом
            return;													// и выходим
        }

        ::Math::LimitationDecrease<uint8>(reinterpret_cast<uint8 *>(&set.time.base), 0);
    }

    if (old == set.time.base)
    {
        return;
    }

    Load();

    Osci::Restart();

    DisplayOsci::SetFlagRedraw();
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
    tBases[TBase::Count] =
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
    return m[static_cast<int>(set.mem.enumPoints)][set.time.peakDet][value];
}


void TBase::Load()
{
    static const uint8 values[TBase::Count] =
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

    HAL_FSMC::WriteToFPGA8(WR::TBASE, values[set.time.base]);

    TShift::Load();

    set.ch[Chan::A].bandwidth.Load();
    set.ch[Chan::B].bandwidth.Load();

    MessageMgr::OsciSettingsEffectOnAverageChanged();
}


void Range::LoadBoth()
{
    uint16 val = static_cast<uint16>(ValueForRange(Chan::B) + (ValueForRange(Chan::A) << 8));

    GPIO::WriteRegisters(FPin::SPI3_CS2, val);

    PAUSE_ON_MS(10);                // Задержка нужна, чтобы импульсные реле успели отработать

    GPIO::WriteRegisters(FPin::SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

    //DEF__STRUCT(StructRange, uint8) vals[Range::Count] =
    static const uint8 vals[Range::Count] =
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

    GPIO::WritePin(FPin::A1, _GET_BIT(valueA, 1));
    GPIO::WritePin(FPin::A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[set.ch[Chan::B].range];

    GPIO::WritePin(FPin::A3, _GET_BIT(valueB, 1));
    GPIO::WritePin(FPin::A4, _GET_BIT(valueB, 0));

    set.ch[Chan::A].bandwidth.Load();
    set.ch[Chan::B].bandwidth.Load();

    if(set.disp.lastAffectedChannel == Chan::B)
    {
        RShift::Load(Chan::A);
        RShift::Load(Chan::B);
    }
    else
    {
        RShift::Load(Chan::B);
        RShift::Load(Chan::A);
    }

    Osci::Restart();

    MessageMgr::OsciSettingsEffectOnAverageChanged();
}


pString Range::ToString(int8 _divider)
{
    return ranges[value][_divider].name;
}


static uint8 ValueForRange(Chan::E ch) // -V2506
{
    static const uint8 datas[ModeCouple::Count] =
    {
        BIN_U8(01000001),  // -V2501  // DC
        BIN_U8(10000001),  // -V2501  // AC
        BIN_U8(00000010)   // -V2501  // GND
    };

    //DEF__STRUCT(StructRange, uint16) values[Range::Count][2] =
    static const uint16 values[Range::Count][2] =
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

    return static_cast<uint8>(values[range][ch] | datas[couple]);
}


int16 &RShift::Value(Chan::E ch)
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


void TrigLevel::Find()
{
    if (DS)
    {
        Chan::E ch = static_cast<Chan::E>(set.trig.source);

        if (!ENABLED_DS(ch))
        {
            return;
        }

        const uint8 *data = IN(ch);

        uint numBytes = DS->SizeChannel();

        uint8 max = Math::MaxFromArray(data, 0, static_cast<int>(numBytes) - 1);
        uint8 min = Math::MinFromArray(data, 0, static_cast<int>(numBytes) - 1);

        int deltaValue = static_cast<int>(VALUE::AVE) - (max + min) / 2;

        int deltaRShift = RShift::Value(ch);

        float k = 200 / 125.0F;     // Этот коэффициент получается так, что на верхей границе экрана лежит 125-я точка сигнала от центра экрана (нулевого значение),
                                    // а маркер в этой точке смещён на 200 единиц относительно цента экрана

        float additionShift = deltaValue + deltaRShift / k;     // Итоговое смщение, которое нужно добавить к TrigLev::Zero

        Set(static_cast<int16>(HARDWARE_ZERO + additionShift * k + 0.5F));
    }
}


void RShift::DrawBoth()
{
    if(set.disp.lastAffectedChannel == Chan::B)
    {
        Draw(Chan::A);
        Draw(Chan::B);
    }
    else
    {
        Draw(Chan::B);
        Draw(Chan::A);
    }
}


void RShift::Draw(Chan::E ch)
{
    Color::CHAN[ch].SetAsCurrent();

    int delta = Value(ch) / STEP;

    if (set.fft.enabled)
    {
        delta /= 2;
    }

    int y = (Grid::ChannelBottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    Math::Limitation(&y, Grid::Top(), Grid::ChannelBottom());

    Char(Symbol8::RSHIFT_NORMAL).Draw(Grid::Left() - 8, y - 4);

    Char(Chan(ch).IsA() ? '1' : '2', TypeFont::_5).Draw(Grid::Left() - 7, y - 6, Color::BACK);
}



int Chan::PointsInChannel() const
{
    //DEF__STRUCT(StructNumPoints, int) numPoints[ENumPointsFPGA::Count] =
    static const int numPoints[ENumPointsFPGA::Count] =
    {
        512,
        1024,
        2048,
        4096,
        8192
    };

    return numPoints[static_cast<int>(set.mem.enumPoints)];
}


int Chan::RequestBytes(DataSettings *) const
{
    return ENumPointsFPGA::MAX_NUM;
}


pString Chan::Name() const
{
    static pString names[Chan::Count] =
    {
        "Канал 1",
        "Канал 2"
    };

    return names[value];
}


void TrigLevel::Load()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = static_cast<uint16>(HARDWARE_ZERO + set.trig.lev[set.trig.source]);

    GPIO::WriteRegisters(FPin::SPI3_CS1, static_cast<uint16>(0xa000 | (value << 2)));

    Osci::Restart();
}


void TrigLevel::Change(int16 delta)
{
    Math::AdditionThisLimitation(&set.trig.lev[set.trig.source], TrigLevel::STEP * delta, TrigLevel::MIN, TrigLevel::MAX);

    Load();

    Trig::NeedForDraw();
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
        Timer::SetAndStartOnce(TypeTimer::ShowLevelTrigLev, DisableDrawing, 2000);
        DisplayOsci::SetFlagRedraw();
    }
}


void TrigLevel::Set(int16 level)
{
    set.trig.lev[set.trig.source] = level;

    Math::Limitation(&set.trig.lev[set.trig.source], TrigLevel::MIN, TrigLevel::MAX);

    Load();

    Trig::NeedForDraw();
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

        float trigLevVal = RShift::ToAbs(set.trig.lev[set.trig.source], set.ch[set.trig.source].range) * Divider(static_cast<uint8>(set.ch[set.trig.source].divider)).ToAbs();

        Voltage voltage(trigLevVal);

        String("Синхр %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}


void TrigLevel::Draw()
{
    Chan::E ch = static_cast<Chan::E>(set.trig.source);

    int trigLev = set.trig.lev[set.trig.source] + RShift::Value(ch);
    float scale = 1.0F / ((MAX - MIN) / 2.4F / Grid::Height());
    int y0 = (Grid::Top() + Grid::ChannelBottom()) / 2 + static_cast<int>(scale * (HARDWARE_ZERO - MIN));
    int y = y0 - static_cast<int>(scale * (trigLev - MIN));

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Color::Trig().SetAsCurrent();

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol8::TRIG_LEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);
        Pixel().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol8::TRIG_LEV_ABOVE).Draw(x + 3, Grid::Top() + 2);
        Pixel().Draw(x + 5, Grid::Top() + 2);
    }
    else
    {
        Char(Symbol8::TRIG_LEV_NORMAL).Draw(x + 1, y - 4);
    }

    static const char symbols[2] = { '1', '2' };

    Char(symbols[static_cast<uint8>(set.trig.source)], TypeFont::_5).Draw(x + 5, y - 6, Color::BACK);

    Trig::DrawOnGrid();
}


float RShift::ToAbs(int16 rShift, Range::E range)
{
    return (rShift * absStep[range]);
}

int16 RShift::ToRel(float rShiftAbs, Range::E range)
{
    int16 retValue = static_cast<int16>(rShiftAbs / absStep[range]);

    if (retValue < MIN)
    {
        retValue = MIN;
    }
    else if (retValue > MAX)
    {
        retValue = MAX;
    }
    else
    {
        // здесь ничего делать не нужно
    }

    return retValue;
}


bool RShift::ChangeMath(int delta)
{
    int16 prevRShift = set.math.rShift;
    int16 rShift = prevRShift;

    if (delta > 0)
    {

        if (rShift < MAX)
        {
            rShift += 4 * STEP;  // -V112
            LIMIT_ABOVE(rShift, MAX);
            if (prevRShift < 0 && rShift > 0)
            {
                rShift = 0;
            }
            set.math.rShift = rShift;
            return true;
        }
    }
    else
    {
        if (rShift > MIN)
        {
            rShift -= 4 * STEP; // -V112
            LIMIT_BELOW(rShift, MIN);
            if (prevRShift > 0 && rShift < 0)
            {
                rShift = 0;
            }
            set.math.rShift = rShift;
            return true;
        }
    }

    return false;
}


float TShift::ToAbs(int tShift, TBase::E tBase)
{
    return absStep[tBase] * tShift;
}

void VALUE::PointsToVoltage(const uint8 *points, uint numPoints, Range::E range, int16 rShift, float *voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = MaxVoltageOnScreen(range);
    float rShiftAbs = RShift::ToAbs(rShift, range);
    int diff = static_cast<int>((VALUE::MIN * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3F);
    float koeff = 1.0F / 20e3F;
    for (uint i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPixel - diff) * koeff; //-V636
    }
}


float VALUE::MaxVoltageOnScreen(Range::E range)
{
    //DEF__STRUCT(StructRange, float) table[Range::Count] =
    static const float table[Range::Count] =
    {
        2e-3F, 5e-3F, 10e-3F, 20e-3F, 50e-3F, 100e-3F, 200e-3F, 500e-3F, 1.0F, 2.0F, 5.0F, 10.0F, 20.0F
    };

    return table[range] * 5.0F;
}
