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
        /// Аппроксимировать единичное измерение режима рандомизатора функцией sinX/X
        void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
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
    };
};
