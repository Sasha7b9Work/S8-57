#pragma once
#include "Utils/String.h"
#include "Osci/Osci_Settings.h"


#define CURsT_CNTRL_CH(ch)          (set.curs.cntrlT[ch])
#define CURsT_CNTRL                 (CURsT_CNTRL_CH(set.curs.source))
#define CURsT_CNTRL_1               (CURsT_CNTRL == CursorsControl::_1)
#define CURsT_CNTRL_2               (CURsT_CNTRL == CursorsControl::_2)
#define CURsT_CNTRL_1_2             (CURsT_CNTRL == CursorsControl::_1_2)
#define CURsT_DISABLED              (CURsT_CNTRL == CursorsControl::Disable)
#define CURsT_ENABLED               (!CURsT_DISABLED)


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
