#pragma once
#include "Controls.h"
#include "defines.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:

    static void Init();

    static void Update();
    ///  оличество выводов RL дл€ опроса клавиатуры
    static const int NUM_RL = 6;
    ///  оличество выводов SL дл€ опроса клавиатуры
    static const int NUM_SL = 8;
    /// ¬озращает им€ органа управлени€
    static const char *ControlName(Control control);

private:

    static void FillCommand(Control control, Control::Action::E action);

    static uint TimeBetweenRepeats(uint time);

    static Control commands[10];

    static int pointer;
    /// ѕри обнаружении нажати€ кнопки сюда записываетс€ врем€ нажати€
    static uint timePress[NUM_RL][NUM_SL];
    /// ”становленное в true значение означает, что сохран€ть куда-либо информацию о еЄ состо€нии нельз€ до отпускани€ (чтобы не было ложных
    /// срабатываний типа Long
    static bool alreadyLong[NUM_RL][NUM_SL];

    static bool init;
};
