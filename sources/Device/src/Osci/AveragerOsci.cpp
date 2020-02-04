#include "defines.h"
#include "Osci.h"
#include "Hardware/Memory/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include <cstring>


/// «десь хранитс€ количество сигналов дл€ каждого из каналов, уже поучавствовавших в усреднении
static uint16 numSignals[2] = { 0, 0 };


void AveragerOsci::Process(Chan::E ch, const uint8 *dataNew, uint size)
{
    /*
        ¬ режиме рандомизатора в усреднении надо использовать только те данные, которы считаны. Ќельз€ брать данные дл€ усреднени€ из предыдущего сохранЄнного сигнала.
        ƒл€ этого нужно завести битовый массив, в котором отмечать те точки, которые считаны в данной итерации.
    */

    uint16 numAve = static_cast<uint16>(ENumAverage().Number());

    uint index = 0;
    uint step = 1;

    if (Osci::InModeRandomizer())
    {
        Osci::StructReadRand str = Osci::GetInfoForReadRand();

        index = str.posFirst;
        step = str.step;
    }

    uint8 *_new = const_cast<uint8 *>(dataNew) + index;
    uint16 *av = AVE_DATA(ch);

    /// \todo «десь неправильно - места в AVE_DATA слишком мало дл€ 8к * 16биты

    if (numSignals[ch] < ENumAverage())
    {
        if (numSignals[ch] == 0)
        {
            std::memset(AVE_DATA(ch), 0, static_cast<uint>(size) * 2);

            for (uint i = 0; i < size; i++)
            {
                av[i] = dataNew[i];
            }
        }
        else
        {
            for (uint i = index; i < size; i += step)
            {
                av[i] += *_new;

                _new += step;
            }
        }
    }
    else
    {
        for (uint i = index; i < size; i += step)
        {
            av[i] = static_cast<uint16>(av[i] - (av[i] >> numAve) + *_new);

            *_new = static_cast<uint8>(av[i] >> numAve);

            _new += step;
        }
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
    if (ENumAverage() > 1 && numSignals[0] < ENumAverage())
    {
        int height = 10;

        Rectangle(Grid::Width(), height).Draw(Grid::Left(), Grid::Top(), Color::GRID);

        Region(static_cast<int>(Grid::Width() * static_cast<float>(numSignals[0]) / ENumAverage()), height).Fill(Grid::Left(), Grid::Top());
    }
}
