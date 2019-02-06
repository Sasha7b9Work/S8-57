#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


extern const PageBase pageFind;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::PageFind::pointer = &pageFind;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *hintsSearch_Mode[] = { "Ручной", "Hand", "Автоматический",  "Auto" };

static const ChoiceBase cMode =                                                                                                                               //--- СИНХР - ПОИСК - Режим ---
{
    Control::Type::Choice, 2, false, Page::Name::NoPage, &pageFind, FuncActive,
    {
        "Режим", "Mode"
        ,
        "Выбор режима автоматического поиска синхронизации:\n"
        "1. \"Ручной\" - поиск производится по нажатию кнопки \"Найти\" или по удержанию в течение 0.5с кнопки СИНХР, если установлено "
        "\"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\".\n"
        "2. \"Автоматический\" - поиск производится автоматически."
        ,
        "Selecting the automatic search of synchronization:\n"
        "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the СИНХР button if it is established "
        "\"SERVICE\x99Mode long СИНХР\x99\x41utolevel\".\n"
        "2. \"Auto\" - the search is automatically."
    },
    (int8 *)&TRIG_MODE_FIND, // -V206
    hintsSearch_Mode, Choice::EmptyChange, Choice::EmptyDraw
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Search()
{
    return TRIG_MODE_FIND_HAND;
}

static void OnPress_Search()
{
    Osci::Settings::Trig::Level::Find();
}

DEF_BUTTON( bSearch,                                                                                                                                          //--- СИНХР - ПОИСК - Найти ---
    "Найти", "Search",
    "Производит поиск уровня синхронизации.",
    "Runs for search synchronization level.",
    pageFind, IsActive_Search, OnPress_Search, Choice::EmptyDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageFind, // -V641 // -V1027                                                                                                                              //--- СИНХР - ПОИСК ---
    "ПОИСК", "SEARCH",
    "Управление автоматическим поиском уровня синхронизации.",
    "Office of the automatic search the trigger level.",
    &cMode,                         ///< СИНХР - ПОИСК - Режим
    &bSearch,                       ///< СИНХР - ПОИСК - Найти
    Page::Name::Trig_Search, PageTrig::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
