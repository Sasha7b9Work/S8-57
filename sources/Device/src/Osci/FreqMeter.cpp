#include "defines.h"
#include "Osci/FreqMeter.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/ContextFreqMeter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#include "Utils/Stack.h"
#include "Utils/StringUtils.h"
#include <cstring>


#define EMPTY_STRING    "---.---"
#define OVERFLOW_STRING ">>>"


/// для отладки
/// \todo удалить
static BitSet32 lastFreq;
static BitSet32 lastPeriod;

bool     FreqMeter::readPeriod;
float    FreqMeter::prevFreq;
float    FreqMeter::frequency;
BitSet32 FreqMeter::freqActual;
BitSet32 FreqMeter::periodActual;
uint     FreqMeter::lastFreqRead;
uint     FreqMeter::lastPeriodRead;
uint     FreqMeter::lastFreqOver;
uint     FreqMeter::lastPeriodOver;
uint     FreqMeter::timeStartMeasureFreq = 0;
uint     FreqMeter::timeStartMeasurePeriod = 0;

//                         0    1    2    3    4    5    6 
static char buffer[11] = {'0', '0', '0', '0', '0', '0', '0', 0, 0, 0, 0};


void FreqMeter::Init()
{
    LoadSettings();
    HAL_FSMC::WriteToFPGA8(WR::RESET_COUNTER_FREQ, 1);
    HAL_FSMC::WriteToFPGA8(WR::RESET_COUNTER_PERIOD, 1);
    freqActual.word = periodActual.word = 0;
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif



void FreqMeter::LoadSettings()
{
    uint8 data = 0;

    if (FreqMeterEnabled())
    {
        const uint16 maskTime[3] = {0, 1, 2};
        const uint16 maskFreqClc[4] = {0, (1 << 2), (1 << 3), ((1 << 3) + (1 << 2))};

        static const uint16 maskPeriod[FreqMeterNumberPeriods::Count] =
        { //        654 - задействованные биты
            BIN_U8(00000000), //-V2501
            BIN_U8(00010000), //-V2501
            BIN_U8(00100000), //-V2501
            BIN_U8(00110000), //-V2501
            BIN_U8(01000000), //-V2501 // -V536
            BIN_U8(01010000)  //-V2501
        };

        data |= maskTime[FreqMeterTimeCounting()];
        data |= maskFreqClc[set.freq.freqClc];
        data |= maskPeriod[set.freq.numberPeriods];
    }
    else
    {
        _SET_BIT(data, 2);
    }

    HAL_FSMC::WriteToFPGA8(WR::FREQMETER, data);
}


#ifdef WIN32
#pragma warning(pop)
#endif



void FreqMeter::LoadFreqSettings()
{
    LoadSettings();
    HAL_FSMC::WriteToFPGA8(WR::RESET_COUNTER_FREQ, 1);
    freqActual.word = 0;
    timeStartMeasureFreq = 0;
}


void FreqMeter::LoadPeriodSettings()
{
    LoadSettings();
    HAL_FSMC::WriteToFPGA8(WR::RESET_COUNTER_PERIOD, 1);
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
        freqActual.Set(*RD::FREQ_BYTE_3, *RD::FREQ_BYTE_2, *RD::FREQ_BYTE_1, *RD::FREQ_BYTE_0);

        lastFreq.Set(freqActual.word);
        
        if (!readPeriod)
        {
            ReadFreq();
        }
    }

    if (periodReady)
    {
        periodActual.Set(*RD::PERIOD_BYTE_3, *RD::PERIOD_BYTE_2, *RD::PERIOD_BYTE_1, *RD::PERIOD_BYTE_0);

        lastPeriod.Set(periodActual.word);

        if (readPeriod)
        {
            ReadPeriod();
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
    BitSet32 freqSet(*RD::FREQ_BYTE_3, *RD::FREQ_BYTE_2, *RD::FREQ_BYTE_1, *RD::FREQ_BYTE_0);

    lastFreq.Set(freqSet.word);

    if (freqSet.word < 1000)
    {
        readPeriod = true;
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
    BitSet32 periodSet(*RD::PERIOD_BYTE_3, *RD::PERIOD_BYTE_2, *RD::PERIOD_BYTE_1, *RD::PERIOD_BYTE_0);

    lastPeriod.Set(periodSet.word);

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
    readPeriod = false;
}


float FreqMeter::FreqSetToFreq(const BitSet32 *fr)
{
    const float k[3] = {10.0F, 1.0F, 0.1F};
    return FreqMeterEnabled() ? (fr->word * k[FreqMeterTimeCounting()]) : (fr->word * 10.0F);
}


float FreqMeter::PeriodSetToFreq(const BitSet32 *period_)
{
    if (period_->word == 0)
    {
        return 0.0F;
    }

    const float k[4] = {10e4F, 10e5F, 10e6F, 10e7F};
    const float kP[3] = {1.0F, 10.0F, 100.0F};

    return FreqMeterEnabled() ? (k[set.freq.freqClc] * kP[set.freq.numberPeriods] / static_cast<float>(period_->word)) : (10e5F / static_cast<float>(period_->word));
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


void DisplayFreqMeter::Draw()
{
    /// \todo В этой строке точку ставить не где придётся, а в той позиции, где она стояла последний раз

    if (FreqMeterEnabled())
    {
        return;
    }

    Font::Set(TypeFont::_GOST28);
    int spacing = Font::GetSpacing();
    Font::SetSpacing(1);

    int width = 200;
    int height = 90;
    int x = Grid::Left() + (Grid::Width() - width) / 2;
    int y = Grid::Top() + (Grid::Height() / 2 - height) / 2;


    Region(width - 2, height - 2).Fill(x + 1, y + 1, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    x += 2;
    y += 2;

    if (FreqMeterModeView().IsFrequency())
    {
        DrawFrequency(x, y);
    }
    else
    {
        DrawPeriod(x, y);
    }

    if (false)
    {
        DrawDebugInfo();
    }

    Font::Pop();
    Font::SetSpacing(spacing);
    Font::SetMinWidth(0);
}


void DisplayFreqMeter::DrawFrequency(int x, int _y)
{
    _y += 4;
    
    int yF = _y;
    int yT = _y + 4 + Font::GetHeight();

    x += 6;
    
    Text("F").Draw(x, yF, Color::FILL);
    Text("T").Draw(x, yT);

    ProgressBarFreqMeter::Draw(x, yT + 4 + Font::GetHeight());

    int dX = 17;

    Text("=").Draw(x + dX, yF);
    Text("=").Draw(x + dX, yT);

    dX = 32;

    char strFreq[50];
    std::strcpy(strFreq, FreqSetToString(&FreqMeter::freqActual));

    Text(strFreq).DrawDigitsMonospace(x + dX, yF, Font::GetWidth('0'));

    float freq = SU::StringToFloat(strFreq);

    if (std::strcmp(&strFreq[std::strlen(strFreq) - 3], "МГц") == 0)
    {
        freq *= 1e6F;
    }
    else if (std::strcmp(&strFreq[std::strlen(strFreq) - 3], "кГц") == 0)
    {
        freq *= 1e3F;
    }
    else if (std::strcmp(&strFreq[std::strlen(strFreq) - 3], "мГц") == 0)
    {
        freq *= 1e-3F;
    }
    else
    {
        // сюда никогда не дойдём
    }

    Time time(1.0F / freq);
    
    if(time.Value() == std::numeric_limits<float>::infinity())
    {
        Text(EMPTY_STRING).DrawDigitsMonospace(x + dX, yT, Font::GetWidth('0'));
    }
    else
    {
        Text(time.ToStringAccuracy(false, strFreq, 6)).DrawDigitsMonospace(x + dX, yT, Font::GetWidth('0'));
    }
}


void DisplayFreqMeter::DrawPeriod(int x, int _y)
{
    _y += 4;

    int yT = _y;
    int yF = _y + 4 + Font::GetHeight();

    x += 6;

    Text("T").Draw(x, yT, Color::FILL);
    Text("F").Draw(x, yF);

    ProgressBarFreqMeter::Draw(x, yF + 4 + Font::GetHeight());

    int dX = 17;

    Text("=").Draw(x + dX, yT);

    Text("=").Draw(x + dX, yF);

    dX = 32;

    char strPeriod[50];
    std::strcpy(strPeriod, PeriodSetToString(&FreqMeter::periodActual));

    Text(strPeriod).DrawDigitsMonospace(x + dX, yT, Font::GetWidth('0'));

    if ((std::strcmp(strPeriod, EMPTY_STRING) == 0) || (std::strcmp(strPeriod, OVERFLOW_STRING) == 0))
    {
        return;
    }

    float per = SU::StringToFloat(strPeriod);

    if (std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "нс") == 0)
    {
        per *= 1e-9F;
    }
    else if (std::strcmp(&strPeriod[std::strlen(strPeriod) - 3], "мкс") == 0)
    {
        per *= 1e-6F;
    }
    else if (std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "мс") == 0)
    {
        per *= 1e-3F;
    }
    else
    {
        LOG_ERROR("Здесь мы никогда не должны оказаться");
    }

    Frequency freq(1.0F / per);

    Text(freq.ToStringAccuracy(strPeriod, 6)).DrawDigitsMonospace(x + dX, yF, Font::GetWidth('0'));
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

    if (TIME_MS - FreqMeter::lastFreqRead < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::BLUE);
    }

    Rectangle(size, size).Draw(x, y + 15, Color::FILL);

    if (TIME_MS - FreqMeter::lastPeriodRead < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::BLUE);
    }

    x += 20;

    Rectangle(size, size).Draw(x, y + 4, Color::FILL);

    if (TIME_MS - FreqMeter::lastFreqOver < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::RED);
    }

    Rectangle(size, size).Draw(x, y + 15, Color::FILL);

    if (TIME_MS - FreqMeter::lastPeriodOver < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::RED);
    }

    x += 20;

    if (ContextFreqMeter::GetFlag::FREQ_IN_PROCESS())
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::FILL);
    }

    if (ContextFreqMeter::GetFlag::PERIOD_IN_PROCESS())
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::FILL);
    }
}


