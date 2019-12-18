#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cMode,                                                                                                                                          //--- СИНХР - ПОИСК - Режим ---
    "Режм",
    "Выбор режима автоматического поиска синхронизации:\n"
    "1. \"Ручной\" - поиск производится по нажатию кнопки \"Найти\" или по удержанию в течение 0.5с кнопки СИНХР, если установлено "
    "\"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\".\n"
    "2. \"Автоматический\" - поиск производится автоматически.",
    "Ручной",
    "Автоматический",
    TrigModeFind::Ref(), &PageTrig::Find::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Search()
{
    TrigLevel().Find();
}

DEF_BUTTON( bSearch,                                                                                                                                          //--- СИНХР - ПОИСК - Найти ---
    "Найти",
    "Производит поиск уровня синхронизации.",
    &PageTrig::Find::self, Item::Active, OnPress_Search
)



DEF_PAGE_2( pFind,                                                                                                                                                    //--- СИНХР - ПОИСК ---
    "ПОИСК",
    "Управление автоматическим поиском уровня синхронизации.",
    &cMode,
    &bSearch,
    PageName::Trig_Search,
    &PageTrig::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::Find::self = static_cast<const Page *>(&pFind);
