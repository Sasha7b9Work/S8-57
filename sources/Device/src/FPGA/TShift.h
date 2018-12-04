#pragma once
#include "Settings/SettingsTime.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TShift
{
public:
    static int Min();
    static int Zero();
    static int Max();

    TShift(int tshift);

    int InPoints();

    void Set(int tShift);

    operator int() const { return value; };

    String ToString(TBase::E tBase) const;

private:

    char *ToString(TBase::E tBase, char buffer[20]) const;

    int value;
};
