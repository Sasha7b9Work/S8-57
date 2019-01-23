#pragma once
#include "FPGA/FPGA_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup Measure
 *  @brief Обработка данных. Здесь отсчёты сигнала преобразуются к выводду на экран и рассчитываются результаты автоматических измерений
 *  @{
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SHOW_MEASURES           (set.meas_show)

#define MEAS_MARKED             (set.meas_marked)

/// По какому каналу производить автоматические измерения
#define SOURCE_MEASURES         (set.meas_source)
/// Автоматические измерения производятся только по каналу A
#define SOURCE_MEASURES_IS_A    (SOURCE_MEASURES == Source::A)
/// Автоматические измерения производятся только по каналу B
#define SOURCE_MEASURES_IS_B    (SOURCE_MEASURES == Source::B)
/// Автоматические измерения производятся по каналам A и B
#define SOURCE_MEASURES_IS_BOTH (SOURCE_MEASURES == Source::A_B)
/// Выводить автоматические измерения по каналу A
#define VIEW_MEASURES_A         (SET_ENABLED_A && (SOURCE_MEASURES_IS_A || SOURCE_MEASURES_IS_BOTH))
/// Выводить автоматические измерения по каналу B
#define VIEW_MEASURES_B         (SET_ENABLED_B && (SOURCE_MEASURES_IS_B || SOURCE_MEASURES_IS_BOTH))
/// Выводить автоматические измерения по обоим каналам
#define VIEW_MEASURES_BOTH      (SET_ENABLED_BOTH && SOURCE_MEASURES_IS_BOTH)


/// Сжимать ли сетку при выводе измерений
#define MODE_VIEW_SIGNALS               (set.meas_modeViewSignals) 
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == ModeViewSignals::Compress)

#define NUM_MEASURES                    (set.meas_number)
#define NUM_MEASURES_IS_1_5             (NUM_MEASURES == OnDisplay::_1_5)
#define NUM_MEASURES_IS_2_5             (NUM_MEASURES == OnDisplay::_2_5)
#define NUM_MEASURES_IS_3_5             (NUM_MEASURES == OnDisplay::_3_5)
#define NUM_MEASURES_IS_6_1             (NUM_MEASURES == OnDisplay::_6_1)
#define NUM_MEASURES_IS_6_2             (NUM_MEASURES == OnDisplay::_6_2)

#define POS_MEAS_CUR_U(n)               (set.meas_PosCurU[n])
#define POS_MEAS_CUR_U_0                (POS_MEAS_CUR_U(0))
#define POS_MEAS_CUR_U_1                (POS_MEAS_CUR_U(1))

#define POS_MEAS_CUR_T(n)               (set.meas_PosCurT[n])
#define POS_MEAS_CUR_T_0                (POS_MEAS_CUR_T(0))
#define POS_MEAS_CUR_T_1                (POS_MEAS_CUR_T(1))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    namespace Measurements
    {
        class Measure
        {
        public:
            // Виды измерений
            struct Type
            {
                enum E
                {
                    None,
                    VoltageMax,
                    VoltageMin,
                    VoltagePic,
                    VoltageMaxSteady,
                    VoltageMinSteady,
                    VoltageAmpl,
                    VoltageAverage,
                    VoltageRMS,
                    VoltageVybrosPlus,
                    VoltageVybrosMinus,
                    Period,
                    Freq,
                    TimeNarastaniya,
                    TimeSpada,
                    DurationPlus,
                    DurationMinus,
                    SkvaznostPlus,
                    SkvaznostMinus,
                    DelayPlus,
                    DelayMinus,
                    PhazaPlus,
                    PhazaMinus,
                    Number
                } value;
                explicit Type(E v) : value(v) {};
            };

            Measure(int r, int c) : row(r), col(c) {};
            /// Возвращает сообщение из ячейки (row, col)
            static Measure Get(int row, int col) { return Measure(row, col); };

            Type::E GetType();

            /// Устанавливает активным следующее или предыдущее измерение
            static void ChangeActive(int delta);
            /// \brief Установить сигнал для обработки. Данные берутся из DS, inA, inB.
            /// Выходные данные, соответствующие текущим настройками set, рассчитываются сразу и записываются в outA, outB.
            static void SetData(bool needSmoothing);
            /// Рассчитать позицию курсора напряжения, соответствующю заданной позиции курсора posCurT
            static float CalculateCursorU(Chan::E ch, float posCurT);
            /// Рассчитать позицию курсора времени, соответствующую заданной позиции курсора напряжения posCurU
            static float CalculateCursorT(Chan::E ch, float posCurU, int numCur);

            static void SetMarkerVoltage(Chan::E ch, int num, float value);

            static void SetMarkerTime(Chan::E ch, int num, int value);

            static char GetChar(Type::E measure);

            static void ShortPressOnSmallButonMarker();
            /// Возвращает true, если измерение активное - выбрано ручкой
            bool IsActive();

            String Name();
            /// Позиция активного измерения (на котором курсор)
            static int8 posActive;
            /// Если true - раскрыта страница выбора измерения
            static bool pageChoiceIsActive;
            /// Позиция курсора на странице выбора измерения
            static int8 posOnPageChoice;

#define MARKER_VOLTAGE(ch, num)             (Processing::markerVoltage[ch][num] - VALUE::MIN)
#define MARKER_TIME(ch, num)                (Processing::markerTime[ch][num])

        private:
            /// Возвращает активное измерение
            static Measure GetActive();
            /// Сделать активным
            static void SetActive(int row, int col);
            /// Строка в таблице, в которой находится данное измерение
            int row;
            /// Колонка в таблице, в которой находится данное измерение
            int col;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class Graphics
        {
        public:
            /// Отрисовать результаты автоматических измерений
            static void Draw();
            /// Возвращает координату x верхнего левого угла таблицы выводимых измерений
            static int GetTop();

            static int NumCols();

            static int NumRows();
            /// На сколько сжимать сетку по горизонтали
            static int GetDeltaGridLeft();
            /// На сколько сжимать сетку по вертикали
            static int GetDeltaGridBottom();
            /// Возвращает высоту поля для вывода автоматического измерения
            static int DY();
            /// Возвращает ширину поля для вывода автоматического измерения
            static int DX();

        private:
            /// Верхняя координата y выводимой таблицы автоматических измерений
            static int top;

            static int GetTopTable();
            /// Нарисовать страницу выбора измерений
            static void DrawPageChoice();
        };
    };
};


/** @}  @}
 */
