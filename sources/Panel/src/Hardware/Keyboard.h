#pragma once
#include "Controls.h"
#include "defines.h"


struct Keyboard
{
    static void Init();

    static void Update();
    //  оличество выводов RL дл€ опроса клавиатуры
    static const int NUM_RL = 6;
    //  оличество выводов SL дл€ опроса клавиатуры
    static const int NUM_SL = 8;
    // ¬озращает им€ органа управлени€
    static const char *ControlName(Control control);
};
