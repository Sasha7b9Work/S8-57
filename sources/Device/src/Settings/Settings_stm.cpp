#include "defines.h"
#include "log.h"
#include "Settings.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/Osci.h"


void Settings::Load(bool _default)
{
    if(_default || !ROM::Settings::Load())
    {
        set = Settings::defaultSettings;
        Osci::Init();
        Menu::Init();
    }
}


void Settings::Save()
{
    LOG_WRITE("сохран€ю настройки");
    ROM::Settings::Save();
}
