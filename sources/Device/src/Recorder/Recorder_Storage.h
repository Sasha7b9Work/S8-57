#pragma once
#include "Data/DataSettings.h"


namespace Recorder
{
    namespace Storage
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Точка на графике
        struct Point
        {
            Point(BitSet16 _data) : data(_data) {};
            static Point CreateEmpty();
            bool IsEmpty();
            int Min();
            int Max();
        private:
            BitSet16 data;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Описывает данные регистратора - цельную запись точек
        struct Frame
        {
            Frame() : start(0), numPoints(0), pointer(0) {} //-V730

            void SetDataAddress(uint16 *address);
            /// Добавление считаной точки
            void AddPoint(BitSet16 dataA, BitSet16 dataB);
            /// Число точек в регистрограмме
            uint NumPoints();
            /// Получить точку в позиции position
            Point GetPoint(uint position, uint maxPoints);
            /// Получить следующую точку
            Point NextPoint(uint maxPoints);
            /// Время записи первой точки
            PackedTime timeStart;
        private:
            /// Указатель на буфер данных - фактически адрес первой сохранённой точки
            BitSet16 *start;
            /// Количество сохранённых точек
            uint numPoints;
            /// Указатель на последние считаннные данные
            uint pointer;
        };


        /// Инициализация. Выполняется при включении режима "РЕГИСТРАТОР"
        void Init();

        void CreateNewFrame();

        Frame *CurrentFrame();
    };
}
