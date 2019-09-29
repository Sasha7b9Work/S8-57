#pragma once


namespace Osci
{
    namespace Measurements
    {
        namespace Cursors
        {
            /// Дискретность перемещения курсоров.
            struct Movement
            {
                enum E
                {
                    Pixels,    ///< По пикселям экрана
                    Percents   ///< По процентам
                } value;
                explicit Movement(E v) : value(v) {};
            };
        }
    }
}
