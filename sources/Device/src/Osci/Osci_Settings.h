#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_RSHIFT(ch)          (Osci::Settings::RShift::Value(ch))
#define SET_RSHIFT_A            (SET_RSHIFT(Chan::A))
#define SET_RSHIFT_B            (SET_RSHIFT(Chan::B))

#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == Trig::Source::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == Trig::Source::B)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_IS_HAND  (TRIG_MODE_FIND == Osci::Settings::Trig::ModeFind::Hand)
#define TRIG_MODE_FIND_IS_AUTO  (TRIG_MODE_FIND == Osci::Settings::Trig::ModeFind::Auto)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    namespace Settings
    {
        /// Установить значение удержания синхронизации
        void LoadHoldfOff();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Range
        {
            static void Change(Chan::E ch, int delta);
            /// Загружаться эта настройка может только для обоих каналов одновременно
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
                Size
            } value;
            explicit Range(E v) : value(v) {};
            pString Name() const;

            static void Set(Chan::E ch, E range);

            pString ToString(int8 divider);

        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct RShift
        {
            /// Это значение соответствует минимуму смещения
            static const int MIN = 20;
            /// Это значение соотвествует максимуму смещения
            static const int MAX = 980;
            /// Это значение соответствует середине экрана
            static const int ZERO = 500;
            /// Изменить на delta
            static void Change(Chan::E ch, int delta);
            /// Установить значение
            static void Set(Chan::E ch, uint16 rShift);
            /// Загрузить в аппаратуру
            static void Load(Chan::E ch);
            /// Отрисовать оба на экране
            static void DrawBoth();
            /// Преобразовать в строку
            static String ToString(uint16 rShiftRel, Osci::Settings::Range::E range, int8 divider);
            /// Возвращает ссылку на значение
            static uint16 &Value(Chan::E ch);
        private:
            /// Отрисовать маркер вертикального смещения на сетке
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
                Size
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
        namespace Trig
        {
            /// Возвращает true в случае наличия синхроимпульса
            bool SyncPulse();
            /// Вывести уровень синхронизации поверх сетки
            void DrawOnGrid();
            /// После вызова этой функции уровень синхронизации будет отрисовываться поверх сигнала в течение timMS миллисекунд
            static void NeedForDraw();

            /// Источник синхронизации
            struct Source
            {
                enum E
                {
                    A,    /// Канал 1
                    B     /// Канал 2
                } value;

                static void Load();
            };

            struct Level
            {
                static const int MIN = 20;
                static const int MAX = 980;
                static const int ZERO = 500;
                /// Загрузить уровень синхронизации в аппаратную часть
                static void Load();
                /// Изменить уровень синхронизации на delta единиц
                static void Change(int delta);
                /// Установить заданный уровень синхронизации
                static void Set(int level);
                /// Однократно найти уровень синхронизации
                static void Find();
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
                explicit StartMode(E v) : value(v) {};
            };

            struct ModeFind
            {
                enum E
                {
                    Hand,      ///< Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки.
                    Auto       ///< Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала.
                } value;
                explicit ModeFind(E v) : value(v) {};
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
                explicit Polarity(E v) : value(v) {};

                static void Load();
            };

            extern bool pulse;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class TShift
        {
        public:
            static int Min();
            static int Zero();
            static int Max();

            int InPoints();

            static void Set(int tShift);

            static void Load();

            static void Change(int delta);

            static String ToString(Osci::Settings::TBase::E tBase);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct PeakDetMode
        {
            enum E
            {
                Disabled,
                Enabled,
                Average
            } value;
            explicit PeakDetMode(E v) : value(v) {};
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        namespace Memory
        {
            /// Число точек сигнала, с которым идёт работа.
            struct ENumPointsFPGA
            {
                enum E
                {
                    _512,
                    _1k,
                    _2k,
                    _4k,
                    _8k,
                    Size
                } value;
                explicit ENumPointsFPGA(E v) : value(v) {};
                uint BytesInChannel(PeakDetMode::E mode) const;
                uint PointsInChannel() const;
            };
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Положение точки синхронизация на сигнале.
        struct TPos
        {
            enum E
            {
                Left,          ///< Привязка к левому краю.
                Center,        ///< Привязка к центру.
                Right          ///< Привязка к правому краю.
            } value;
            explicit TPos(E v) : value(v) {};
            /// Узнать привязку отсительно уровня синхронизации в байтах
            int InBytes();
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Тип выборки для режима рандомизатора.
        struct SampleType
        {
            enum E
            {
                Real,    ///< реальное время - в построении участвуют только реально считанные точки, ничего не рассчитывается.
                Equal    ///< эквивалентная - сигнал строится по последним точкам, полученным от рандомизатора.
            } value;
            explicit SampleType(E v) : value(v) {};
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Функция ВР/ДЕЛ.
        struct FunctionTime
        {
            enum E
            {
                Time,
                ShiftInMemory
            } value;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Тип привязки к смещению по горизонтали
        struct LinkingTShift
        {
            enum E
            {
                Time,     ///< Смещение привязано к времени
                Position  ///< Смещение привязано к позиции
            } value;
        };
    }
}
