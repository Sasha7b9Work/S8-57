#include <stm32f4xx.h>
#include "defines.h"
#include "log.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FrequencyCounter.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Hardware/FSMC.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <limits>


using namespace Display::Primitives;
using namespace FPGA::ADDR;

using FPGA::HAL::GetFlag;
using Hardware::Timer;
using Utils::Stack;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Здесь хранится последнее действительное значение частоты. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
static BitSet32 freqActual;
/// Здесь хранится последнее действительное значение периода. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
static BitSet32 periodActual;

/// для отладки
/// \todo удалить
static BitSet32 lastFreq;
static BitSet32 lastPeriod;
/// Последнее время разрешшения чтения частоты
static uint lastFreqRead;
/// Последнее время разрешения чтения периода
static uint lastPeriodRead;
/// Последнее время переполнения частоты
static uint lastFreqOver;
/// Последнее время переполения периода
static uint lastPeriodOver;
/// Если true - горит лампочка счёта частоты
static bool lampFreq = false;
/// Если false - горит лампочка счёта периода
static bool lampPeriod = false;;


bool     FrequencyCounter::readPeriod;
float    FrequencyCounter::prevFreq;
float    FrequencyCounter::frequency;

//                         0    1    2    3    4    5    6 
static char buffer[11] = {'0', '0', '0', '0', '0', '0', '0', 0, 0, 0, 0};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Выводит отладочную информацию
static void DrawDebugInfo();

static pString FreqSetToString(const BitSet32 *fr);

