#pragma once
#include "Settings/SettingsChannel.h"
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RShift
{
public:
    static const int MIN = 20;
    static const int MAX = 980;
    static const int ZERO = 500;

    static void Draw();

    static String ToString(uint16 rShiftRel, Range::E range, Divider::E divider);

private:

    static void Draw(Chan::E ch);
};
