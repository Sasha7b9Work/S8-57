#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Osci/Osci.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load(bool _default)
{
    if(_default || !Memory::LoadSettings())
    {
        Reset();
        Osci::Init();
        Menu::Init();
        FPGA::OnPressStart();
    }

    SET_TSHIFT.Set(set.time_shift);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    set.time_shift = SET_TSHIFT;
    
    Memory::SaveSettings();
}
