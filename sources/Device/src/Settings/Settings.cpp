#include "defines.h"
#include "Settings.h"
#include "Hardware/Memory/ROM.h"
#include "FPGA/FPGA.h"
#include "Osci/Osci.h"
#include <cstring>



Settings set = Settings::defaultSettings;


SettingsChannel *const setCh[Chan::Count] = { &set._ch[Chan::A], &set._ch[Chan::B] };
SettingsChannel *const setChA = &set._ch[Chan::A];
SettingsChannel *const setChB = &set._ch[Chan::B];
SettingsFreqMeter *const setFreq = &set._freq;



void Settings::Reset()
{
    Settings old = set;

    set = defaultSettings;

    set.dbg.nrst = old.dbg.nrst;
}
