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
        uint8 data[1024];
        TransformFromLoad(data);
        Menu::Init();
        FPGA::OnPressStart();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    uint8 data[1024];
    set.TransformForSave(data);
    Memory::SaveSettings();
}