pString DisplayFreqMeter::FreqSetToString(const BitSet32 *fr)
{
    if (fr->word < 2)
    {
        return EMPTY_STRING;
    }
    else if (fr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }
    else
    {
        // здесь ничего не делаем
    }

    Hex value(fr->word);

    while (value.NumDigits() > 6)
    {
        value.Set(value / 10);
    }

    for (int i = 0; i < 7; i++)
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
    if(giverFreq < _1kHz) { std::strcpy(buffer + 7, suffix_E4); } else if (giverFreq < _1MHz) { std::strcpy(buffer + 7, "кГц"); } else { std::strcpy(buffer + 7, "МГц"); }

#define HIGH_FREQ                            \
    if(giverFreq < _10MHz)                   \
    {                                        \
        std::memmove(buffer, buffer + 1, 2); \
        buffer[1] = '.';                     \
    }                                        \
    else if (giverFreq < _100MHz)            \
    {                                        \
        std::memmove(buffer, buffer + 1, 3); \
        buffer[2] = '.';                     \
    }                                        \
    else                                     \
    {                                        \
        std::memmove(buffer, buffer + 1, 3); \
        buffer[3] = '.';                     \
    }


    switch (FreqMeterTimeCounting())
    {
    case FreqMeterTimeCounting::_100ms:

        giverFreq *= 100;

        WRITE_SUFFIX("кГц");

        if (giverFreq < _1MHz)                       // Меньше 1 МГц
        {
            if (freq >= _10Hz)                       // Больше или равно 10 Гц
            {
                std::memmove(buffer, buffer + 1, 5);
            }
            buffer[4] = '.';
        }
        else
        {
            HIGH_FREQ;
        }
        break;

    case FreqMeterTimeCounting::_1s:

        giverFreq *= 10;

        WRITE_SUFFIX("Гц");

        if (giverFreq < _1MHz)                      // Меньше 1 МГц
        {
            if (giverFreq < _1kHz)                   // Меньше 1 кГц
            {
                std::memmove(buffer, buffer + 1, 6);
                buffer[6] = '.';
            }
            else
            {
                std::memmove(buffer, buffer + 1, 4);
                buffer[3] = '.';
            }
        }
        else
        {
            HIGH_FREQ;
        }
        break;

    case FreqMeterTimeCounting::_10s:

        WRITE_SUFFIX("Гц");

        if (freq < _1MHz)                       // Меньше 1 МГц
        {
            if (giverFreq < _1kHz)              // Меньше 1 кГц
            {
                std::memmove(buffer, buffer + 1, 5);
                buffer[5] = '.';
            }
            else if (giverFreq < _100kHz)
            {
                std::memmove(buffer, buffer + 1, 3);
                buffer[2] = '.';
            }
            else
            {
                std::memmove(buffer, buffer + 1, 3);
                buffer[3] = '.';
            }
        }
        else
        {
            HIGH_FREQ;
        }
        break;
    case FreqMeterTimeCounting::Count:
    default:
        LOG_ERROR("");
        break;
    }

    return buffer;
}


