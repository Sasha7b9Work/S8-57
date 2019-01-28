#pragma once
#include "FPGA/FPGA_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup Measure
 *  @brief ќбработка данных. «десь отсчЄты сигнала преобразуютс€ к выводду на экран и рассчитываютс€ результаты автоматических измерений
 *  @{
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SHOW_MEASURES           (set.meas_show)

#define MEAS_MARKED             (set.meas_marked)

/// ѕо какому каналу производить автоматические измерени€
#define SOURCE_MEASURES         (set.meas_source)
/// јвтоматические измерени€ производ€тс€ только по каналу A
#define SOURCE_MEASURES_IS_A    (SOURCE_MEASURES == Source::A)
/// јвтоматические измерени€ производ€тс€ только по каналу B
#define SOURCE_MEASURES_IS_B    (SOURCE_MEASURES == Source::B)
/// јвтоматические измерени€ производ€тс€ по каналам A и B
#define SOURCE_MEASURES_IS_BOTH (SOURCE_MEASURES == Source::A_B)
/// ¬ыводить автоматические измерени€ по каналу A
#define VIEW_MEASURES_A         (SET_ENABLED_A && (SOURCE_MEASURES_IS_A || SOURCE_MEASURES_IS_BOTH))
/// ¬ыводить автоматические измерени€ по каналу B
#define VIEW_MEASURES_B         (SET_ENABLED_B && (SOURCE_MEASURES_IS_B || SOURCE_MEASURES_IS_BOTH))
/// ¬ыводить автоматические измерени€ по обоим каналам
#define VIEW_MEASURES_BOTH      (SET_ENABLED_BOTH && SOURCE_MEASURES_IS_BOTH)


/// —жимать ли сетку при выводе измерений
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
            // ¬иды измерений
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
            /// ¬озвращает сообщение из €чейки (row, col)
            static Measure Get(int row, int col) { return Measure(row, col); };

            Type::E GetType();

            /// ”станавливает активным следующее или предыдущее измерение
            static void ChangeActive(int delta);
            /// –ассчитать позицию курсора напр€жени€, соответствующю заданной позиции курсора posCurT
            static float CalculateCursorU(Chan::E ch, float posCurT);
            /// –ассчитать позицию курсора времени, соответствующую заданной позиции курсора напр€жени€ posCurU
            static float CalculateCursorT(Chan::E ch, float posCurU, int numCur);

            static void SetMarkerVoltage(Chan::E ch, int num, float value);

            static void SetMarkerTime(Chan::E ch, int num, int value);

            static char GetChar(Type::E measure);

            static void ShortPressOnSmallButonMarker();
            /// ¬озвращает true, если измерение активное - выбрано ручкой
            bool IsActive();

            String Name();
            /// ѕозици€ курсора на странице выбора измерени€
            static int8 posOnPageChoice;

#define MARKER_VOLTAGE(ch, num)             (Processing::markerVoltage[ch][num] - VALUE::MIN)
#define MARKER_TIME(ch, num)                (Processing::markerTime[ch][num])

        private:
            
            /// —делать активным
            static void SetActive(int row, int col);
            /// —трока в таблице, в которой находитс€ данное измерение
            int row;
            ///  олонка в таблице, в которой находитс€ данное измерение
            int col;
        };
    };
};


/** @}  @}
 */
