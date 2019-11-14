#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Reader.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"


/// Это пространство CCM-памяти
static uint8 ccm[4][16 * 1024] __attribute__((section("CCM_DATA")));
/// Это указатели на считанные данные
const uint8 *dataIN[2] = { nullptr, nullptr };
/// Это указатели на данные, пересчитанные и готовые к выводу
uint8 *dataOUT[2] = { ccm[0], ccm[1] };
/// 
uint16 *ave[2] = { reinterpret_cast<uint16 *>(ccm[2]), reinterpret_cast<uint16 *>(ccm[3]) };
/// Указатель на настройки считанных данных
DataSettings *pDS = nullptr;

FrameP2P *frameP2P = nullptr;

/// Поиск уровня синхронизации, если установлен автоматический режим поиска
static void FindTrigLevelIfNeed();


void Reader::ReadDataFromStorage()
{
    DS = nullptr;

    IN_A = IN_B = nullptr;

    if (RAM::Read(&DS, (set.mem.modeWork == ModeWork::RAM) ? static_cast<uint>(RAM::currentSignal) : 0U))
    {
        IN_A = DS->dataA;
        IN_B = DS->dataB;

        FindTrigLevelIfNeed();
    }

    if ((set.mem.modeWork == ModeWork::Dir) && Osci::InModeP2P())
    {
        ReadDataP2P();
    }
}


void Reader::ReadDataP2P()
{
    if (Osci::InModeP2P())
    {
        FRAME_P2P = RAM::GetFrameP2P();
        if (FRAME_P2P)
        {
            if (DS)
            {
                if ((set.trig.startMode == TrigStartMode::Wait) && DS->Equals(*FRAME_P2P->ds))
                {
                    FRAME_P2P = nullptr;
                }
                else
                {
                    DS = FRAME_P2P->ds;
                }
            }
        }
    }
}


static void FindTrigLevelIfNeed()
{
    if (set.trig.modeFind == TrigModeFind::Auto)
    {
        TrigLevel::Find();
    }
}
