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

    set.dbg.nrst = old.dbg.nrst;
}
