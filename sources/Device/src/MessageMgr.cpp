#include "defines.h"
#include "MessageMgr.h"
#include "Osci/Osci.h"



void MessageMgr::OsciSettingsEffectOnAverageChanged()
{
    AveragerOsci::SettingChanged();
}
