#include "defines.h"
#include "Settings/Settings.h"


bool Chan::IsEnabled() const
{
    return set.ch[value].enabled;
}


bool Chan::IsInversed() const
{
    return set.ch[value].inverse;
}
