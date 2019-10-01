#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"



DEF_CHOICE_3( cSource,
    "Источник",
    "Откуда брать данные для просмотра",
    "ОЗУ",
    "Внутр ЗУ",
    "Внешн ЗУ",
    set.rec.storagePlay, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_CHOICE_2( cCursor,
    "Курсор",
    "",
    "1",
    "2",
    set.rec.currentCursor, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Следующий ---
    "Влево",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Next
)


static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Предыдущий ---
    "Вправо",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Prev
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

DEF_PAGE_2( pShow, // -V641 // -V1027                                                                                                              //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР ---
    "ПРОСМОТР",
    "Просмотр записанных данных",
    &cSource,
    PageRecorder::PageShow::PageChoice::self,
    PageName::Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::OpenClose, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::PageShow::self = (const Page *)&pShow;
