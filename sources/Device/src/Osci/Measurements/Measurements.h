#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"



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

        
        /// Страница выбора измерений
        namespace PageChoice
        {
            /// Обработка нажатий при открытой странице
            void OnKeyEvent(const KeyEvent &event);
            /// Нужно вызывать для открытия/закрытия страницы выбора
            void OnOpenCloseEvent();
            /// Включить/отключить отображение
            void ChangeState();

            void Draw();
        };

        
        namespace Cursors
        {
            /// Нарисовать курсоры и результаты курсорных измерений
            void Draw();
            /// Возвращает true,если нужно рисовать курсоры
            bool  NecessaryDraw();
            /// Получить позицию курсора напряжения
            float PosU(Chan::E ch, int numCur);
            /// Возвращает значение курсора времени
            float PosT(Chan::E ch, int num);
            /// Получить строку курсора напряжения
            String Voltage(Chan::E source, int numCur);
            /// Получть строку курсора времени
            String Time(Chan::E source, int numCur);
            /// Возвращает строку процентных измерений по напряжению
            String PercentsU(Chan::E source);
            /// Возвращает строку процентных измерений по времени
            String PercentsT(Chan::E source);

            void SetCursPosT_temp(Chan::E ch, int num, float value);
        };

        namespace Table
        {
            /// Отрисовать результаты автоматических измерений
            void Draw();

            int NumCols();

            int NumRows();
            /// На сколько сжимать сетку по горизонтали
            int GetDeltaGridLeft();
            /// Возвращает высоту поля для вывода автоматического измерения
            int DY();
            /// Возвращает ширину поля для вывода автоматического измерения
            int DX();


            class Cell
            {
            public:
                Cell(int _row, int _col) : row(_row), col(_col) {};
                void Draw(int x, int y);

            private:
                void DrawStringMeasure(int x, int y);

                int row;
                int col;
            };
        };
    };
};
