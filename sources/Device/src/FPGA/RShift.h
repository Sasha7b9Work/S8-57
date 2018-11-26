#pragma once
#include "Settings/SettingsChannel.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RShift
{
public:
    static const int MIN = 20;
    static const int MAX = 980;
    static const int ZERO = 500;

    static void Draw();

    static pString ToString(uint16 rShiftRel, Range range ,Divider divider, char buffer[20]);

private:

    static void Draw(Chan::E ch);
};
