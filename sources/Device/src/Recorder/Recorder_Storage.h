#pragma once


namespace Recorder
{
    struct Storage
    {
        /// Описывает данные регистратора - цельную запись точек
        struct Data
        {
            /// Число точек в регистрограмме
            uint NumPoints();
        };
        /// Инициализация. Выполняется при включении режима "РЕГИСТРАТОР"
        static void Init();
        /// Добавление считаной точки
        static void AddPoint(uint16 dataA, uint16 dataB);

        static Data &CurrentFrame();
    };
}
