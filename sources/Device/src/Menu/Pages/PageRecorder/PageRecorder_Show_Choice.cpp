#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "common/Display/Font/Font.h"
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
    &PageRecorder::Show::Choice::self, IsActive_Previous, OnPress_Previous, Draw_Previous
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
    &PageRecorder::Show::Choice::self, IsActive_Next, OnPress_Next, Draw_Next
)


static void OnPress_Confirm()
{

}

static void Draw_Confirm(int x, int y)
{
    Char(SymbolUGO2::SELECT).Draw4SymbolsInRect(x + 4, y);
}

static bool IsActive_Confirm()
{
    return true;
}

DEF_GRAPH_BUTTON( bConfirm,                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ВЫБОР - Подтвердить ---
    "Подтвердить",
    "Выбрать текущий сигнал",
    &PageRecorder::Show::Choice::self, IsActive_Confirm, OnPress_Confirm, Draw_Confirm
)


static void OnPress_Leave()
{
}

static void Draw_Leave(int x, int y)
{
    Char(SymbolUGO2::LEAVE).Draw4SymbolsInRect(x + 4, y);
}

DEF_GRAPH_BUTTON( bLeave,                                                                                                        //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ВЫБОР - Возврат ---
    "Возврат",
    "Возврат на предыдущую страницу меню",
    &PageRecorder::Show::Choice::self, Button::Active, OnPress_Leave, Draw_Leave
)


static void OnPress_Delete()
{
}

static void Draw_Delete(int x, int y)
{
    Char(SymbolUGO2::DELETE).Draw4SymbolsInRect(x + 4, y);
}

DEF_GRAPH_BUTTON( bDelete,                                                                                                       //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ВЫБОР - Удалить ---
    "Удалить",
    "Удаление текущей записи",
    &PageRecorder::Show::Choice::self, Button::Active, OnPress_Delete, Draw_Delete
)


DEF_PAGE_5( pChoice,
    "ВЫБОР",
    "Выбор сигнала для показа",
    &bPrevious,
    &bNext,
    &bConfirm,
    &bLeave,
    &bDelete,
    PageName::Recorder_Show_Choice,
    &PageRecorder::Show::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


const Page *const PageRecorder::Show::Choice::self = static_cast<const Page *>(&pChoice);
