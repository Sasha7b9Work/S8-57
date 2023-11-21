// 2023/11/21 10:38:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls.h"
#include "defines.h"


namespace Keyboard
{
    void Init();

    void Update();

    //  оличество выводов RL дл€ опроса клавиатуры
    static const int NUM_RL = 6;

    //  оличество выводов SL дл€ опроса клавиатуры
    static const int NUM_SL = 8;

    // ¬озращает им€ органа управлени€
    const char *ControlName(Control control);

    bool *GetStateControls();

    //                         sl rl
    void GetSL_RL(bool[NUM_SL][NUM_RL]);
};
