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


extern const Page pChanA;
extern const Page pChanB;

const Page * const PageChannelA::self = (const Page *)&pChanA;
const Page * const PageChannelB::self = (const Page *)&pChanB;

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
    SET_ENABLED_A, &PageChannelA::self, 0, PageChannelA::OnChanged_Input, 0
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
    SET_COUPLE_A, &PageChannelA::self, 0, PageChannelA::OnChanged_Couple, 0
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
    SET_BANDWIDTH_A, &PageChannelA::self, 0, OnChanged_ChanA_Bandwidth, 0
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
    &PageChannelA::self, 0, OnPress_BalanceA, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerA,                                                                                                                                         //--- КАНАЛ 1 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    divider[0], &PageChannelA::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseA,                                                                                                                                         //--- КАНАЛ 1 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    SET_INVERSE_A, &PageChannelA::self, 0, 0, 0
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
    Page::Name::ChannelA, nullptr, 0, 0, 0, 0
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
    SET_ENABLED_B, &PageChannelB::self, 0, PageChannelB::OnChanged_Input, 0
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
    SET_COUPLE_B, &PageChannelB::self, 0, PageChannelB::OnChanged_Couple, 0
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
    SET_BANDWIDTH_B, &PageChannelB::self, 0, OnChanged_BandwidthB, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- КАНАЛ 2 - Балансировка ---
    "Балансировка",
    "",
    &PageChannelB::self, 0, OnPress_BalanceB, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cDividerB,                                                                                                                                         //--- КАНАЛ 2 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    divider[1], &PageChannelB::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cInverseB,                                                                                                                                         //--- КАНАЛ 2 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    SET_INVERSE_B, &PageChannelB::self, 0, 0, 0
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
    Page::Name::ChannelB, nullptr, 0, 0, 0, 0
)
