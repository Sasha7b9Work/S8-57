#include "defines.h"
#include "FPGA/Calibrator.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
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
    setA.enabled, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageChannelA::OnChanged_Couple(bool)
{
    Range::LoadBoth();
}

DEF_CHOICE_3( cCoupleA,                                                                                                                                             //--- КАНАЛ 1 - Связь ---
    "Связь",
    chanCouple,
    "Пост",
    "Перем",
    "Земля",
    ModeCouple::Ref(ChanA), &PageChannelA::self, Item::Active, PageChannelA::OnChanged_Couple, Choice::AfterDraw
)


static void OnChanged_BandwidthA(bool)
{
    Bandwidth::Load(ChanA);
}

DEF_CHOICE_2( cBandwidthA,                                                                                                                                         //--- КАНАЛ 1 - Полоса ---
    "Полоса",
    "Задаёт полосу пропускания канала",
    "Полная",
    "20МГц",
    setA.bandwidth, &PageChannelA::self, Item::Active, OnChanged_BandwidthA, Choice::AfterDraw
)


static void Balance(Chan::E ch)
{
    Calibrator::BalanceChannel(ch);
}


static void OnPress_BalanceA()
{
    Balance(ChanA);
}

DEF_BUTTON( bBalanceA,                                                                                                                                      //--- КАНАЛ 1 - Балансировать ---
    "Баланс",
    "",
    &PageChannelA::self, Item::Active, OnPress_BalanceA
)


DEF_CHOICE_2( cDividerA,                                                                                                                                         //--- КАНАЛ 1 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    S_DIVIDER_A, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cInverseA,                                                                                                                                         //--- КАНАЛ 1 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    setA.inverse, &PageChannelA::self, Item::Active, Choice::Changed, Choice::AfterDraw
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
    setB.enabled, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageChannelB::OnChanged_Couple(bool)
{
    Range::LoadBoth();
}

DEF_CHOICE_3( cCoupleB,                                                                                                                                             //--- КАНАЛ 2 - Связь ---
    "Связь",
    chanCouple,
    "Пост",
    "Перем",
    "Земля",
    ModeCouple::Ref(ChanB), &PageChannelB::self, Item::Active, PageChannelB::OnChanged_Couple, Choice::AfterDraw
)


static void OnChanged_BandwidthB(bool)
{
    Bandwidth::Load(ChanB);
}

DEF_CHOICE_2( cBandwidthB,                                                                                                                                         //--- КАНАЛ 2 - Полоса ---
    "Полоса",
    "",
    "Полная",
    "20МГц",
    setB.bandwidth, &PageChannelB::self, Item::Active, OnChanged_BandwidthB, Choice::AfterDraw
)


static void OnPress_BalanceB()
{
    Balance(ChanB);
}

DEF_BUTTON( bBalanceB,                                                                                                                                       //--- КАНАЛ 2 - Балансировка ---
    "Баланс",
    "",
    &PageChannelB::self, Item::Active, OnPress_BalanceB
)


DEF_CHOICE_2( cDividerB,                                                                                                                                         //--- КАНАЛ 2 - Делитель ---
    "Делитель",
    "",
    "1X",
    "10X",
    S_DIVIDER_B, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cInverseB,                                                                                                                                         //--- КАНАЛ 2 - Инверсия ---
    "Инверсия",
    "Инвертирует сигнал относительно уровня 0В",
    "Откл",
    "Вкл",
    setB.inverse, &PageChannelB::self, Item::Active, Choice::Changed, Choice::AfterDraw
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
