#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/ContextFreqMeter.h"
#include "FreqMeter/DisplayFreqMeter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#include "Utils/Stack.h"
#include "Utils/StringUtils.h"
#include <cstring>


// для отладки
// \todo удалить
static BitSet32 lastFreq;
static BitSet32 lastPeriod;

BitSet32 FreqMeter::freqActual;
BitSet32 FreqMeter::periodActual;
uint     FreqMeter::lastFreqRead;
uint     FreqMeter::lastPeriodRead;
uint     FreqMeter::lastFreqOver;
uint     FreqMeter::lastPeriodOver;
uint     FreqMeter::timeStartMeasureFreq = 0;
uint     FreqMeter::timeStartMeasurePeriod = 0;

static bool freqNeedCalculateFromPeriod;    // Установленное в true значение означает, что частоту нужно считать по счётчику периода
static float prevFreq;
static float frequency;                     // Значение частоты для встроенного частотомера справа вверху экрана



void FreqMeter::Init()
{
    FPGA::LoadSettings();
    FPGA::ResetCounterFreq();
    FPGA::ResetCounterPeriod();
    
    freqActual.word = 0;
    periodActual.word = 0;
}


void FreqMeter::LoadFreqSettings()
{
    FPGA::LoadSettings();
    FPGA::ResetCounterFreq();

    freqActual.word = 0;
    timeStartMeasureFreq = 0;
}


void FreqMeter::LoadPeriodSettings()
{
    FPGA::LoadSettings();
    FPGA::ResetCounterPeriod();

    periodActual.word = 0;
    timeStartMeasurePeriod = 0;
}


void FreqMeter::Update()
{
    SetStateLamps();

    bool freqReady = ContextFreqMeter::GetFlag::FREQ_READY();

    if(freqReady)
    {
        lastFreqRead = TIME_MS;
    }

    bool periodReady = ContextFreqMeter::GetFlag::PERIOD_READY();

    if(periodReady)
    {
        lastPeriodRead = TIME_MS;
    }

    if (freqReady)
    {
        freqActual = FPGA::ReadCounterFreq();

        lastFreq = freqActual;
        
        if (!freqNeedCalculateFromPeriod)
        {
            ReadFreq();
        }
    }

    if (periodReady)
    {
        periodActual = FPGA::ReadCounterPeriod();

        lastPeriod.Set(periodActual.word);

        if (freqNeedCalculateFromPeriod)
        {
            ReadPeriod();
            HAL_BUS::FPGA::Write8(WR::RESET_COUNTER_PERIOD, 1);
        }
    }

    if(ContextFreqMeter::GetFlag::FREQ_OVERFLOW())
    {
        freqActual.word = MAX_UINT;
        lastFreqOver = TIME_MS;
    }
    if(ContextFreqMeter::GetFlag::PERIOD_OVERFLOW())
    {
        periodActual.word = MAX_UINT;
        lastPeriodOver = TIME_MS;
    }
}


void FreqMeter::ReadFreq()
{
    BitSet32 freqSet = FPGA::ReadCounterFreq();

    lastFreq.Set(freqSet.word);

    if (freqSet.word < 1000)
    {
        freqNeedCalculateFromPeriod = true;
    }
    else
    {
        float fr = FreqSetToFreq(&freqSet);
        if (fr < prevFreq * 0.9F || fr > prevFreq * 1.1F)
        {
            frequency = Float::ERROR;
        }
        else
        {
            frequency = fr;
        }
        prevFreq = fr;
    }
}


void FreqMeter::ReadPeriod()
{
    BitSet32 periodSet = FPGA::ReadCounterPeriod();

    lastPeriod = periodSet;

    float fr = PeriodSetToFreq(&periodSet);
    if (fr < prevFreq * 0.9F || fr > prevFreq * 1.1F)
    {
        frequency = Float::ERROR;
    }
    else
    {
        frequency = fr;
    }
    prevFreq = fr;
    freqNeedCalculateFromPeriod = false;
}


float FreqMeter::FreqSetToFreq(const BitSet32 *fr)
{
    const float k[3] = {10.0F, 1.0F, 0.1F};
    return S_FREQ_ENABLED ? (fr->word * k[set.freq.timeCounting]) : (fr->word * 10.0F);
}


float FreqMeter::PeriodSetToFreq(const BitSet32 *period_)
{
    if (period_->word == 0)
    {
        return 0.0F;
    }

    const float k[4] = {10e4F, 10e5F, 10e6F, 10e7F};
    const float kP[3] = {1.0F, 10.0F, 100.0F};

    return S_FREQ_ENABLED ? (k[set.freq.freqClc] * kP[set.freq.numberPeriods] / static_cast<float>(period_->word)) : (10e5F / static_cast<float>(period_->word));
}


float FreqMeter::GetFreq()
{
    return frequency;
}


void FreqMeter::SetStateLamps()
{
    SetStateLampFreq();
    SetStateLampPeriod();
}


