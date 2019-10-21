#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Display/Font/Font.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"


static void OnPress_Previous()
{
}

static void Draw_Previous(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 4);
}

static bool IsActive_Previous()
{
    return true;
}

DEF_GRAPH_BUTTON( bPrevious,                                                                                                          //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Предыдущий ---
    "Предыдущий",
    "Перейти к предыдущей записи",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Previous, OnPress_Previous, Draw_Previous
)


static void OnPress_Next()
{

}

static void Draw_Next(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 4);
}

static bool IsActive_Next()
{
    return true;
}

DEF_GRAPH_BUTTON( bNext,                                                                                                               //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Следующий ---
    "Следующий",
    "Перейти к следующей записи",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Next, OnPress_Next, Draw_Next
)


static void OnPress_Confirm()
{

}

static void Draw_Confirm(int x, int y)
{
    Char(SymbolUGO2::SELECT).Draw4SymbolsInRect(x + 2, y + 4);
}

static bool IsActive_Confirm()
{
    return true;
}

DEF_GRAPH_BUTTON( bConfirm,
    "Подтвердить",
    "Выбрать текущий сигнал",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Confirm, OnPress_Confirm, Draw_Confirm
)


DEF_PAGE_3( pChoice,
    "ВЫБОР",
    "Выбор сигнала для показа",
    &bPrevious,
    &bNext,
    &bConfirm,
    PageName::Recorder_Show_Choice,
    &PageRecorder::PageShow::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


const Page *const PageRecorder::PageShow::PageChoice::self = static_cast<const Page *>(&pChoice);
