#include "defines.h"
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
    if(Menu::OpenedPage()->IsSubPage(PageService::self))
    {
        while(Menu::OpenedPage() != PageService::self)
        {
            Menu::CloseOpenedItem();
        }
    }

    ROM::Settings::Save();
}