void FreqMeter::SetStateLampFreq()
{
    if(timeStartMeasureFreq == 0)
    {
        if(ContextFreqMeter::GetFlag::FREQ_IN_PROCESS())
        {
            timeStartMeasureFreq = TIME_MS;
        }
    }
    else
    {
        if(ContextFreqMeter::GetFlag::FREQ_READY())
        {
            timeStartMeasureFreq = 0;
        }
    }
}


void FreqMeter::SetStateLampPeriod()
{
    if(timeStartMeasurePeriod == 0)
    {
        if(ContextFreqMeter::GetFlag::PERIOD_IN_PROCESS())
        {
            timeStartMeasurePeriod = TIME_MS;
        }
    }
    else
    {
        if(ContextFreqMeter::GetFlag::PERIOD_READY())
        {
            timeStartMeasurePeriod = 0;
        }
    }
}


void DisplayFreqMeter::DrawDebugInfo()
{
    int width = 50;
    int height = 27;
    int x = 50;
    int y = 120;
    Region(width, height).Fill(x, y, Color::BACK);
    Rectangle(width + 2, height + 2).Draw(x - 1, y - 1, Color::FILL);

    String("%d", FreqMeter::freqActual.word).Draw(x + 4, y + 4);
    String("%d", FreqMeter::periodActual.word).Draw(x + 4, y + 15);

    x += 100;
    width = 120;


    Region(width, height).Fill(x, y, Color::BACK);
    Rectangle(width + 2, height + 2).Draw(x - 1, y - 1, Color::FILL);

    String("%d", lastFreq.word).Draw(x + 4, y + 4);
    String("%d", lastPeriod.word).Draw(x + 4, y + 15);

    int size = 8;

    x += 60;

#define TIME 250

    Rectangle(size, size).Draw(x, y + 4, Color::FILL);

    if(TIME_MS - FreqMeter::lastFreqRead < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::BLUE);
    }

    Rectangle(size, size).Draw(x, y + 15, Color::FILL);

    if(TIME_MS - FreqMeter::lastPeriodRead < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::BLUE);
    }

    x += 20;

    Rectangle(size, size).Draw(x, y + 4, Color::FILL);

    if(TIME_MS - FreqMeter::lastFreqOver < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::RED);
    }

    Rectangle(size, size).Draw(x, y + 15, Color::FILL);

    if(TIME_MS - FreqMeter::lastPeriodOver < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::RED);
    }

    x += 20;

    if(ContextFreqMeter::GetFlag::FREQ_IN_PROCESS())
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::FILL);
    }

    if(ContextFreqMeter::GetFlag::PERIOD_IN_PROCESS())
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::FILL);
    }
}


void FreqMeter::FPGA::ResetCounterFreq()
{
    HAL_BUS::FPGA::Write8(WR::RESET_COUNTER_FREQ, 1);
}


void FreqMeter::FPGA::ResetCounterPeriod()
{
    HAL_BUS::FPGA::Write8(WR::RESET_COUNTER_PERIOD, 1);
}


BitSet32 FreqMeter::FPGA::ReadCounterFreq()
{
    BitSet32 result;

    result.byte3 = *RD::FREQ_BYTE_3;
    result.byte2 = *RD::FREQ_BYTE_2;
    result.byte1 = *RD::FREQ_BYTE_1;
    result.byte0 = *RD::FREQ_BYTE_0;

    return result;
}

BitSet32 FreqMeter::FPGA::ReadCounterPeriod()
{
    BitSet32 result;

    result.byte3 = *RD::PERIOD_BYTE_3;
    result.byte2 = *RD::PERIOD_BYTE_2;
    result.byte1 = *RD::PERIOD_BYTE_1;
    result.byte0 = *RD::PERIOD_BYTE_0;

    return result;
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif


void FreqMeter::FPGA::LoadSettings()
{
    uint8 data = 0;

    if(S_FREQ_ENABLED)
    {
        const uint16 maskTime[3] = { 0, 1, 2 };
        const uint16 maskFreqClc[4] = { 0, (1 << 2), (1 << 3), ((1 << 3) + (1 << 2)) };

        static const uint16 maskPeriod[NumberPeriods::Count] =
        { //        654 - задействованные биты
            BIN_U8(00000000), //-V2501
            BIN_U8(00010000), //-V2501
            BIN_U8(00100000), //-V2501
            BIN_U8(00110000), //-V2501
            BIN_U8(01000000), //-V2501 // -V536
            BIN_U8(01010000)  //-V2501
        };

        data |= maskTime[set.freq.timeCounting];
        data |= maskFreqClc[set.freq.freqClc];
        data |= maskPeriod[set.freq.numberPeriods];
    }
    else
    {
        _SET_BIT(data, 2);
    }

    HAL_BUS::FPGA::Write8(WR::FREQMETER, data);
}


#ifdef WIN32
#pragma warning(pop)
#endif
