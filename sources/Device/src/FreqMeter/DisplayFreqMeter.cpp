#include "defines.h"
#include "log.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FreqMeter/DisplayFreqMeter.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include <cstring>
#include <limits>


namespace DisplayFreqMeter
{
#define EMPTY_STRING    "---.---"
#define OVERFLOW_STRING ">>>"


    //                         0    1    2    3    4    5    6 
    static char buffer[11] = { '0', '0', '0', '0', '0', '0', '0', 0, 0, 0, 0 };

    static bool needSendToSCPI = false;

    static pString FreqSetToString(const BitSet32 *fr);

    static pString PeriodSetToString(const BitSet32 *pr);

    // Возвращает порядок младшего разряда считанного значения счётчика периода при данных настройках
    static int LowOrder(FreqMeter::FreqClc::E freqCLC, FreqMeter::NumberPeriods::E numPeriods);

    // Преобразует 6 разрядов числа, хранящиеся в стеке, в текстовую строку периода. Младший значащий разряд хранится на вершине стека. order - его порядок
    static pString StackToString(Stack<uint> *stack, int order);

    // Записывает 6 разрядов из стека stack в буфер buffer. Младший разряд на вершине стека. Точку ставить на point позиции, начиная с buffer[0]
    static void WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix);

    static void DrawFrequencyMode(int x, int _y);

    static void DrawPeriodMode(int x, int y);
}


void DisplayFreqMeter::Update() //-V2506
{
    // \todo В этой строке точку ставить не где придётся, а в той позиции, где она стояла последний раз

    if(!S_FREQ_METER_ENABLED)
    {
        return;
    }

    if (SCPI::Sender::freqMeter)
    {
        needSendToSCPI = true;
        SCPI::Sender::freqMeter = false;
    }

    DFont::Set(DTypeFont::_GOST28);
    int spacing = DFont::GetSpacing();
    DFont::SetSpacing(1);

    int width = 200;
    int height = S_FREQ_MODE_MEASURE_IS_FREQUENCY ? 90 : 75;

    int x = Grid::Left() + (Grid::Width() - width) / 2;
    int y = Grid::Top() + (Grid::Height() / 2 - height) / 2;

    if (S_MULT_SHOW_ALWAYS)
    {
        y += DisplayMultimeter::HEIGHT;
    }

    if(S_FREQ_MODE_MEASURE_IS_FREQUENCY)
    {
        y += 7;
    }


    Region(width - 2, height - 2).Fill(x + 1, y + 1, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    x += 2;
    y += 2;

    if(S_FREQ_MODE_MEASURE_IS_FREQUENCY)
    {
        DrawFrequencyMode(x, y);
    }
    else
    {
        DrawPeriodMode(x, y);
    }

    if(false)
    {
        FreqMeter::DrawDebugInfo();
    }

    DFont::Pop();
    DFont::SetSpacing(spacing);
    DFont::SetMinWidth(0);

    needSendToSCPI = false;
}


// Конверитерует строку с текстовым значением в абсолютное значение
static float ConvertFrequencyToAbs(const char *strFreq)
{
    float result = SU::StringToFloat(strFreq);

    if(std::strcmp(&strFreq[std::strlen(strFreq) - 3], "МГц") == 0) //-V2513
    {
        result *= 1e6F;
    }
    else if(std::strcmp(&strFreq[std::strlen(strFreq) - 3], "кГц") == 0) //-V2513
    {
        result *= 1e3F;
    }
    else if(std::strcmp(&strFreq[std::strlen(strFreq) - 3], "мГц") == 0) //-V2513 //-V2516
    {
        result *= 1e-3F;
    }

    return result;
}


void DisplayFreqMeter::DrawFrequencyMode(int x, int _y)
{
    _y += 4;

    int yF = _y;
    int yT = _y + 4 + DFont::GetHeight();

    x += 6;

    Text("F").Draw(x, yF, Color::FILL);
    Text("T").Draw(x, yT);

    ProgressBarFreqMeter::Draw(x, yT + 4 + DFont::GetHeight());

    int dX = 17;

    Text("=").Draw(x + dX, yF);
    Text("=").Draw(x + dX, yT);

    dX = 32;

    char strFreq[50];
    std::strcpy(strFreq, FreqSetToString(&FreqMeter::freqActual)); //-V2513

    Text(strFreq).DrawDigitsMonospace(x + dX, yF, DFont::GetWidth('0'));

    if (needSendToSCPI)
    {
        SCPI::SendMeasure(String("F=%s", strFreq));
    }

    float freq = ConvertFrequencyToAbs(strFreq);

    Time time(1.0F / freq);

    if(time.Value() == std::numeric_limits<float>::infinity())
    {
        Text(EMPTY_STRING).DrawDigitsMonospace(x + dX, yT, DFont::GetWidth('0'));

        if (needSendToSCPI)
        {
            SCPI::SendMeasure(String("T=%s", EMPTY_STRING));
        }
    }
    else
    {
        Text(time.ToStringAccuracy(false, strFreq, 6)).DrawDigitsMonospace(x + dX, yT, DFont::GetWidth('0'));

        if (needSendToSCPI)
        {
            SCPI::SendMeasure(String("T=%s", time.ToStringAccuracy(false, strFreq, 6)));
        }
    }
}


// Конверитерует строку с текстовым значением в абсолютное значение
static float ConvertPeriodToAbs(const char *strPeriod)
{
    float result = SU::StringToFloat(strPeriod);

    if(std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "нс") == 0) //-V2513
    {
        result *= 1e-9F;
    }
    else if(std::strcmp(&strPeriod[std::strlen(strPeriod) - 3], "мкс") == 0) //-V2513
    {
        result *= 1e-6F;
    }
    else if(std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "мс") == 0) //-V2513 //-V2516
    {
        result *= 1e-3F;
    }

    return result;
}


