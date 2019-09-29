#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    namespace Measurements
    {
        /// \brief Установить сигнал для обработки. Данные берутся из DS, inA, inB.
        /// Выходные данные, соответствующие текущим настройками set, рассчитываются сразу и записываются в outA, outB.
        void SetData();
        /// Возвращает true, если данные установлены
        bool DataIsSetting();
        /// Расчитать все автоматические измерения
        void CalculateMeasures();
        /// Возвращает активное измерение
        Measure GetActiveMeasure();

        char* Freq2String(float, bool, char buffer[20]);

        char* Float2String(float, bool, char buffer[20]);

        extern int markerTime[Chan::Size][2];

        extern int markerVoltage[Chan::Size][2];
        /// Позиция активного измерения (на котором курсор)
        extern int8 posActive;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Страница выбора измерений
        class PageChoice
        {
        public:
            /// Обработка нажатий при открытой странице
            static void OnKeyEvent(const KeyEvent &event);
            /// Нужно вызывать для открытия/закрытия страницы выбора
            static void OnOpenCloseEvent();
            /// Включить/отключить отображение
            static void ChangeState();

            static void Draw();
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