pString DisplayFreqMeter::PeriodSetToString(const BitSet32 *pr)
{
    if (pr->word == 0)
    {
        return EMPTY_STRING;
    }
    else if (pr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }
    else
    {
        // все случаи обработаны
    }

    Stack<uint> stack(20);

    uint _period = pr->word;

    while (_period > 0)
    {
        stack.Push(_period % 10);
        _period /= 10;
    }

    int order = LowOrder(set.freq.freqClc, set.freq.numberPeriods);  // В ордер - порядок младшего значащего разряда

    while (stack.Size() < 6)
    {
        stack.Push(0U);                             // Досылаем недостающие нули
    }
    // Теперь в стеке все разряды периода, младший (с порядком order - на дне стека)

    Stack<uint> stackResult(6);                     // Здесь будет храниться шесть итоговых рарядов. Последний - младший

    while (stackResult.Size() < 6)
    {
        stackResult.Push(stack.Pop());
    }
    // Теперь в stackResult все разряды итогового числа. На вершине - младший.
    // Скорректируем его порядок.

    while (!stack.Empty())
    {
        stack.Pop();
        order++;
    }
    // Теперь в order хранится порядок младшего значащего разряда результата (он находится на вершине)

    return StackToString(&stackResult, order);
}


int DisplayFreqMeter::LowOrder(FreqMeterFreqClc::E freqCLC, FreqMeterNumberPeriods::E numPeriods)
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

    //                           | 100к/1 | 1М  /1  |  10М /1   | 100М/1   |            |           |           |           |
    //                           |        | 100к/10 |  1М  /10  | 10М /10  | 100М/10    |           |           |           |
    //                           |        |         |  100к/100 | 1М  /100 | 10М /100   | 100М/100  |           |           |
    //                           |        |         |           | 100к/1к  | 1М  /1к    | 10М /1к   | 100М/1к   |           |
    //                           |        |         |           |          | 100к/10к   | 1М  /10к  | 10М /10к  | 100М/10к  |
    //                           |        |         |           |          |            | 100к/100к | 1М  /100к | 10М /100к | 100М/100к |          |
    //                           |        |         |                                                 100к/          | 1М  /10к  | 10М /10к  | 100М/10к |
    //                                                                                                                                             | 100М/100к
    static const int order[] = { -5,      -6,        -7,        -8,         -9,         -10,        -11,        -12,        -13,      -14,       -15,        -16 };

    return order[freqCLC + numPeriods];
}


