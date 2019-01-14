#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == FPGA::SET::Trig::Source::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == FPGA::SET::Trig::Source::B)
#define TRIG_SOURCE_IS_EXT      (TRIG_SOURCE == FPGA::SET::Trig::Source::Ext)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_HAND     (TRIG_MODE_FIND == FPGA::SET::Trig::ModeFind::Hand)
#define TRIG_MODE_FIND_AUTO     (TRIG_MODE_FIND == FPGA::SET::Trig::ModeFind::Auto)

#define START_MODE              (set.trig_startMode)
#define START_MODE_IS_AUTO      (START_MODE == FPGA::SET::Trig::StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == FPGA::SET::Trig::StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == FPGA::SET::Trig::StartMode::Single)

#define TRIG_POLARITY           (set.trig_polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig_input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig_TimeDelay)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace SET
    {
        void Load();

        void TrigLevChange(int delta);

        void SetTShift(int tShift);

        void LoadTShift();

        void TShiftChange(int delta);

        void SetModeCouple(Chan::E ch, ModeCouple::E couple);
        /// Включить/выключить калибратор.
        void LoadCalibratorMode();
        /// Установить значение удержания синхронизации
        void LoadHoldfOff();
       
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Range
        {
            static void Change(Chan::E ch, int delta);

            static void LoadBoth();

            enum E
            {
                _2mV,
                _5mV,
                _10mV,
                _20mV,
                _50mV,
                _100mV,
                _200mV,
                _500mV,
                _1V,
                _2V,
                _5V,
                _10V,
                _20V,
                Number
            } value;
            explicit Range(E v) : value(v) {};
            pString Name() const;

            pString ToString(Divider::E divider);

        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct RShift
        {
            static const int MIN = 20;
            static const int MAX = 980;
            static const int ZERO = 500;

            static void Change(Chan::E ch, int delta);

            static void Set(Chan::E ch, uint16 rShift);

            static void Load(Chan::E ch);

            static void Draw();

            static String ToString(uint16 rShiftRel, FPGA::SET::Range::E range, Divider::E divider);

        private:

            static void Draw(Chan::E ch);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct TBase
        {
            enum E
            {
                _2ns,
                _5ns,
                _10ns,
                _20ns,
                _50ns,
                _100ns,
                _200ns,
                _500ns,
                _1us,
                _2us,
                _5us,
                _10us,
                _20us,
                _50us,
                _100us,
                _200us,
                _500us,
                _1ms,
                _2ms,
                _5ms,
                _10ms,
                _20ms,
                _50ms,
                _100ms,
                _200ms,
                _500ms,
                _1s,
                _2s,
                _5,
                _10s,
                Number
            } value;
            explicit TBase(E v) : value(v) {};

            pString Name() const;

            pString ToString() const;

            static void Load();

            static void Change(int delta);

            /// С этого значения должен включаться режим поточечного вывода
            static const E MIN_P2P = _50ms;
            /// Минимальный масштаб по времени, при котором ещё возможно включение режима пикового детектора
            static const E MIN_PEAK_DET = _200ns;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class Trig
        {
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

            struct Level
            {
                static void Load();
            };

            /// Источник синхронизации
            struct Source
            {
                enum E
                {
                    A,    /// Канал 1
                    B,    /// Канал 2
                    Ext   /// Внешняя
                } value;

                static void Load();
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
            };

            struct ModeFind
            {
                enum E
                {
                    Hand,      ///< Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки.
                    Auto       ///< Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала.
                } value;
            };

            struct Input
            {
                enum E
                {
                    Full,
                    HF,
                    LF
                } value;

                /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
                static void Load();
            };

            struct Polarity
            {
                enum E
                {
                    Rising,
                    Falling
                } value;

                static void Load();
            };

            /// Установленное в true значение означает, что нужно выводить значок синхроимпульса
            static bool pulse;
        };
    };
}
