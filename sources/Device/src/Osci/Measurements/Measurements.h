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
        void SetData(bool needSmoothing);
        /// Расчитать все автоматические измерения
        void CalculateMeasures();
        /// Возвращает строку автоматического измерения
        String GetStringMeasure(Measurements::Measure::Type::E measure, Chan::E ch, char *buffer, int lenBuf);
        /// Аппроксимировать единичное измерение режима рандомизатора функцией sinX/X
        void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
        /// Возвращает активное измерение
        Measure GetActiveMeasure();

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
            static void OnKeyEvent(KeyEvent event);
            /// Нужно вызывать для открытия/закрытия страницы выбора
            static void OnOpenCloseEvent();

            static bool IsActive();
            /// Включить/отключить отображение
            static void ChangeState();
            /// Позиция курсора
            static int8 posCursor;
        };
    };
};