pString DisplayFreqMeter::StackToString(Stack<uint> *stack, int order)
{
    static const struct StructOrder
    {
        int first;
        int second;
        const char *s1;
        const char *s2;
    }
    structs[] =
    {
        {3, 6, "нс", "пс"},     // -12
        {1, 4, "мкс", "нс"},    // -11
        {2, 5, "мкс", "нс"},    // -10
        {3, 6, "мкс", "нс"},    // -9
        {1, 4, "мс", "мкс"},    // -8
        {2, 5, "мс", "мкс"},    // -7
        {3, 6, "мс", "мкс"},    // -6
        {1, 4, "с", "мс"},      // -5
        {2, 5, "с", "мс"},      // -4
        {3, 6, "с", "мс"},      // -3
        {4, 4, "с", "с"},       // -2
        {5, 5, "с", "с"},       // -1
        {6, 6, "с", "с"}        // 0
    };

    order += 12;

    const StructOrder &str = structs[order];

    if (stack->NumFirstZeros() < str.first)
    {
        WriteStackToBuffer(stack, str.first, str.s1);
    }
    else
    {
        WriteStackToBuffer(stack, str.second, str.s2);
    }

    return buffer;
}


void DisplayFreqMeter::WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix)
{
    for (int i = 6; i >= 0; i--)
    {
        if (point == i)
        {
            buffer[i] = '.';
            continue;
        }
        buffer[i] = static_cast<char>(stack->Pop()) | 0x30;
    }

    std::strcpy(&buffer[7], suffix);
}


void ProgressBarFreqMeter::Draw(int x, int y)
{
    if (FreqMeterModeView().IsFrequency() && FreqMeter::timeStartMeasureFreq != 0)
    {
        static const float time[FreqMeterTimeCounting::Count] = { 100.0F, 1000.0F, 10000.0F };

        int length = 185;

        float percents = (TIME_MS - FreqMeter::timeStartMeasureFreq) / time[FreqMeterTimeCounting()];

        int width = static_cast<int>(length * percents);

        if (width > length)
        {
            width = length;
        }

        if (FreqMeterTimeCounting().Is100ms() && width > length / 2)
        {
            width = length;
        }

        Region(width, 3).Fill(x, y, Color::FILL);
    }
    else if (FreqMeterModeView().IsPeriod() && FreqMeter::timeStartMeasurePeriod != 0)
    {

    }
    else
    {
        // здесь ничего
    }
}
