#include "defines.h"
#include "Settings.h"
#include "Hardware/EEPROM.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"


using namespace FPGA::SET;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Settings set = Settings::defaultSettings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load(bool _default)
{
    if(_default || !EEPROM::LoadSettings())
    {
        Reset();
        Osci::Settings::Load();
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
    EEPROM::SaveSettings();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Reset()
{
    set = defaultSettings;
}
