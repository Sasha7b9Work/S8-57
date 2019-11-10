#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/MemoryROM.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"


extern void MemorySave();
extern void MemoryLoad();


void Settings::Load(bool _default)
{
    set = Settings::defaultSettings;

    MemoryLoad();

    if(_default || !FlashMemory::Settings::Load())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        ContextOsci::OnPressStart();
    }
}


void Settings::Save()
{
    FlashMemory::Settings::Save();

    MemorySave();
}
