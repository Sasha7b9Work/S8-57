#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory/ROM.h"
#include "FPGA/FPGA.h"
#include "Osci/Osci.h"
#include <cstring>



Settings set = Settings::defaultSettings;



void Settings::Reset()
{
    Settings old = set;

    set = defaultSettings;

    set.dbg.enum_gate_max = old.dbg.enum_gate_max;
    set.dbg.enum_gate_min = old.dbg.enum_gate_min;

    std::memcpy(set.dbg.addRShift, old.dbg.addRShift, sizeof(int8) * 2 * Range::Count);
}
