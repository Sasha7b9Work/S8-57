#pragma once
#include "Utils/String.h"
#include "Osci/Osci_Settings.h"


/// Позиция курсора напряжения
#define CURsU_POS(ch, num)          (set.curs.posCurU[ch][num])
/// Позиция куросра времени
#define CURsT_POS(ch, num)          Cursors::PosT(ch, num)

/// Режим слежения за курсорами
#define CURS_LOOK_MODE(ch)          (set.curs.lookMode[ch])
/// Включено слежение за курсорами напряжения
#define CURS_LOOK_U(ch)             (CURS_LOOK_MODE(ch) == CursorsLookMode::Voltage)
/// Включено слежение за курсорами времени
#define CURS_LOOK_T(ch)             (CURS_LOOK_MODE(ch) == CursorsLookMode::Time)
/// Включено слежение за курсорами времени и напряжения
#define CURS_LOOK_BOTH(ch)          (CURS_LOOK_MODE(ch) == CursorsLookMode::Both)

#define CURsU_CNTRL_CH(ch)          (set.curs.cntrlU[ch])
#define CURsU_CNTRL                 (CURsU_CNTRL_CH(CURS_SOURCE))
#define CURsU_CNTRL_1               (CURsU_CNTRL == CursorsControl::_1)
#define CURsU_CNTRL_2               (CURsU_CNTRL == CursorsControl::_2)
#define CURsU_CNTRL_1_2             (CURsU_CNTRL == CursorsControl::_1_2)
#define CURsU_DISABLED              (CURsU_CNTRL == CursorsControl::Disable)
#define CURsU_ENABLED               (!CURsU_DISABLED)

#define CURsT_CNTRL_CH(ch)          (set.curs.cntrlT[ch])
#define CURsT_CNTRL                 (CURsT_CNTRL_CH(CURS_SOURCE))
#define CURsT_CNTRL_1               (CURsT_CNTRL == CursorsControl::_1)
#define CURsT_CNTRL_2               (CURsT_CNTRL == CursorsControl::_2)
#define CURsT_CNTRL_1_2             (CURsT_CNTRL == CursorsControl::_1_2)
#define CURsT_DISABLED              (CURsT_CNTRL == CursorsControl::Disable)
#define CURsT_ENABLED               (!CURsT_DISABLED)

#define CURSORS_SHOW_FREQ           (set.curs.showFreq)

/// Курсоры какого канала сейчас активны
#define CURS_SOURCE                 (set.curs.source)
/// Активны курсоры канала A
#define CURS_SOURCE_A               (CURS_SOURCE == Chan::A)
/// Активны курсоры канала B
#define CURS_SOURCE_B               (CURS_SOURCE == Chan::B)

/// Абсолютное значение напряжения, соответствующее 100%
#define dUperc(ch)                  (set.curs.deltaU100percents[ch])
/// Абсолютное значение времени, соотвествующее 100%
#define dTperc(ch)                  (set.curs.deltaT100percents[ch])

#define CURS_SHOW                   (set.curs.showCursors)


namespace Osci
{
    namespace Measurements
    {
        struct Cursors
        {
            /// Нарисовать курсоры и результаты курсорных измерений
            static void Draw();
            /// Возвращает true,если нужно рисовать курсоры
            static bool  NecessaryDraw();
            /// Получить позицию курсора напряжения
            static float PosU(Chan::E ch, int numCur);
            /// Возвращает значение курсора времени
            static float PosT(Chan::E ch, int num);
            /// Получить строку курсора напряжения
            static String Voltage(Chan::E source, int numCur);
            /// Получть строку курсора времени
            static String Time(Chan::E source, int numCur);
            /// Возвращает строку процентных измерений по напряжению
            static String PercentsU(Chan::E source);
            /// Возвращает строку процентных измерений по времени
            static String PercentsT(Chan::E source);

            static void SetCursPosT_temp(Chan::E ch, int num, float value);
        };
    };
};
