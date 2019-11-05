#pragma once
#include "defines.h"


struct Control
{
    enum E
    {
        K_None,
        K_Function,
        K_Measures,
        K_Memory,
        K_Service,
        K_ChannelA,
        K_ChannelB,
        K_Time,
        K_Start,
        K_Trig,
        K_Display,
        K_RangeMoreA,
        B_RangeLessA,
        B_RShiftMoreA,
        B_RShiftLessA,
        B_RangeMoreB,
        B_RangeLessB,
        B_RShiftMoreB,
        B_RShiftLessB,
        B_TBaseMore,
        B_TBaseLess,
        B_TShiftMore,
        B_TShiftLess,
        B_TrigLevMore,
        B_TrigLevLess,
        B_Left,
        B_Right,
        B_Up,
        B_Down,
        B_Enter,
        B_1,
        B_2,
        B_3,
        B_4,
        B_5,
        Count
    } value;

    struct Action
    {
        enum E
        {
            Press,
            Repeat,
            Release,
            Long
        } value;
        Action(E v) : value(v) {};
    } action;

    Control(E v = K_None, Action::E a = Action::Press) : value(v), action(a) {};
    bool IsRepeatable() const;
    operator uint8() const { return static_cast<uint8>(value); };
};
