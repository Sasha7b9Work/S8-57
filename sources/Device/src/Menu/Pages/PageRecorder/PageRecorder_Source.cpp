#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"



DEF_CHOICE_2( cChanA,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК - Канал 1 ---
    "Канал 1",
    "Выбор канала 1 для записи и просмотра",
    DISABLE_RU,
    ENABLE_RU,
    set.rec.enA, &PageRecorder::PageSource::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanB,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧИНК - Канал 2 ---
    "Канал 2",
    "Выбор канала 2 для записи и просмотра",
    DISABLE_RU,
    ENABLE_RU,
    set.rec.enB, &PageRecorder::PageSource::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cSensor,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧИНК - Датчик ---
    "Датчик",
    "Выбор датчика для записи и просмотра",
    DISABLE_RU,
    ENABLE_RU,
    set.rec.enSensor, &PageRecorder::PageSource::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



static bool IsActive_Source()
{
    return !Recorder::IsRunning();
}

DEF_PAGE_3( pSource, // -V641 // -V1027                                                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК ---
    "ИСТОЧНИК",
    "Выбор записываемых сигналов",
    &cChanA,
    &cChanB,
    &cSensor,
    PageName::Recorder_Source, &PageRecorder::self, IsActive_Source, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageRecorder::PageSource::self = (const Page *)&pSource;
