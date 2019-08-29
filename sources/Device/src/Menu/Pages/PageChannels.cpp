#include "defines.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/MenuItems.h"
#include "Keyboard/Keyboard.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"
#include "Hardware/Timer.h"
#include "Osci/Osci.h"
#include "Settings/SettingsChannel.h"
#include <cstring>


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
    SET_ENABLED_A, &PageChannelA::self, E_BtV, PageChannelA::OnChanged_Input, E_VII
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
    SET_COUPLE_A, &PageChannelA::self, E_BtV, PageChannelA::OnChanged_Couple, E_VII
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
    SET_BANDWIDTH_A, &PageChannelA::self, E_BtV, OnChanged_ChanA_Bandwidth, E_VII
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
    &PageChannelA::self, E_BtV, OnPress_BalanceA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerA,                                                                                                                                         //--- КАНАЛ 1 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    divider[0], &PageChannelA::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseA,                                                                                                                                         //--- КАНАЛ 1 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    SET_INVERSE_A, &PageChannelA::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_6( pChanA, // -V641 // -V1027                                                                                                                                      //--- КАНАЛ 1 ---
    "КАНАЛ 1",
    "Содержит настройки канала 1.",
    &cInputA,           ///< КАНАЛ 1 - Вход
    &cCoupleA,          ///< КАНАЛ 1 - Связь
    &cBandwidthA,       ///< КАНАЛ 1 - Полоса
    &cDividerA,         ///< КАНАЛ 1 - Делитель
    &bBalanceA,         ///< КАНАЛ 1 - Балансировка
    &cInverseA,         ///< КАНАЛ 1 - Инверсия
    PageName::ChannelA, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageChannelA::self = (const Page *)&pChanA;

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
    SET_ENABLED_B, &PageChannelB::self, E_BtV, PageChannelB::OnChanged_Input, E_VII
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
    SET_COUPLE_B, &PageChannelB::self, E_BtV, PageChannelB::OnChanged_Couple, E_VII
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
    SET_BANDWIDTH_B, &PageChannelB::self, E_BtV, OnChanged_BandwidthB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- КАНАЛ 2 - Балансировка ---
    "Балансировка",
    "",
    &PageChannelB::self, E_BtV, OnPress_BalanceB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerB,                                                                                                                                         //--- КАНАЛ 2 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    divider[1], &PageChannelB::self, E_BtV, E_VB, E_VII
    )

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseB,                                                                                                                                         //--- КАНАЛ 2 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    SET_INVERSE_B, &PageChannelB::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_6( pChanB, // -V641 // -V1027                                                                                                                                      //--- КАНАЛ 2 ---
    "КАНАЛ 2",
    "Содержит настройки канала 2.",
    &cInputB,           ///< КАНАЛ 2 - Вход
    &cCoupleB,          ///< КАНАЛ 2 - Связь
    &cBandwidthB,       ///< КАНАЛ 2 - Полоса
    &cDividerB,         ///< КАНЛА 2 - Делитель
    &bBalanceB,         ///< КАНАЛ 2 - Балансировка
    &cInverseB,         ///< КАНАЛ 2 - Инверсия
    PageName::ChannelB, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageChannelB::self = (const Page *)&pChanB;
