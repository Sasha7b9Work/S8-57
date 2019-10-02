#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Storage.h"
#include "Settings/Settings.h"


DEF_CHOICE_3( cSource,
    "Источник",
    "Откуда брать данные для просмотра",
    "ОЗУ",
    "Внутр ЗУ",
    "Внешн ЗУ",
    set.rec.storagePlay, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Right()
{
}

static void Draw_Right(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bRight,                                                                                                                 //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Вправо ---
    "Вправо",
    "Переместить окно просмотра вправо",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Right, Draw_Right
)


static void OnPress_Left()
{
}

static void Draw_Left(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bLeft,                                                                                                                   //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Влево ---
    "Влево",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Left, Draw_Left
)


static bool IsActive_PageShow()
{
    return !Recorder::IsRunning();
}

static bool HandlerKey_PageShow(const KeyEvent &event)
{
    if (event.type == TypePress::Press || event.type == TypePress::Repeat)
    {
        if (event.key == Key::Left)
        {
            Recorder::Display::MoveCursorLeft();
            return true;
        }
        else if (event.key == Key::Right)
        {
            Recorder::Display::MoveCursorRight();
            return true;
        }
        else
        {
            // здесь ничего не надо
        }
    }

    return false;
}

DEF_PAGE_5( pShow, // -V641 // -V1027                                                                                                              //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР ---
    "ПРОСМОТР",
    "Просмотр записанных данных",
    &cSource,
    PageRecorder::PageShow::PageChoice::self,
    &bLeft,
    &bRight,
    PageRecorder::PageShow::PageCursors::self,
    PageName::Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::OpenClose, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::PageShow::self = (const Page *)&pShow;
