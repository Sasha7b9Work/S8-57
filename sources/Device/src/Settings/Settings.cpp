#include "defines.h"
#include "Settings.h"


Settings set = Settings::defaultSettings;


void Settings::Reset()
{
    set = defaultSettings;
}
