#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include "Display/Font/Font.h"
#include "Display/Symbols.h"
#include "Display/Display_Primitives.h"


using namespace Display::Primitives;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pRTC, // -V641 // -V1027                                                                                                                                 //--- СЕРВИС - ВРЕМЯ ---
    "ВРЕМЯ",
    "Установка и настройка времени",
    PageRTC::PageSet::self,         ///< СЕРВИС - ВРЕМЯ - Время
    PageRTC::PageCorrect::self,     ///< CЕРВИС - ВРЕМЯ - Коррекция
    PageName::Service_RTC,
    &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::self = (const Page *)&pRTC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_SetLeft()
{
}

static void Draw_Left(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetLeft, Draw_Left
)

static void OnPress_SetRight()
{
}

static void Draw_Right(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetRight, Draw_Right
)

static void OnPress_SetUp()
{
}

static void Draw_Up(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 4);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Up,
    "Больше",
    "Увеличить",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetUp, Draw_Up
)

static void OnPress_SetDown()
{
}

static void Draw_Down(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 4);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetDown, Draw_Down
)

static void OnPress_SetPick()
{
}

static void Draw_Pick(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SELECT).Draw4SymbolsInRect(x + 2 , y);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Pick,
    "Выбрать",
    "Выбор подсвеченного элемента",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetPick, Draw_Pick
)

DEF_PAGE_5( pSet, //-V641 //-V1027
    "УСТАНОВКА",
    "Установка текущего времени",
    &bSet_Left,
    &bSet_Right,
    &bSet_Up,
    &bSet_Down,
    &bSet_Pick,
    PageName::Service_RTC_Set,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageSet::self = (const Page *)&pSet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_CorrectionLeft()
{
}

DEF_GRAPH_BUTTON( bCorrection_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionLeft, Draw_Left
)

static void OnPress_CorrectionRight()
{
}

DEF_GRAPH_BUTTON( bCorrection_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionRight, Draw_Right
)

static void OnPress_CorrectionUp()
{
}

DEF_GRAPH_BUTTON( bCorrection_Up,
    "Больше",
    "Увеличить",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionUp, Draw_Up
)

static void OnPress_CorrectionDown()
{
}

DEF_GRAPH_BUTTON( bCorrection_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionDown, Draw_Down
)

static void OnPress_CorrectionPick()
{
}

DEF_GRAPH_BUTTON( bCorrection_Pick,
    "Выбор",
    "Активировать подсвеченный элемент",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionPick, Draw_Pick
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pCorrection, //-V641 //-V1027
    "КОРРЕКЦИЯ",
    "Коррекция скорости хода часов",
    &bCorrection_Left,
    &bCorrection_Right,
    &bCorrection_Up,
    &bCorrection_Down,
    &bCorrection_Pick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageCorrect::self = (const Page *)&pCorrection;
