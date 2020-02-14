#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/Osci.h"
#include "Utils/Debug.h"


void Settings::Load(bool _default)
{
    if(_default || !ROM::Settings::Load())
    {
        Reset();
        Osci::Init();
        Menu::Init();
    }
}


void Settings::Save()
{
    ROM::Settings::Save();
}
