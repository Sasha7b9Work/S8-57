#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"


extern const Page pChoice;


static void OnPress_Up()
{
}

static void Draw_Up(int, int)
{

}

static bool IsActive_Up()
{
    return true;
}

DEF_GRAPH_BUTTON( bUp,
    "Предыдущий",
    "Перейти к предыдущей записи",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Up, OnPress_Up, Draw_Up
)


static void OnPress_Down()
{

}

static void Draw_Down(int, int)
{

}

static bool IsActive_Down()
{
    return true;
}

DEF_GRAPH_BUTTON( bDown,
    "Следующий",
    "Перейти к следующей записи",
    &PageRecorder::PageShow::PageChoice::self, IsActive_Down, OnPress_Down, Draw_Down
)


DEF_PAGE_2( pChoice,
    "ВЫБОР",
    "Выбор сигнала для показа",
    &bUp,
    &bDown,
    PageName::Recorder_Show_Choice,
    &PageRecorder::PageShow::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


const Page *const PageRecorder::PageShow::PageChoice::self = (const Page *)&pChoice;
