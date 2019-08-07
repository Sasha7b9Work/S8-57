#include "defines.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/MenuItems.h"
#include "Keyboard/Keyboard.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Hardware/Timer.h"

#include "Osci/Osci.h"
#include "Settings/SettingsChannel.h"
#include <cstring>


extern const PageBase pChanA;
extern const PageBase pChanB;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char chanInput[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                  "2. \"Откл\" - не выводить сигнал на экран.";

static const char chanCouple[] =  "Задаёт вид связи с источником сигнала.\n"
                                  "1. \"Пост\" - открытый вход.\n"
                                  "2. \"Перем\" - закрытый вход.\n"
                                  "3. \"Земля\" - вход соединён с землёй.";

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Input(bool)
{

}

DEF_CHOICE_2( cInputA,                                                                                                                                               //--- КАНАЛ 1 - Вход ---
    "Вход",
    chanInput,
    DISABLE_RU,
    ENABLE_RU,
    SET_ENABLED_A, pChanA, 0, PageChannelA::OnChanged_Input, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Couple(bool)
{
    FPGA::Settings::ModeCouple::Set(Chan::A, SET_COUPLE_A);
}

DEF_CHOICE_3( cCoupleA,                                                                                                                                             //--- КАНАЛ 1 - Связь ---
    "Связь",
    chanCouple,
    "Пост",
    "Перем",
    "Земля",
    SET_COUPLE_A, pChanA, 0, PageChannelA::OnChanged_Couple, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Bandwidth(bool)
{
    SET_BANDWIDTH_A.Load();
}

DEF_CHOICE_2( cBandwidthA,                                                                                                                                         //--- КАНАЛ 1 - Полоса ---
    "Полоса",
    "Задаёт полосу пропускания канала",
    "Полная",
    "20МГц",
    SET_BANDWIDTH_A, pChanA, 0, OnChanged_ChanA_Bandwidth, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Balance(Chan::E ch)
{
    Display::FuncOnWaitStart(ch == Chan::A ? "Балансировка канала 1" : "Балансировка канала 2", false);

    Settings old = set;

    Osci::Balance(ch);

    std::memcpy(&old.addRShift[0][0], &set.addRShift[0][0], sizeof(int8) * 2 * Osci::Settings::Range::Size);

    set = old;

    Osci::Init();

    Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceA()
{
    Balance(Chan::A);
}

DEF_BUTTON( bBalanceA,                                                                                                                                      //--- КАНАЛ 1 - Балансировать ---
    "Балансировка",
    "",
    pChanA, 0, OnPress_BalanceA, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cDividerA,
    "Делитель",
    "",
    "1X",
    "10X",
    divider[0], pChanA, 0, 0, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageChannelA::pointer = &pChanA;

DEF_PAGE_5( pChanA, // -V641 // -V1027                                                                                                                                      //--- КАНАЛ 1 ---
    "КАНАЛ 1",
    "Содержит настройки канала 1.",
    &cInputA,           ///< КАНАЛ 1 - Вход
    &cCoupleA,          ///< КАНАЛ 1 - Связь
    &cBandwidthA,       ///< КАНАЛ 1 - Полоса
    &cDividerA,         ///< КАНАЛ 1 - Делитель
    &bBalanceA,         ///< КАНАЛ 1 - Балансировка
    Page::Name::ChannelA, nullptr, 0, 0, 0, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelB::OnChanged_Input(bool active)
{
    if (!active)
    {
        Display::ShowWarning(Warning::TooLongMemory);
        return;
    }
}

DEF_CHOICE_2( cInputB,                                                                                                                                               //--- КАНАЛ 2 - Вход ---
    "Вход",
    chanInput,
    DISABLE_RU,
    ENABLE_RU,
    SET_ENABLED_B, pChanB, 0, PageChannelB::OnChanged_Input, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelB::OnChanged_Couple(bool)
{
    FPGA::Settings::ModeCouple::Set(Chan::B, SET_COUPLE_B);
}

DEF_CHOICE_3( cCoupleB,                                                                                                                                             //--- КАНАЛ 2 - Связь ---
    "Связь",
    chanCouple,
    "Пост",
    "Перем",
    "Земля",
    SET_COUPLE_B, pChanB, 0, PageChannelB::OnChanged_Couple, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_BandwidthB(bool)
{
    SET_BANDWIDTH_B.Load();
}

DEF_CHOICE_2( cBandwidthB,                                                                                                                                         //--- КАНАЛ 2 - Полоса ---
    "Полоса",
    "",
    "Полная",
    "20МГц",
    SET_BANDWIDTH_B, pChanB, 0, OnChanged_BandwidthB, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- КАНАЛ 2 - Балансировка ---
    "Балансировка",
    "",
    pChanB, 0, OnPress_BalanceB, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerB,
    "Делитель",
    "",
    "1X",
    "10X",
    divider[1], pChanB, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageChannelB::pointer = &pChanB;

DEF_PAGE_5( pChanB, // -V641 // -V1027                                                                                                                                      //--- КАНАЛ 2 ---
    "КАНАЛ 2",
    "Содержит настройки канала 2.",
    &cInputB,           ///< КАНАЛ 2 - Вход
    &cCoupleB,          ///< КАНАЛ 2 - Связь
    &cBandwidthB,       ///< КАНАЛ 2 - Полоса
    &cDividerB,
    &bBalanceB,         ///< КАНАЛ 2 - Балансировка
    Page::Name::ChannelB, nullptr, 0, 0, 0, FuncRegSetPage
)
