#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
#include "defines.h"
#include "FrequencyCounter.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Hardware/FSMC.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <string.h>
#include <limits>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BitSet32 FrequencyCounter::freqActual;
BitSet32 FrequencyCounter::periodActual;
bool     FrequencyCounter::readPeriod;
float    FrequencyCounter::prevFreq;
float    FrequencyCounter::frequency;
bool     FrequencyCounter::lampFreq = false;
bool     FrequencyCounter::lampPeriod = false;

//                         0    1    2    3    4    5    6 
static char buffer[11] = {'0', '0', '0', '0', '0', '0', '0', 0, 0, 0, 0};

BitSet32 FrequencyCounter::lastFreq;
BitSet32 FrequencyCounter::lastPeriod;
uint     FrequencyCounter::lastFreqRead = 0;
uint     FrequencyCounter::lastPeriodRead = 0;
uint     FrequencyCounter::lastFreqOver = 0;
uint     FrequencyCounter::lastPeriodOver = 0;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FrequencyCounter::Init()
{
    LoadSettings();
    FSMC::WriteToFPGA8(WR_RESET_COUNTER_FREQ, 1);
    FSMC::WriteToFPGA8(WR_RESET_COUNTER_PERIOD, 1);
    freqActual.word = periodActual.word = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::LoadSettings()
{
    uint8 data = 0;

    if (FREQ_METER_IS_ENABLED)
    {
        const uint16 maskTime[3] = {0, 1, 2};
        const uint16 maskFreqClc[4] = {0, (1 << 2), (1 << 3), ((1 << 3) + (1 << 2))};
        const uint16 maskPeriods[3] = {0, (1 << 4), (1 << 5)};

        data |= maskTime[FREQ_METER_TIMECOUNTING];
        data |= maskFreqClc[FREQ_METER_FREQ_CLC];
        data |= maskPeriods[FREQ_METER_NUM_PERIODS];
    }
    else
    {
        _SET_BIT(data, 2);
    }

    FSMC::WriteToFPGA8(WR_FREQMETER, data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::LoadFreqSettings()
{
    LoadSettings();
    FSMC::WriteToFPGA8(WR_RESET_COUNTER_FREQ, 1);
    freqActual.word = 0;
    lampFreq = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::LoadPeriodSettings()
{
    LoadSettings();
    FSMC::WriteToFPGA8(WR_RESET_COUNTER_PERIOD, 1);
    periodActual.word = 0;
    lampPeriod = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::Update()
{
    SetStateLamps();

    bool freqReady = FPGA::GetFlag::FREQ_READY();

    if(freqReady)
    {
        lastFreqRead = TIME_MS;
    }

    bool periodReady = FPGA::GetFlag::PERIOD_READY();

    if(periodReady)
    {
        lastPeriodRead = TIME_MS;
    }

    if (freqReady)
    {
        freqActual.Set(*RD_FREQ_BYTE_3, *RD_FREQ_BYTE_2, *RD_FREQ_BYTE_1, *RD_FREQ_BYTE_0);

        lastFreq.Set(freqActual.word);
        
        if (!readPeriod)
        {
            ReadFreq();
        }
    }

    if (periodReady)
    {
        periodActual.Set(*RD_PERIOD_BYTE_3, *RD_PERIOD_BYTE_2, *RD_PERIOD_BYTE_1, *RD_PERIOD_BYTE_0);

        lastPeriod.Set(periodActual.word);

        if (readPeriod)
        {
            ReadPeriod();
        }
    }

    if(FPGA::GetFlag::FREQ_OVERFLOW())
    {
        freqActual.word = MAX_UINT;
        lastFreqOver = TIME_MS;
    }
    if(FPGA::GetFlag::PERIOD_OVERFLOW())
    {
        periodActual.word = MAX_UINT;
        lastPeriodOver = TIME_MS;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::ReadFreq()
{
    BitSet32 freqSet(*RD_FREQ_BYTE_3, *RD_FREQ_BYTE_2, *RD_FREQ_BYTE_1, *RD_FREQ_BYTE_0);

    lastFreq.Set(freqSet.word);

    if (freqSet.word < 1000)
    {
        readPeriod = true;
    }
    else
    {
        float fr = FreqSetToFreq(&freqSet);
        if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
        {
            frequency = ERROR_VALUE_FLOAT;
        }
        else
        {
            frequency = fr;
        }
        prevFreq = fr;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::ReadPeriod()
{
    BitSet32 periodSet(*RD_PERIOD_BYTE_3, *RD_PERIOD_BYTE_2, *RD_PERIOD_BYTE_1, *RD_PERIOD_BYTE_0);

    lastPeriod.Set(periodSet.word);

    float fr = PeriodSetToFreq(&periodSet);
    if (fr < prevFreq * 0.9f || fr > prevFreq * 1.1f)
    {
        frequency = ERROR_VALUE_FLOAT;
    }
    else
    {
        frequency = fr;
    }
    prevFreq = fr;
    readPeriod = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::FreqSetToFreq(const BitSet32 *fr)
{
    const float k[3] = {10.0f, 1.0f, 0.1f};
    return FREQ_METER_IS_ENABLED ? (fr->word * k[FREQ_METER_TIMECOUNTING]) : (fr->word * 10.0f);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::PeriodSetToFreq(const BitSet32 *period_)
{
    if (period_->word == 0)
    {
        return 0.0f;
    }

    const float k[4] = {10e4f, 10e5f, 10e6f, 10e7f};
    const float kP[3] = {1.0f, 10.0f, 100.0f};

    return FREQ_METER_IS_ENABLED ? (k[FREQ_METER_FREQ_CLC] * kP[FREQ_METER_NUM_PERIODS] / (float)period_->word) : (10e5f / (float)period_->word);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::GetFreq()
{
    return frequency;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::Draw()
{
    /// \todo В этой строке точку ставить не где придётся, а в той позиции, где она стояла последний раз

#define SIZE 4

    if (!FREQ_METER_IS_ENABLED)
    {
        return;
    }

    int width = 241;
    int height = 74;
    int x = Grid::Left() + (Grid::Width() - width) / 2;
    int y = Grid::Top() + (Grid::Height() / 2 - height) / 2;

    
    Painter::FillRegion(x + 1,   y + 1, width - 2, height - 2, Color::BACK);
    Painter::DrawRectangle(x,    y,     width,     height,     Color::FILL);

    x += 2;
    y += 2;

    Painter::DrawBigText(x + 2,  y + 1,         SIZE, "F", Choice::ColorMenuField(PageFunction::PageFrequencyCounter::GetChoiceTimeF()));

    Painter::DrawRectangle(x - 20, y, 10, 10);
    if(lampFreq)
    {
        Painter::FillRegion(x - 20, y, 10, 10);
    }

    Painter::DrawBigText(x + 2,  y + 10 * SIZE, SIZE, "T", Choice::ColorMenuField(PageFunction::PageFrequencyCounter::GetChoiceNumPeriods()));

    Painter::DrawRectangle(x - 20, y + 10 * SIZE, 10, 10);
    if(lampPeriod)
    {
        Painter::FillRegion(x - 20, y + 10 * SIZE, 10, 10);
    }

    int dX = 7 * SIZE;
    Painter::DrawBigText(x + dX, y + 1,         SIZE, "=", Choice::ColorMenuField(PageFunction::PageFrequencyCounter::GetChoiceTimeF()));
    Painter::DrawBigText(x + dX, y + 10 * SIZE, SIZE, "=", Choice::ColorMenuField(PageFunction::PageFrequencyCounter::GetChoiceNumPeriods()));
    
    dX = SIZE * 12;

    Painter::DrawBigText(x + dX, y + 1, SIZE, FreqSetToString(&freqActual),
                         Choice::ColorMenuField(PageFunction::PageFrequencyCounter::GetChoiceTimeF()));

    Painter::DrawBigText(x + dX, y + 10 * SIZE, SIZE, PeriodSetToString(&periodActual),
                         Choice::ColorMenuField(PageFunction::PageFrequencyCounter::GetChoiceNumPeriods()));


    width = 50;
    height = 27;
    x = 50;
    y = 120;
    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color::FILL);
    Painter::DrawFormatText(x + 4, y + 4, "%d", freqActual.word);
    Painter::DrawFormatText(x + 4, y + 15, "%d", periodActual.word);

    x += 100;
    width = 120;


    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color::FILL);

    Painter::DrawFormatText(x + 4, y + 4, "%d", lastFreq.word);
    Painter::DrawFormatText(x + 4, y + 15, "%d", lastPeriod.word);

    int size = 8;

    x += 60;

#define TIME 250

    Painter::DrawRectangle(x, y + 4, size, size, Color::FILL);

    if(TIME_MS - lastFreqRead < TIME)
    {
        Painter::FillRegion(x + 1, y + 5, size - 2, size - 2, Color::BLUE);
    }

    Painter::DrawRectangle(x, y + 15, size, size, Color::FILL);

    if(TIME_MS - lastPeriodRead < TIME)
    {
        Painter::FillRegion(x + 1, y + 16, size - 2, size - 2, Color::BLUE);
    }

    x += 20;

    Painter::DrawRectangle(x, y + 4, size, size, Color::FILL);

    if(TIME_MS - lastFreqOver < TIME)
    {
        Painter::FillRegion(x + 1, y + 5, size - 2, size - 2, Color::RED);
    }

    Painter::DrawRectangle(x, y + 15, size, size, Color::FILL);

    if(TIME_MS - lastPeriodOver < TIME)
    {
        Painter::FillRegion(x + 1, y + 16, size - 2, size - 2, Color::RED);
    }

    x += 20;

    if(FPGA::GetFlag::FREQ_IN_PROCESS())
    {
        Painter::FillRegion(x + 1, y + 5, size - 2, size - 2, Color::FILL);
    }

    if(FPGA::GetFlag::PERIOD_IN_PROCESS())
    {
        Painter::FillRegion(x + 1, y + 16, size - 2, size - 2, Color::FILL);
    }
}


#define EMPTY_STRING    "\xa9\xa9\xa9.\xa9\xa9\xa9"
#define OVERFLOW_STRING ">>>"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString FrequencyCounter::PeriodSetToString(const BitSet32 *pr)
{
    if (pr->word == 0)
    {
        return EMPTY_STRING;
    }
    else if (pr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }

    Stack<uint> stack(20);

    uint _period = pr->word;

    while(_period > 0)
    {
        stack.Push(_period % 10);
        _period /= 10;
    }

    int order = LowOrder(FREQ_METER_FREQ_CLC, FREQ_METER_NUM_PERIODS);  // В ордер - порядок младшего значащего разряда

    while(stack.Size() < 6)
    {
        stack.Push(0U);                             // Досылаем недостающие нули
    }
                                                    // Теперь в стеке все разряды периода, младший (с порядком order - на дне стека)

    Stack<uint> stackResult(6);                     // Здесь будет храниться шесть итоговых рарядов. Последний - младший

    while(stackResult.Size() < 6)
    {
        stackResult.Push(stack.Pop());
    }
                                                    // Теперь в stackResult все разряды итогового числа. На вершине - младший.
                                                    // Скорректируем его порядок.

    while(!stack.Empty())
    {
        stack.Pop();
        order++;
    }
                                                    // Теперь в order хранится порядок младшего значащего разряда результата (он находится на вершине)

    return StackToString(&stackResult, order);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FrequencyCounter::LowOrder(FreqClc freqCLC, NumberPeriods numPeriods)
{
/*
    Измеряемое значение | Принимаемое значение | Вывод на экран | последний значащий разряд

    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    |          | 100 кГц                    0        | 1 МГц                      1        | 10 МГц                     2        | 100 МГц                    3        |
    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    | 1        | 10   мкс        1    0.01 мс  1e-5  | 1 мкс          1      1. мкс  1e-6  | 100 нс         1     0.1 мкс  1e-7  | 10  нс         1    0.01 мкс  1e-8  | 
    |          | 100  мкс       10    0.10 мс  1e-5  | 10 мкс        10     10. мкс  1e-6  | 1   мкс       10     1.0 мкс  1e-7  | 100 нс        10    0.10 мкс  1e-8  |
    |          | 1    мс       100    1.00 мс  1e-5  | 100 мкс      100    100. мкс  1e-6  | 10  мкс      100    10.0 мкс  1e-7  | 1   мкс      100    1.00 мкс  1e-8  |
    |          | 10   мс      1000   10.00 мс  1e-5  | 1   мс      1000   1.000 мс   1e-6  | 100 мкс     1000   100.0 мкс  1e-7  | 10  мкс     1000   10.00 мкс  1e-8  |
    |          | 100  мс     10000  100.00 мс  1e-5  | 10  мс     10000  10.000 мс   1e-6  | 1   мс     10000  1.0000 мс   1e-7  | 100 мкс    10000  100.00 мкс  1e-8  |
    |          | 1    с     100000 1.00000 с   1e-5  | 100 мс    100000 100.000 мс   1e-6  | 10  мс    100000 10.0000 мс   1e-7  | 1   мс    100000 1.00000 мс   1e-8  |
    |          | 10   с    100000- 10.0000 с   1e-4  | 1   с    100000- 1.00000 с    1e-5  | 100 мс   100000- 100.000 мс   1e-6  | 10  мc   100000- 10.0000 мс   1e-7  |
    |          | 100  с   100000-- 100.000 с   1e-3  | 10  с   100000-- 10.0000 с    1e-4  | 1   с   100000-- 1.00000 с    1e-5  | 100 мс  100000-- 100.000 мс   1e-6  |
    |        0 | 1000 с   переполнение               | 100 с   переполнение                | 10  с   переполнение                | 1   c   переполнение                |
    +----------+-------------------------------------+-------------------------------------|-------------------------------------+-------------------------------------+
    | 10       |                                     |                                     |                                     | 1   нс         1      1. нс   1e-9  |
    |          |                                     |                                     |                         /           | 10  нс        10     10. нс   1e-9  |
    |          |                                     |                                     |                     /               | 100 нс       100    100. нс   1e-9  |
    |          |                                     |                                     |                  /                  | 1   мкс     1000   1.000 мкс  1e-9  |
    |          |                                     |                                     |             /                       | 10  мкс    10000  10.000 мкс  1e-9  | 
    |          |                                     |                                     |         /                           | 100 мкс   100000 100.000 мкс  1e-9  | 
    |          |                                     |                                     |   |  /                              | 1   мс   100000- 1.00000 мс   1e-8  | 
    |          |                                     |                                     |   |---                              | 10  мс  100000-- 10.0000 мс   1e-7  |
    |        1 |                                     |                                     |                                     | 100 мс  переполнение                |
    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    | 100      |                                     |                                     |                                     | 0.1 нс         1     0.1 нс   1e-10 |
    |          |                                     |                                     |                           /         | 1   нс        10     1.0 нс   1e-10 |
    |          |                                     |                                     |                       /             | 10  нс       100    10.0 нс   1e-10 |
    |          |                                     |                                     |                    /                | 100 нс      1000   100.0 нс   1e-10 |
    |          |                                     |                                     |               /                     | 1   мкс    10000  1.0000 мкс  1e-10 |
    |          |                                     |                                     |           /                         | 10  мкс   100000 10.0000 мкс  1e-10 |
    |          |                                     |                                     |     |  /                            | 100 мкс  100000- 100.000 мкс  1e-9  |
    |          |                                     |                                     |     |---                            | 1   мс  100000-- 1.00000 мс   1e-8  |
    |        2 |                                     |                                     |                                     | 10  мс  переполнение                |
    +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
*/

    //                           | 100к/1 | 1М  /1  |  10М /1  | 100М/1   |          |
    //                           |        | 100к/10 |  1М  /10 | 10М /10  | 100М/1   |
    //                           |        |         |  100к/2  | 1М  /100 | 10М /100 | 100М/100
    static const int order[6] = {  -5,      -6,        -7,       -8,        -9,        -10};

    return order[freqCLC + numPeriods];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString FrequencyCounter::StackToString(Stack<uint> *stack, int order)
{
    if(order == -10)
    {
        if(stack->NumFirstZeros()< 2)
        {
            WriteStackToBuffer(stack, 2, "мкс");
        }
        else
        {
            WriteStackToBuffer(stack, 5, "нс");
        }
    }
    else if(order == -9)
    {
        if(stack->NumFirstZeros() < 3)
        {
            WriteStackToBuffer(stack, 3, "мкс");
        }
        else
        {
            WriteStackToBuffer(stack, 6, "нс");
        }
    }
    else if(order == -8)
    {
        if(stack->NumFirstZeros() < 1)
        {
            WriteStackToBuffer(stack, 1, "мс");
        }
        else
        {
            WriteStackToBuffer(stack, 4, "мкс");
        }
    }
    else if (order == -7)
    {
        if(stack->NumFirstZeros() < 2)
        {
            WriteStackToBuffer(stack, 2, "мс");
        }
        else
        {
            WriteStackToBuffer(stack, 5, "мкс");
        }
    }
    else if (order == -6)
    {
        if(stack->NumFirstZeros() < 3)
        {
            WriteStackToBuffer(stack, 3, "мс");
        }
        else
        {
            WriteStackToBuffer(stack, 6, "мкс");
        }
    }
    else if (order == -5)
    {
        if(stack->NumFirstZeros() < 1)
        {
            WriteStackToBuffer(stack, 1, "с");
        }
        else
        {
            WriteStackToBuffer(stack, 4, "мс");
        }
    }
    else if (order == -4)
    {
        if(stack->NumFirstZeros() < 2)
        {
            WriteStackToBuffer(stack, 2, "с");
        }
        else
        {
            WriteStackToBuffer(stack, 5, "мс");
        }
    }
    else if (order == -3)
    {
        if(stack->NumFirstZeros() < 3)
        {
            WriteStackToBuffer(stack, 3, "с");
        }
        else
        {
            WriteStackToBuffer(stack, 6, "мс");
        }
    }
    else if (order == -2)
    {
        WriteStackToBuffer(stack, 4, "с");
    }
    else if (order == -1)
    {
        WriteStackToBuffer(stack, 5, "c");
    }
    else
    {
        WriteStackToBuffer(stack, 6, "c");
    }

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::WriteStackToBuffer(Stack<uint> *stack, int point, char *suffix)
{
    for(int i = 6; i >= 0; i--)
    {
        if(point == i)
        {
            buffer[i] = '.';
            continue;
        }
        buffer[i] = ((char)stack->Pop() | 0x30);
    }

    strcpy(&buffer[7], suffix);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString FrequencyCounter::FreqSetToString(const BitSet32 *fr)
{
    if(fr->word == 0)
    {
        return EMPTY_STRING;
    }
    else if(fr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }

    Hex value(fr->word);

    while(value.NumDigits() > 6)
    {
        value.Set(value / 10);
    }

    for(int i = 0; i < 7; i++)
    {
        buffer[i] = value.DigitInPosition(6 - i);
    }

    uint freq = fr->word;

    uint giverFreq = freq;

/// Это герцы * 10
#define _10Hz   (               100) /* E_2 */
#define _100Hz  (              1000) /* E_3 */
#define _1kHz   (         10 * 1000) /* E_4 */
#define _10kHz  (        100 * 1000) /* E_5 */
#define _100kHz (       1000 * 1000) /* E_6 */
#define _1MHz   (  10 * 1000 * 1000) /* E_7 */
#define _10MHz  ( 100 * 1000 * 1000) /* E_8 */
#define _100MHz (1000 * 1000 * 1000) /* E_9 */


#undef WRITE_SUFFIX
#define WRITE_SUFFIX(suffix_E4)    \
    if(giverFreq < _1kHz) { strcpy(buffer + 7, suffix_E4); } else if (giverFreq < _1MHz) { strcpy(buffer + 7, "кГц"); } else { strcpy(buffer + 7, "МГц"); }

#define HIGH_FREQ                       \
    if(giverFreq < _10MHz)              \
    {                                   \
        memcpy(buffer, buffer + 1, 2);  \
        buffer[1] = '.';                \
    }                                   \
    else if (giverFreq < _100MHz)       \
    {                                   \
        memcpy(buffer, buffer + 1, 3);  \
        buffer[2] = '.';                \
    }                                   \
    else                                \
    {                                   \
        memcpy(buffer, buffer + 1, 3);  \
        buffer[3] = '.';                \
    }


    switch (FREQ_METER_TIMECOUNTING)
    {
        case FrequencyCounter::TimeCounting::_100ms:

            giverFreq *= 100;

            WRITE_SUFFIX("кГц");

            if(giverFreq < _1MHz)                       // Меньше 1 МГц
            {
                if(freq >= _10Hz)                       // Больше или равно 10 Гц
                {
                    memcpy(buffer, buffer + 1, 5);
                }
                buffer[4] = '.';
            }
            else
            {
                HIGH_FREQ;
            }            
            break;

        case FrequencyCounter::TimeCounting::_1s:

            giverFreq *= 10;

            WRITE_SUFFIX("Гц");

            if (giverFreq < _1MHz)                      // Меньше 1 МГц
            {
                if(giverFreq < _1kHz)                   // Меньше 1 кГц
                {
                    memcpy(buffer, buffer + 1, 6);
                    buffer[6] = '.';
                }
                else
                {
                    memcpy(buffer, buffer + 1, 4);
                    buffer[3] = '.';
                }
            }
            else
            {
                HIGH_FREQ;
            }
            break;

        case FrequencyCounter::TimeCounting::_10s:

            WRITE_SUFFIX("Гц");

            if (freq < _1MHz)                       // Меньше 1 МГц
            {
                if (giverFreq < _1kHz)              // Меньше 1 кГц
                {
                    memcpy(buffer, buffer + 1, 5);
                    buffer[5] = '.';
                }
                else if(giverFreq < _100kHz)
                {
                    memcpy(buffer, buffer + 1, 3);
                    buffer[2] = '.';
                }
                else
                {
                    memcpy(buffer, buffer + 1, 3);
                    buffer[3] = '.';
                }
            }
            else
            {
                HIGH_FREQ;
            }
            break;
        default:
            break;
    }

    return buffer;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::SetStateLamps()
{
    SetStateLampFreq();
    SetStateLampPeriod();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::SetStateLampFreq()
{
    if(!lampFreq)
    {
        if(FPGA::GetFlag::FREQ_IN_PROCESS())
        {
            lampFreq = true;
        }
    }
    else
    {
        if(FPGA::GetFlag::FREQ_READY())
        {
            lampFreq = false;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::SetStateLampPeriod()
{
    if(!lampPeriod)
    {
        if(FPGA::GetFlag::PERIOD_IN_PROCESS())
        {
            lampPeriod = true;
        }
    }
    else
    {
        if(FPGA::GetFlag::PERIOD_READY())
        {
            lampPeriod = false;
        }
    }
}
