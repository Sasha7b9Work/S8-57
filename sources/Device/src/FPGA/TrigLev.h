#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == Trig::Source::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == Trig::Source::B)
#define TRIG_SOURCE_IS_EXT      (TRIG_SOURCE == Trig::Source::Ext)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_HAND     (TRIG_MODE_FIND == Trig::ModeFind::Hand)
#define TRIG_MODE_FIND_AUTO     (TRIG_MODE_FIND == Trig::ModeFind::Auto)

#define START_MODE              (set.trig_startMode)
#define START_MODE_IS_AUTO      (START_MODE == Trig::StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == Trig::StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == Trig::StartMode::Single)

#define TRIG_POLARITY           (set.trig_polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig_input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig_TimeDelay)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Trig
{

friend class FPGA;

public:
    static const int MIN = 20;
    static const int MAX = 980;
    static const int ZERO = 500;
    /// Возвращает true в случае наличия синхроимпульса
    static bool SyncPulse();
    /// Вывести уровень синхронизации поверх сетки
    static void DrawOnGrid();
    /// После вызова этой функции уровень синхронизации будет отрисовываться поверх сигнала в течение timMS миллисекунд
    static void NeedForDraw(uint timeMS);

    /// Источник синхронизации
    struct Source
    {
        enum E
        {
            A,    /// Канал 1
            B,    /// Канал 2
            Ext   /// Внешняя
        } value;
    };

    /// Режим запуска.
    struct StartMode
    {
        enum E
        {
            Auto,     ///< Автоматический.
            Wait,     ///< Ждущий.
            Single    ///< Однократный.
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct ModeFind
    {
        enum E
        {
            Hand,      ///< Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки.
            Auto       ///< Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала.
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct Input
    {
        enum E
        {
            Full,
            HF,
            LF
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct Polarity
    {
        enum E
        {
            Rising,
            Falling
        } value;
        operator uint8() const { return (uint8)value; };
    };

private:
    /// Установленное в true значение означает, что нужно выводить значок синхроимпульса
    static bool pulse;
};
