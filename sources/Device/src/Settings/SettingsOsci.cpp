#include "defines.h"
#include "Settings/Settings.h"


bool Chan::IsEnabled() const
{
    return set.ch[value].enabled;
}
