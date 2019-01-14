#pragma once
#include "FPGA/FPGA_Settings.h"
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

    String ToString(FPGA::SET::TBase::E tBase) const;

private:

    int value;
};
