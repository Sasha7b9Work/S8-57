#include "defines.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Keyboard/Keyboard.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"
#include "Hardware/Timer.h"
#include "Osci/Osci.h"
#include <cstring>


static const char chanInput[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                  "2. \"Откл\" - не выводить сигнал на экран.";

static const char chanCouple[] =  "Задаёт вид связи с источником сигнала.\n"
                                  "1. \"Пост\" - открытый вход.\n"
                                  "2. \"Перем\" - закрытый вход.\n"
                                  "3. \"Земля\" - вход соединён с землёй.";



DEF_CHOICE_2( cInputA,                                                                                                                                               //--- КАНАЛ 1 - Вход ---
    "Вход",
    chanInput,
    DISABLE_RU,
    ENABLE_RU,
    set.ch[Chan::A].enabled, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageChannelA::OnChanged_Couple(bool)
{
    ModeCouple::Set(Chan::A, ModeCouple(Chan::A));
}

DEF_CHOICE_3( cCoupleA,                                                                                                                                             //--- КАНАЛ 1 - Связь ---
    "Связь",
    chanCouple,
    "Пост",
    "Перем",
    "Земля",
    set.ch[Chan::A]._couple, &PageChannelA::self, Item::Active, PageChannelA::OnChanged_Couple, Choice::AfterDraw
)


static void OnChanged_BandwidthA(bool)
{
    set.ch[Chan::A].bandwidth.Load();
}

DEF_CHOICE_2( cBandwidthA,                                                                                                                                         //--- КАНАЛ 1 - Полоса ---
    "Полоса",
    "Задаёт полосу пропускания канала",
    "Полная",
    "20МГц",
    set.ch[Chan::A].bandwidth, &PageChannelA::self, Item::Active, OnChanged_BandwidthA, Choice::AfterDraw
)


static void Balance(Chan::E ch)
{
    Display::FuncOnWaitStart(ch == Chan::A ? "Балансировка канала 1" : "Балансировка канала 2", false);

    Settings old = set;

    Osci::Balance(ch);

    std::memcpy(&old.dbg.addRShift[0][0], &set.dbg.addRShift[0][0], sizeof(int8) * 2 * Range::Count);

    set = old;

    Osci::Init();

    Display::FuncOnWaitStop();
}


static void OnPress_BalanceA()
{
    Balance(Chan::A);
}

DEF_BUTTON( bBalanceA,                                                                                                                                      //--- КАНАЛ 1 - Балансировать ---
    "Балансировка",
    "",
    &PageChannelA::self, Item::Active, OnPress_BalanceA
)


DEF_CHOICE_2( cDividerA,                                                                                                                                         //--- КАНАЛ 1 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    set.ch[Chan::A].divider, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cInverseA,                                                                                                                                         //--- КАНАЛ 1 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    set.ch[Chan::A].inverse, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_6( pChanA,                                                                                                                                                         //--- КАНАЛ 1 ---
    "КАНАЛ 1",
    "Содержит настройки канала 1.",
    &cInputA,
    &cCoupleA,
    &cBandwidthA,
    &cDividerA,
    &bBalanceA,
    &cInverseA,
    PageName::ChannelA, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageChannelA::self = static_cast<const Page *>(&pChanA);


DEF_CHOICE_2( cInputB,                                                                                                                                               //--- КАНАЛ 2 - Вход ---
    "Вход",
    chanInput,
    DISABLE_RU,
    ENABLE_RU,
    set.ch[Chan::B].enabled, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageChannelB::OnChanged_Couple(bool)
{
    ModeCouple::Set(Chan::B, ModeCouple(Chan::B));
}

DEF_CHOICE_3( cCoupleB,                                                                                                                                             //--- КАНАЛ 2 - Связь ---
    "Связь",
    chanCouple,
    "Пост",
    "Перем",
    "Земля",
    set.ch[Chan::B]._couple, &PageChannelB::self, Item::Active, PageChannelB::OnChanged_Couple, Choice::AfterDraw
)


static void OnChanged_BandwidthB(bool)
{
    set.ch[Chan::B].bandwidth.Load();
}

DEF_CHOICE_2( cBandwidthB,                                                                                                                                         //--- КАНАЛ 2 - Полоса ---
    "Полоса",
    "",
    "Полная",
    "20МГц",
    set.ch[Chan::B].bandwidth, &PageChannelB::self, Item::Active, OnChanged_BandwidthB, Choice::AfterDraw
)


static void OnPress_BalanceB()
{
    Balance(Chan::B);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- КАНАЛ 2 - Балансировка ---
    "Балансировка",
    "",
    &PageChannelB::self, Item::Active, OnPress_BalanceB
)


DEF_CHOICE_2( cDividerB,                                                                                                                                         //--- КАНАЛ 2 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    set.ch[Chan::B].divider, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cInverseB,                                                                                                                                         //--- КАНАЛ 2 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    set.ch[Chan::B].inverse, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_6( pChanB,                                                                                                                                                         //--- КАНАЛ 2 ---
    "КАНАЛ 2",
    "Содержит настройки канала 2.",
    &cInputB,
    &cCoupleB,
    &cBandwidthB,
    &cDividerB,
    &bBalanceB,
    &cInverseB,
    PageName::ChannelB, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageChannelB::self = static_cast<const Page *>(&pChanB);