static pString PeriodSetToString(const BitSet32 *pr);
/// Возвращает порядок младшего разряда считанного значения счётчика периода при данных настройках
static int LowOrder(FrequencyCounter::FreqClc::E freqCLC, FrequencyCounter::NumberPeriods::E numPeriods);
/// Преобразует 6 разрядов числа, хранящиеся в стеке, в текстовую строку периода. Младший значащий разряд хранится на вершине стека. order - его порядок
static pString StackToString(Utils::Stack<uint> *stack, int order);
/// Записывает 6 разрядов из стека stack в буфер buffer. Младший разряд на вершине стека. Точку ставить на point позиции, начиная с buffer[0]
static void WriteStackToBuffer(Utils::Stack<uint> *stack, int point, const char *suffix);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FrequencyCounter::Init()
{
    LoadSettings();
    FSMC::WriteToFPGA8(WR::RESET_COUNTER_FREQ, 1);
    FSMC::WriteToFPGA8(WR::RESET_COUNTER_PERIOD, 1);
    freqActual.word = periodActual.word = 0;
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::LoadSettings()
{
    uint8 data = 0;

    if (FREQ_METER_IS_ENABLED)
    {
        const uint16 maskTime[3] = {0, 1, 2};
        const uint16 maskFreqClc[4] = {0, (1 << 2), (1 << 3), ((1 << 3) + (1 << 2))};

        DEF_STRUCT(StructPeriod, uint16) maskPeriod[NumberPeriods::Number] =
        { //        654 - задействованные биты
            BIN_U8(00000000), //-V2501
            BIN_U8(00010000), //-V2501
            BIN_U8(00100000), //-V2501
            BIN_U8(00110000), //-V2501
            BIN_U8(01000000), //-V2501 // -V536
            BIN_U8(01010000)  //-V2501
        };

        data |= maskTime[FREQ_METER_TIMECOUNTING];
        data |= maskFreqClc[FREQ_METER_FREQ_CLC];
        data |= maskPeriod[FREQ_METER_NUM_PERIODS].val;
    }
    else
    {
        _SET_BIT(data, 2);
    }

    FSMC::WriteToFPGA8(WR::FREQMETER, data);
}


#ifdef WIN32
#pragma warning(pop)
#endif


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::LoadFreqSettings()
{
    LoadSettings();
    FSMC::WriteToFPGA8(WR::RESET_COUNTER_FREQ, 1);
    freqActual.word = 0;
    lampFreq = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::LoadPeriodSettings()
{
    LoadSettings();
    FSMC::WriteToFPGA8(WR::RESET_COUNTER_PERIOD, 1);
    periodActual.word = 0;
    lampPeriod = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::Update()
{
    SetStateLamps();

    bool freqReady = GetFlag::FREQ_READY();

    if(freqReady)
    {
        lastFreqRead = TIME_MS;
    }

    bool periodReady = GetFlag::PERIOD_READY();

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

    if(GetFlag::FREQ_OVERFLOW())
    {
        freqActual.word = MAX_UINT;
        lastFreqOver = TIME_MS;
    }
    if(GetFlag::PERIOD_OVERFLOW())
    {
        periodActual.word = MAX_UINT;
        lastPeriodOver = TIME_MS;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::ReadFreq()
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::ReadPeriod()
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::FreqSetToFreq(const BitSet32 *fr)
{
    const float k[3] = {10.0F, 1.0F, 0.1F};
    return FREQ_METER_IS_ENABLED ? (fr->word * k[FREQ_METER_TIMECOUNTING]) : (fr->word * 10.0F);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::PeriodSetToFreq(const BitSet32 *period_)
{
    if (period_->word == 0)
    {
        return 0.0F;
    }

    const float k[4] = {10e4F, 10e5F, 10e6F, 10e7F};
    const float kP[3] = {1.0F, 10.0F, 100.0F};

    return FREQ_METER_IS_ENABLED ? (k[FREQ_METER_FREQ_CLC] * kP[FREQ_METER_NUM_PERIODS] / (float)period_->word) : (10e5F / (float)period_->word);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FrequencyCounter::GetFreq()
{
    return frequency;
}

#define SIZE 4

#define EMPTY_STRING    "\xa9\xa9\xa9.\xa9\xa9\xa9"
#define OVERFLOW_STRING ">>>"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawFrequency(int x, int y)
{
    Text("F", SIZE).Draw(x + 2, y + 1, Color::FILL);
    Text("T", SIZE).Draw(x + 2, y + 10 * SIZE);

    Rectangle(10, 10).Draw(x - 20, y);
    if (lampFreq)
    {
        Region(10, 10).Fill(x - 20, y);
    }

    int dX = 7 * SIZE;

    Text("=", SIZE).Draw(x + dX, y + 1);
    Text("=", SIZE).Draw(x + dX, y + 10 * SIZE);

    dX = SIZE * 12;

    char strFreq[50];
    std::strcpy(strFreq, FreqSetToString(&freqActual));

    Text(strFreq, SIZE).Draw(x + dX, y + 1);

    if (std::strcmp(strFreq, EMPTY_STRING) == 0)
    {
        return;
    }

    if (std::strcmp(strFreq, OVERFLOW_STRING) == 0)
    {
        return;
    }

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

    Text(time.ToStringAccuracy(false, strFreq, 6), SIZE).Draw(x + dX, y + 10 * SIZE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPeriod(int x, int y)
{
    Text("T", SIZE).Draw(x + 2, y + 1, Color::FILL);
    Text("F", SIZE).Draw(x + 2, y + 10 * SIZE);

    Rectangle(10, 10).Draw(x - 20, y + 1);
    if (lampPeriod)
    {
        Region(10, 10).Fill(x - 20, y);
    }

    int dX = 7 * SIZE;

    Text("=", SIZE).Draw(x + dX, y + 1);

    Text("=", SIZE).Draw(x + dX, y + 10 * SIZE);

    dX = SIZE * 12;

    char strPeriod[50];
    std::strcpy(strPeriod, PeriodSetToString(&periodActual));

    Text(strPeriod, SIZE).Draw(x + dX, y + 1);

    if ((std::strcmp(strPeriod, EMPTY_STRING) == 0) || (std::strcmp(strPeriod, OVERFLOW_STRING) == 0))
    {
        return;
    }

    float period = SU::StringToFloat(strPeriod);

    if (std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "нс") == 0)
    {
        period *= 1e-9F;
    }
    else if (std::strcmp(&strPeriod[std::strlen(strPeriod) - 3], "мкс") == 0)
    {
        period *= 1e-6F;
    }
    else if (std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "мс") == 0)
    {
        period *= 1e-3F;
    }
    else
    {
        LOG_ERROR("Здесь мы никогда не должны оказаться");
    }

    Frequency freq(1.0F / period);

    Text(freq.ToStringAccuracy(strPeriod, 6), SIZE).Draw(x + dX, y + 10 * SIZE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrequencyCounter::Draw()
{
    /// \todo В этой строке точку ставить не где придётся, а в той позиции, где она стояла последний раз

    if (!FREQ_METER_IS_ENABLED)
    {
        return;
    }

    int width = 241;
    int height = 74;
    int x = Grid::Left() + (Grid::Width() - width) / 2;
    int y = Grid::Top() + (Grid::Height() / 2 - height) / 2;

    
    Region(width - 2, height - 2).Fill(x + 1, y + 1, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    x += 2;
    y += 2;

    if (FREQ_METER_MODE_VIEW_IS_FREQUENCY)
    {

        DrawFrequency(x, y);
    }
    else
    {
        DrawPeriod(x, y);
    }
   
    if(false)
    {
        DrawDebugInfo();
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString PeriodSetToString(const BitSet32 *pr)
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
static int LowOrder(FrequencyCounter::FreqClc::E freqCLC, FrequencyCounter::NumberPeriods::E numPeriods)
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
    static const int order[] =   {  -5,      -6,        -7,        -8,         -9,         -10,        -11,        -12,        -13,      -14,       -15,        -16};

    return order[freqCLC + numPeriods];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pString StackToString(Stack<uint> *stack, int order)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix)
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

    std::strcpy(&buffer[7], suffix);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pString FreqSetToString(const BitSet32 *fr)
{
    if(fr->word == 0)
    {
        return EMPTY_STRING;
    }
    else if(fr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }
    else
    {
        LOG_ERROR("Ошибка");
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


    switch (FREQ_METER_TIMECOUNTING)
    {
        case FrequencyCounter::TimeCounting::_100ms:

            giverFreq *= 100;

            WRITE_SUFFIX("кГц");

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

        case FrequencyCounter::TimeCounting::_1s:

            giverFreq *= 10;

            WRITE_SUFFIX("Гц");

            if (giverFreq < _1MHz)                      // Меньше 1 МГц
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

        case FrequencyCounter::TimeCounting::_10s:

            WRITE_SUFFIX("Гц");

            if (freq < _1MHz)                       // Меньше 1 МГц
            {
                if (giverFreq < _1kHz)              // Меньше 1 кГц
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
        default:
            LOG_ERROR("");
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
        if(GetFlag::FREQ_IN_PROCESS())
        {
            lampFreq = true;
        }
    }
    else
    {
        if(GetFlag::FREQ_READY())
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
        if(GetFlag::PERIOD_IN_PROCESS())
        {
            lampPeriod = true;
        }
    }
    else
    {
        if(GetFlag::PERIOD_READY())
        {
            lampPeriod = false;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDebugInfo()
{
    int width = 50;
    int height = 27;
    int x = 50;
    int y = 120;
    Region(width, height).Fill(x, y, Color::BACK);
    Rectangle(width + 2, height + 2).Draw(x - 1, y - 1, Color::FILL);

    String("%d", freqActual.word).Draw(x + 4, y + 4);
    String("%d", periodActual.word).Draw(x + 4, y + 15);

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

    if (TIME_MS - lastFreqRead < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::BLUE);
    }

    Rectangle(size, size).Draw(x, y + 15, Color::FILL);

    if (TIME_MS - lastPeriodRead < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::BLUE);
    }

    x += 20;

    Rectangle(size, size).Draw(x, y + 4, Color::FILL);

    if (TIME_MS - lastFreqOver < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::RED);
    }

    Rectangle(size, size).Draw(x, y + 15, Color::FILL);

    if (TIME_MS - lastPeriodOver < TIME)
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::RED);
    }

    x += 20;

    if (GetFlag::FREQ_IN_PROCESS())
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 5, Color::FILL);
    }

    if (GetFlag::PERIOD_IN_PROCESS())
    {
        Region(size - 2, size - 2).Fill(x + 1, y + 16, Color::FILL);
    }
}
