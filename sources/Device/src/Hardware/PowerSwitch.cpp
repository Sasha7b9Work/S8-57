#include "defines.h"
#include "device.h"
#include "Hardware/Battery.h"
#include "Hardware/PowerSwitch.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


void PowerSwitch::Init()
{
    HAL_PIO::Init(PIN_POWER, HMode::Output_PP, HPull::Up);
    HAL_PIO::Set(PIN_POWER);
}


void PowerSwitch::OffIfNeed()
{
    if(Battery::GetVoltageAKK() < 5.0F)
    {
        if(!Device::InModeTester())
        {
            Settings::Save();
        }

        setNRST.Save();

        HAL_PIO::Reset(PIN_POWER);
    }
}
