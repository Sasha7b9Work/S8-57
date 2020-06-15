#include "defines.h"
#include "log.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"


bool Osci::ReadDataChannel(Chan::E ch, uint8 *data)
{
    int numPoints = ENumPointsFPGA::PointsInChannel();

    if(addrRead == 0xffff)
    {
        static const int shift[TBase::Count] =
        {
            10,  // 2 ns
            10,  // 5 ns
            10,  // 10 ns
            10,  // 20 ns
            10,  // 50 ns
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

        addrRead = static_cast<uint16>(ReadLastRecord(ch) - static_cast<int>(numPoints) / TBase::DeltaPoint() - shift[S_TIME_BASE]);
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, static_cast<uint16>(addrRead));
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = (ch == ChanA) ? RD::DATA_A : RD::DATA_B;  // -V566
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        return ReadDataChannelRand(a1, data);
    }
    else
    {
        uint8 *p = data;

        *p = HAL_BUS::FPGA::ReadA0();    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = HAL_BUS::FPGA::ReadA1();    // -V519

        if(PeakDetMode().IsEnabled())
        {
            for(int i = 0; i < numPoints; i++)
            {
                *p++ = HAL_BUS::FPGA::ReadA0();
                *p++ = HAL_BUS::FPGA::ReadA1();
            }
        }
        else
        {
            float stretch = HAL_BUS::FPGA::GetStretch(a1);

            for(int i = 0; i < numPoints; i++)
            {
                int delta = VALUE::AVE - static_cast<int>(*a1);

                int result = static_cast<int>(VALUE::AVE - static_cast<int>(delta * stretch));

                if(result < VALUE::MIN)
                {
                    p[i] = VALUE::MIN;
                }
                else if(result > VALUE::MAX)
                {
                    p[i] = VALUE::MAX;
                }
                else
                {
                    p[i] = static_cast<uint8>(result);
                }
            }
        }
    }

    return true;
}
