#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
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
#include "Utils/Dictionary.h"
#endif


extern const PageBase pChanA;
extern const PageBase pChanB;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char chanInputRu[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                    "2. \"Откл\" - не выводить сигнал на экран.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "Задаёт вид связи с источником сигнала.\n"
                                    "1. \"Пост\" - открытый вход.\n"
                                    "2. \"Перем\" - закрытый вход.\n"
                                    "3. \"Земля\" - вход соединён с землёй.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Input(bool)
{

}

DEF_CHOICE_2(       cChanA_Input,                                                                                             //--- КАНАЛ 1 - Вход ---
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SET_ENABLED_A, pChanA, FuncActive, PageChannelA::OnChanged_Input, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelA::OnChanged_Couple(bool)
{
    FPGA::Settings::SetModeCouple(Chan::A, SET_COUPLE_A);
}

DEF_CHOICE_3(       cChanA_Couple,                                                                                           //--- КАНАЛ 1 - Связь ---
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground",
    SET_COUPLE_A, pChanA, FuncActive, PageChannelA::OnChanged_Couple, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ChanA_Bandwidth(bool)
{
    FPGA::SetBandwidth(Chan::A);
}

DEF_CHOICE_2( cChanA_Bandwidth,                                                                                                                                    //--- КАНАЛ 1 - Полоса ---
    "Полоса", "Bandwidth",
    "Задаёт полосу пропускания канала",
    "Sets the channel bandwidth",
    "Полная", "Full",
    "20МГц",  "20MHz",
    SET_BANDWIDTH_A, pChanA, FuncActive, OnChanged_ChanA_Bandwidth, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageChannelA::pointer = &pChanA;

DEF_PAGE_3( pChanA, // -V641                                                                                                                                                //--- КАНАЛ 1 ---
    "КАНАЛ 1", "CHANNEL 1",
    "Содержит настройки канала 1.",
    "Contains settings of the channel 1.",
    &cChanA_Input,              ///< КАНАЛ 1 - Вход
    &cChanA_Couple,             ///< КАНАЛ 1 - Связь
    &cChanA_Bandwidth,          ///< КАНАЛ 1 - Полоса
    Page::Name::ChannelA, Menu::pageMain, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
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

DEF_CHOICE_2( cChanB_Input,                                                                                                                                          //--- КАНАЛ 2 - Вход ---
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_ENABLED_B, pChanB, FuncActive, PageChannelB::OnChanged_Input, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageChannelB::OnChanged_Couple(bool)
{
    FPGA::Settings::SetModeCouple(Chan::B, SET_COUPLE_B);
}

DEF_CHOICE_3( cChanB_Couple,                                                                                                                                        //--- КАНАЛ 2 - Связь ---
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "DC",
    "Перем", "AC",
    "Земля", "Ground",
    SET_COUPLE_B, pChanB, FuncActive, PageChannelB::OnChanged_Couple, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_BandwidthB(bool)
{
    FPGA::SetBandwidth(Chan::B);
}

DEF_CHOICE_2( cChanB_Bandwidth,                                                                                                                                    //--- КАНАЛ 2 - Полоса ---
    "Полоса", "Bandwidth",
    "",
    "",
    "Полная", "Full",
    "20МГц", "20MHz",
    SET_BANDWIDTH_B, pChanB, FuncActive, OnChanged_BandwidthB, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageChannelB::pointer = &pChanB;

DEF_PAGE_3( pChanB, // -V641                                                                                                                                                //--- КАНАЛ 2 ---
    "КАНАЛ 2", "CHANNEL 2",
    "Содержит настройки канала 2.",
    "Contains settings of the channel 2.",
    &cChanB_Input,       // КАНАЛ 2 - Вход
    &cChanB_Couple,      // КАНАЛ 2 - Связь
    &cChanB_Bandwidth,   // КАНАЛ 2 - Полоса
    Page::Name::ChannelB, Menu::pageMain, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
