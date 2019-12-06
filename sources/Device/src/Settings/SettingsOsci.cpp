#include "defines.h"
#include "Settings/Settings.h"


bool Chan::IsEnabled()
{
    return Ref().enabled;
}


bool Chan::IsInversed() const
{
    return set.ch[value].inverse;
}


SettingsChannel &Chan::Ref()
{
    return set.ch[value];
}
