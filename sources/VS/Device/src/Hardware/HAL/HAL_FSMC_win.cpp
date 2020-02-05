#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Hardware/HAL/HAL.h"
#include "Recorder/Recorder_win.h"
#include "Utils/Math.h"


uint8 *HAL_BUS::addrData0 = nullptr;
uint8 *HAL_BUS::addrData1 = nullptr;


void HAL_BUS::WriteToFPGA8(uint8 *, uint8)
{
}


void HAL_BUS::WriteToFPGA16(uint8 *, uint16)
{
}


uint8 HAL_BUS::ReadFromFPGA(const uint8 *address)
{
    if (address == RD::DATA_A || (address == RD::DATA_A + 1) ||
        address == RD::DATA_B || (address == RD::DATA_B + 1))
    {
        return RecorderHAL::ReadData((address == RD::DATA_A) || (address == RD::DATA_A + 1) ? Chan::A : Chan::B);
    }

    return 0;
}


void HAL_BUS::SetAddrData(uint8 *address0, uint8 *address1)
{
    addrData0 = address0;
    addrData1 = address1;
}


uint8 HAL_BUS::ReadData0()
{
    static uint prevTime = TIME_MS;

    Chan::E ch = (addrData0 == RD::DATA_A || addrData0 == RD::DATA_A_PEAK_MIN) ? Chan::A : Chan::B;

    double freq = TuneGeneratorDialog::frequency[ch];

    double amplitude = 100.0;

    double value = VALUE::AVE + amplitude * (sin(2 * Math::PI * (TIME_MS - prevTime) * freq));

    return static_cast<uint8>(value);
}


uint8 HAL_BUS::ReadData1()
{
    return VALUE::AVE;
}
