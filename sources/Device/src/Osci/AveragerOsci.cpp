#include "defines.h"
#include "log.h"
#include "Osci.h"
#include "Hardware/Memory/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include <cstring>


/// Здесь хранится количество сигналов для каждого из каналов, уже поучавствовавших в усреднении
static uint16 numSignals[2] = { 0, 0 };


#define AVERAGE(shift)                                                  \
    for(uint i = 0; i < size; i++)                                      \
    {                                                                   \
        av[i] = static_cast<uint16>(av[i] - (av[i] >> shift) + *_new);  \
        *_new = static_cast<uint8>(av[i] >> shift);                     \
        _new++;                                                         \
    }


void AveragerOsci::Process(Chan::E ch, const uint8 *dataNew, uint size)
{
    uint8 *_new = const_cast<uint8 *>(dataNew);
    uint16 *av = AVE_DATA(ch);

    uint16 shift = static_cast<uint16>(ENumAverage());

    uint16 count = numSignals[ch];

    if (count < ENumAverage().Number())
    {
        if(ch == Chan::A)     { LOG_WRITE("точка 1");  }

        if (count == 0)
        {
            if(ch == Chan::A) { LOG_WRITE("точка 2");  }

            for (uint i = 0; i < size; i++)
            {
                av[i] = dataNew[i];
            }
        }
        else if(count == 1)
        {
            if(ch == Chan::A) { LOG_WRITE("точка 3"); }

            AVERAGE(1);
        }
        else if(count < 4)
        {
            AVERAGE(2);
        }
        else if(count < 8)
        {
            AVERAGE(3);
        }
        else if(count < 16)
        {
            AVERAGE(4);
        }
        else if(count < 32)
        {
            AVERAGE(5);
        }
        else if(count < 64)
        {
            AVERAGE(6);
        }
        else if(count < 128)
        {
            AVERAGE(7);
        }
        else if(count < 256)
        {
            AVERAGE(8);
        }
    }
    else
    {
        AVERAGE(shift);
    }

    if (numSignals[ch] < NUM_AVE_MAX + 10)
    {
        numSignals[ch]++;
    }
}


void AveragerOsci::SettingChanged()
{
    numSignals[0] = 0;
    numSignals[1] = 0;
}


void AveragerOsci::Draw()
{
    int numAverage = ENumAverage().Number();

    if (numAverage > 1 && numSignals[0] < numAverage)
    {
        int height = 10;

        Rectangle(Grid::Width(), height).Draw(Grid::Left(), Grid::Top(), Color::GRID);

        Region(static_cast<int>(Grid::Width() * static_cast<float>(numSignals[0]) / numAverage), height).Fill(Grid::Left(), Grid::Top());
    }
}
