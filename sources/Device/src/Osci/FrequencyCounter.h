#pragma once
#include "Utils/Stack.h"


#define FREQ_METER_ENABLED              (set.freq_enabled)
#define FREQ_METER_IS_ENABLED           (FREQ_METER_ENABLED.value == FrequencyCounter::Enabled::On)
#define FREQ_METER_TIMECOUNTING         (set.freq_timeCounting)
#define FREQ_METER_FREQ_CLC             (set.freq_freqClc)
#define FREQ_METER_NUM_PERIODS          (set.freq_numberPeriods)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FrequencyCounter
{
public:
    /// Инициализация.
    static void Init();
    /// Заслать настройки для счётчика частоты
    static void LoadFreqSettings();
    /// Заслать настрйки для счётчика периода
    static void LoadPeriodSettings();

    static void Update();
    /// Возвращает измеренное значение частоты
    static float GetFreq();
    /// Отрисовка значения
    static void Draw();

    /// Время счёта периода.
    struct TimeCounting
    {
        enum E
        {
            _100ms,
            _1s,
            _10s
        } value;
        operator uint8() const { return (uint8)value; }
    };

    /// Частота заполняющих импульсов для счёта частоты.
    struct FreqClc
    {
        enum E
        {
            _100kHz,
            _1MHz,
            _10MHz,
            _100MHz,
            Number
        } value;
        operator uint8() const { return (uint8)value; }
    };

    /// Количество периодов.
    struct NumberPeriods
    {
        enum E
        {
            _1,
            _10,
            _100,
            _1k,
            _10k,
            _100k,
            Number
        } value;
        operator uint8() const { return (uint8)value; }
    };

    struct Enabled
    {
        enum E
        {
            Off,
            On
        } value;
        uint8 ToUINT8() const { return (uint8)value; };
    };

private:
    /// Установить состояние лампочек счётчиков в состояние, соответствующее текущему моменту
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static void LoadSettings();

    static float FreqSetToFreq(const BitSet32 *fr);

    static pString FreqSetToString(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);

    static pString PeriodSetToString(const BitSet32 *pr);
    /// Преобразует 6 разрядов числа, хранящиеся в стеке, в текстовую строку периода. Младший значащий разряд хранится на вершине стека. order - его порядок
    static pString StackToString(Stack<uint> *stack, int order);
    /// Записывает 6 разрядов из стека stack в буфер buffer. Младший разряд на вершине стека. Точку ставить на point позиции, начиная с buffer[0]
    static void WriteStackToBuffer(Stack<uint> *stack, int point, char *suffix);
    /// Возвращает порядок младшего разряда считанного значения счётчика периода при данных настройках
    static int LowOrder(FreqClc freqCLC, NumberPeriods numPeriods);
    /// Здесь хранится последнее действительное значение частоты. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    static BitSet32 freqActual;
    /// Здесь хранится последнее действительное значение периода. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    static BitSet32 periodActual;
    /// Установленное в true значение означает, что частоту нужно считать по счётчику периода
    static bool readPeriod;

    static float prevFreq;

    static float frequency;

    static float period;
    /// Если true - горит лампочка счёта частоты
    static bool lampFreq;
    /// Если false - горит лампочка счёта периода
    static bool lampPeriod;

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
};
