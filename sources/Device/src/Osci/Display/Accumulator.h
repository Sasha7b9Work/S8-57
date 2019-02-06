#pragma once


namespace Osci
{
    namespace Display
    {
        class Accumulator
        {
        public:
            /// Ёту функцию нужно вызывать после каждой отрисовки сигналов
            static void NextDraw();
            /// —брос информации
            static void Reset();
        };
    }
}