void DisplayFreqMeter::DrawPeriodMode(int x, int _y)
{
    _y += 4;

    int yT = _y;
    int yF = _y + 4 + DFont::GetHeight();

    x += 6;

    Text("T").Draw(x, yT, Color::FILL);
    Text("F").Draw(x, yF);

    ProgressBarFreqMeter::Draw(x, yF + 4 + DFont::GetHeight());

    int dX = 17;

    Text("=").Draw(x + dX, yT);

    Text("=").Draw(x + dX, yF);

    dX = 32;

    char strPeriod[50];
    std::strcpy(strPeriod, PeriodSetToString(&FreqMeter::periodActual)); //-V2513

    Text(strPeriod).DrawDigitsMonospace(x + dX, yT, DFont::GetWidth('0'));

    if (needSendToSCPI)
    {
        SCPI::SendMeasure(String("T=%s", strPeriod));
    }

    Text strFreq(strPeriod);

    if((std::strcmp(strPeriod, EMPTY_STRING) != 0) && (std::strcmp(strPeriod, OVERFLOW_STRING) != 0)) //-V2513
    {
        float period = ConvertPeriodToAbs(strPeriod);

        strFreq = Frequency(1.0F / period).ToStringAccuracy(strPeriod, 6);
    }

    strFreq.DrawDigitsMonospace(x + dX, yF, DFont::GetWidth('0'));

    if (needSendToSCPI)
    {
        SCPI::SendMeasure(String("F=%s", strFreq.c_str()));
    }
}


static pString DisplayFreqMeter::FreqSetToString(const BitSet32 *fr) //-V2506
{
    if(fr->word < 2)
    {
        return EMPTY_STRING;
    }
    else if(fr->word == MAX_UINT) //-V2516
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

    // Это герцы * 10
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


    switch(S_FREQ_TIME_COUNTING)
    {
    case FreqMeter::TimeCounting::_100ms:

        giverFreq *= 100;

        WRITE_SUFFIX("кГц"); //-V2513

        if(giverFreq < _1MHz)                       // Меньше 1 МГц
        {
            if(freq >= _10Hz)                       // Больше или равно 10 Гц
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

    case FreqMeter::TimeCounting::_1s:

        giverFreq *= 10;

        WRITE_SUFFIX("Гц"); //-V2513

        if(giverFreq < _1MHz)                      // Меньше 1 МГц
        {
            if(giverFreq < _1kHz)                   // Меньше 1 кГц
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

    case FreqMeter::TimeCounting::_10s:

        WRITE_SUFFIX("Гц"); //-V2513

        if(freq < _1MHz)                       // Меньше 1 МГц
        {
            if(giverFreq < _1kHz)              // Меньше 1 кГц
            {
                std::memmove(buffer, buffer + 1, 5);
                buffer[5] = '.';
            }
            else if(giverFreq < _100kHz)
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
    case FreqMeter::TimeCounting::Count:
        // здесь ничего
        break;
    }

    return buffer;
}


int DisplayFreqMeter::LowOrder(FreqMeter::FreqClc::E freqCLC, FreqMeter::NumberPeriods::E numPeriods)
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


static pString DisplayFreqMeter::PeriodSetToString(const BitSet32 *pr) //-V2506
{
    if(pr->word == 0)
    {
        return EMPTY_STRING;
    }
    else if(pr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }
    else
    {
        // все случаи обработаны
    }

    Stack<uint> stack(20);

    uint _period = pr->word;

    while(_period > 0)
    {
        stack.Push(_period % 10);
        _period /= 10;
    }

    int order = LowOrder(S_FREQ_FREQ_CLC, S_FREQ_NUMBER_PERIODS);  // В ордер - порядок младшего значащего разряда

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

    if(stack->NumFirstZeros() < str.first)
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
    for(int i = 6; i >= 0; i--)
    {
        if(point == i)
        {
            buffer[i] = '.';
            continue;
        }
        buffer[i] = static_cast<char>(stack->Pop()) | 0x30;
    }

    std::strcpy(&buffer[7], suffix); //-V2513
}


void ProgressBarFreqMeter::Draw(int x, int y)
{
    if(S_FREQ_MODE_MEASURE_IS_FREQUENCY && (FreqMeter::timeStartMeasureFreq != 0))
    {
        static const float time[FreqMeter::TimeCounting::Count] = { 100.0F, 1000.0F, 10000.0F };

        int length = 185;

        float percents = (TIME_MS - FreqMeter::timeStartMeasureFreq) / time[S_FREQ_TIME_COUNTING];

        int width = static_cast<int>(length * percents);

        if(width > length)
        {
            width = length;
        }

        if(S_FREQ_TIME_COUNTING_IS_100ms && (width > length / 2))
        {
            width = length;
        }

        Region(width, 3).Fill(x, y, Color::FILL);
    }
    else if(S_FREQ_MODE_MEASURE_IS_PERIOD && (FreqMeter::timeStartMeasurePeriod != 0)) //-V2516
    {

    }
}
