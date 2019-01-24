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

            /// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
            struct Active
            {
                enum E
                {
                    U,
                    T,
                    None
                } value;
                explicit Active(E v) : value(v) {};
            };

            /// Режим слежения курсоров.
            struct LookMode
            {
                enum E
                {
                    None,      ///< Курсоры не следят.
                    Voltage,   ///< Курсоры следят за напряжением автоматически.
                    Time,      ///< Курсоры следят за временем автоматически.
                    Both       ///< Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
                } value;
                explicit LookMode(E v) : value(v) {};
            };

            /// Каким курсором управлять
            struct Control
            {
                enum E
                {
                    _1,        ///< первым
                    _2,        ///< вторым
                    _1_2,      ///< обоими
                    Disable    ///< никаким
                } value;
                explicit Control(E v) : value(v) {};
            };
        }
    }
}
