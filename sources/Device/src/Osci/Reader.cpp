#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Settings/Settings.h"
#include <cstring>


// Это пространство CCM-памяти
static uint8 ccm[8][8 * 1024] __attribute__((section("CCM_DATA")));

// Это указатели на считанные данные
uint8 *dataIN[2] = { ccm[0], ccm[1] };

// Это указатели на данные, пересчитанные и готовые к выводу
uint8 *dataOUT[2] = { ccm[2], ccm[3] };

// 
uint16 *ave[2] = { IntRAM::Averager16k(ChanA), IntRAM::Averager16k(ChanB) };

DataSettings ds;

// Указатель на настройки считанных данных
const DataSettings *pDS = nullptr;


FrameP2P *frameP2P = nullptr;


void Reader::ReadDataFromRAM()
{
    DS = nullptr;

    DS = RAM::Get((set.mem.modeWork == ModeWork::RAM) ? static_cast<int>(RAM::currentSignal) : 0);

    if(Roller::NeedDraw())
    {
        DS = Roller::GetDS();
    }

    if (DS)
    {
        if (DS->enableA)
        {
            std::memcpy(IN_A, DS->dataA, 8 * 1024);
        }
        if (DS->enableB)
        {
            std::memcpy(IN_B, DS->dataB, 8 * 1024);
        }

        ds = *DS;
        DS = &ds;

        FindTrigLevelIfNeed();
    }
}


void Reader::ReadDataFromROM()
{
    DS = nullptr;

    DS = ROM::Data::Read(NUM_ROM_SIGNAL);

    if (DS)
    {
        if (DS->enableA)
        {
            std::memcpy(IN_A, DS->dataA, 8 * 1024);
        }
        if (DS->enableB)
        {
            std::memcpy(IN_B, DS->dataB, 8 * 1024);
        }
    }
}


void Reader::FindTrigLevelIfNeed()
{
    if (set.trig.modeFind == TrigModeFind::Auto)
    {
        TrigLevel::Find();
    }
}
