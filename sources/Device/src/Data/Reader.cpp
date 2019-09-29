#include "defines.h"
#include "Data/Reader.h"
#include "Osci/Osci_Storage.h"
#include "Utils/CommonFunctions.h"
#include "Osci/Osci.h"

#include "Osci/Osci_Settings.h"
#include "Settings/Settings.h"
#include "Hardware/RAM.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Это пространство CCM-памяти
static uint8 ccm[4][16 * 1024] __attribute__((section("CCM_DATA")));
/// Это указатели на считанные данные
const uint8 *dataIN[2] = { nullptr, nullptr };
/// Это указатели на данные, пересчитанные и готовые к выводу
uint8 *dataOUT[2] = { ccm[0], ccm[1] };
/// 
uint16 *ave[2] = { reinterpret_cast<uint16 *>(ccm[2]), reinterpret_cast<uint16 *>(ccm[3]) };
/// Указатель на настройки считанных данных
const DataSettings *pDS = nullptr;
/// Указатель на считанные даныне
Osci::Data *pData = nullptr;
/// Указатель на фрейм поточечного вывода
Osci::DataP2P *pDataP2P = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Поиск уровня синхронизации, если установлен автоматический режим поиска
static void FindTrigLevelIfNeed();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Reader::ReadDataFromStorage()
{
    DATA_P2P = nullptr;
    DATA = nullptr;
    DS = nullptr;

    IN_A = IN_B = nullptr;

    DATA = Osci::Storage::GetData((set.mem.modeWork == ModeWork::RAM) ? Memory::RAM::CurrentSignal() : 0);

    if (DATA != nullptr)
    {
        DS = DATA->Settings();

        IN_A = DATA->DataA();
        IN_B = DATA->DataB();

        FindTrigLevelIfNeed();
    }

    if ((set.mem.modeWork == ModeWork::Dir) && Osci::InModeP2P())
    {
        ReadDataP2P();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Reader::ReadDataP2P()
{
    if (Osci::InModeP2P())
    {
        DATA_P2P = Osci::Storage::GetFrameP2P();

        if (DATA_P2P)
        {
            if (DS)
            {
                if ((set.trig.startMode == TrigStartMode::Wait) && DS->Equals(*DATA_P2P->Settings()))
                {
                    DATA_P2P = nullptr;
                }
                else
                {
                    DS = DATA_P2P->Settings();
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FindTrigLevelIfNeed()
{
    if (set.trig.modeFind == TrigModeFind::Auto)
    {
        Osci::Settings::Trig::Level::Find();
    }
}
