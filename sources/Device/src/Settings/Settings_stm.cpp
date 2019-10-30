#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory/Memory.h"
#include "FPGA/ContextOsci.h"
#include "Osci/Osci.h"



void Settings::Load(bool _default)
{
    if(_default || !FlashMemory::LoadSettings())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        ContextOsci::OnPressStart();
    }
}


void Settings::Save()
{
    FlashMemory::SaveSettings();
}
