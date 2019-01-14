#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA_HAL.h"
#include "Hardware/FSMC.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTime.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::HAL::LoadRegUPR()
{
    uint8 data = 0;

    if (SET_PEAKDET_EN)
    {
        data |= 1 << BIT_UPR_PEAK;
    }

    static const uint8 mask[3] =
    {
        (1 << BIT_UPR_CALIBR_AC_DC),
        (1 << BIT_UPR_CALIBR_ZERO),
        (0)
    };

    data |= mask[CALIBRATOR_MODE];

    FSMC::WriteToFPGA8(WR_UPR, data);
}
