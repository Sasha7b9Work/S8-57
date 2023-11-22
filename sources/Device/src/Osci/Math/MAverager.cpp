// 2023/11/22 13:47:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Math/OsciMath.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


// «десь хранитс€ количество сигналов дл€ каждого из каналов, уже поучавствовавших в усреднении
static uint16 numSignals[2] = { 0, 0 };


void AveragerOsci::Process(Chan::E ch, const uint8 *dataNew, int size)
{
    uint8 *_new = const_cast<uint8 *>(dataNew); //-V2567
    uint16 *av = AVE_DATA(ch);

    uint16 enumAverages = S_OSCI_ENUM_AVERAGE_U16;

    if((OSCI_IN_MODE_RANDOMIZER) && (NRST_ENUM_AVERAGE_RAND > enumAverages))
    {
        enumAverages = static_cast<uint16>(NRST_ENUM_AVERAGE_RAND);
    }

    uint16 numAverages = static_cast<uint16>(1 << enumAverages);

    if (numSignals[ch] < numAverages)
    {
        if (numSignals[ch] == 0)
        {
            for (int i = 0; i < size; i++)
            {
                av[i] = dataNew[i];
            }
        }
        else
        {
            for(int i = 0; i < size; i++)
            {
                if(dataNew[i] != VALUE::NONE)
                {
                    av[i] += dataNew[i];
                    *_new++ = static_cast<uint8>(av[i] / (numSignals[ch] + 1));
                }
            }
        }
    }
    else
    {
        uint16 shift = static_cast<uint16>(enumAverages);

        for(int i = 0; i < size; i++)
        {
            av[i] = static_cast<uint16>(av[i] - (av[i] >> shift) + *_new);
            *_new++ = static_cast<uint8>(av[i] >> shift);
        }
    }

    if(numSignals[ch] < NUM_AVE_MAX + 10)
    {
        numSignals[ch]++;
    }
}


void AveragerOsci::SettingChanged()
{
    numSignals[0] = 0;
    numSignals[1] = 0;
}
