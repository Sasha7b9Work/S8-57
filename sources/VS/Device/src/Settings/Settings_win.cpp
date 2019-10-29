#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/Memory/Memory.h"
#include "FPGA/FPGA.h"
#include "Osci/Osci.h"


extern void MemorySave();
extern void MemoryLoad();


void Settings::Load(bool)
{
    set = Settings::defaultSettings;

    MemoryLoad();
}


void Settings::Save()
{
    MemorySave();
}
