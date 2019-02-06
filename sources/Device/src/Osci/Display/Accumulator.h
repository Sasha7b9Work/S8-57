#pragma once


namespace Osci
{
    namespace Display
    {
        class Accumulator
        {
        public:
            /// Ёту функцию нужно вызывать после каждой отрисовки сигналов
            static void NextFrame();
            /// —брос информации
            static void Reset();
        };
    }
}
