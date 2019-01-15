#pragma once


namespace Osci
{
    namespace Settings
    {
        /// Загружает настройки в аппаратную часть
        void Load();
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

            static String ToString(uint16 rShiftRel, Osci::Settings::Range::E range, Divider::E divider);

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
        namespace Trig
        {
            static const int MIN = 20;
            static const int MAX = 980;
            static const int ZERO = 500;
            /// Возвращает true в случае наличия синхроимпульса
            bool SyncPulse();
            /// Вывести уровень синхронизации поверх сетки
            void DrawOnGrid();
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

                static void Load();
            };

            struct Level
            {
                static void Load();

                static void Change(int delta);
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

            extern bool pulse;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class TShift
        {
        public:
            static int Min();
            static int Zero();
            static int Max();

            TShift(int tshift);

            int InPoints();

            static void Set(int tShift);

            static void Load();

            static void Change(int delta);

            operator int() const { return value; };

            String ToString(Osci::Settings::TBase::E tBase) const;

        private:

            static int value;
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
        };

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
    }
}
