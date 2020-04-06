#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"



Settings set = Settings::defaultSettings;


void Settings::Reset()
{
    set = defaultSettings;
}


uint Settings::CalcWriteCRC32()
{
    uint8 *buffer = reinterpret_cast<uint8 *>(this);

    return HAL_CRC32::Calculate8bit(buffer + 2 * sizeof(uint), sizeof(*this) - 2 * sizeof(uint));
}


uint8 SettingsService::SoundVolume()
{
    static const uint8 volume[4] = { 0, 25, 55, 100 };

    return volume[set.serv.enumVolume];
}
