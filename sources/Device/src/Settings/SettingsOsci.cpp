#include "defines.h"
#include "Settings/Settings.h"


bool Chan::IsEnabled()
{
    return Ref().enabled;
}


bool Chan::IsInversed()
{
    return Ref().inverse;
}


SettingsChannel &Chan::Ref()
{
    return set.ch[ch];
}
