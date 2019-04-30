#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Osci/Osci.h"

#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Settings set = Settings::defaultSettings;

Osci::Settings::TShift TimeShift(0);

int8 divider[2] = { 0, 0 };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Reset()
{
    Settings old = set;

    set = defaultSettings;

    set.dbg_enum_gate_max = old.dbg_enum_gate_max;
    set.dbg_enum_gate_min = old.dbg_enum_gate_min;

    std::memcpy(set.addRShift, old.addRShift, sizeof(int8) * 2 * Osci::Settings::Range::Size);
}
