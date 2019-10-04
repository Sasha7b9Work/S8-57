#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory.h"
#include "FPGA/ContextOsci.h"
#include "Osci/Osci.h"



void Settings::Load(bool _default)
{
    if(_default || !Memory::LoadSettings())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        ContextOsci::OnPressStart();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    Memory::SaveSettings();
}
