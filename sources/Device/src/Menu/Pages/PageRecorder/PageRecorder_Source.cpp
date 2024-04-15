// 2023/11/22 13:42:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"


DEF_CHOICE_2( cChanA,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК - Канал 1 ---
    "Канал 1",
    "Выбор канала 1 для записи и просмотра",
    DISABLE_RU,
    ENABLE_RU,
    S_REC_ENABLED_A, &PageRecorder::Source::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanB,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧИНК - Канал 2 ---
    "Канал 2",
    "Выбор канала 2 для записи и просмотра",
    DISABLE_RU,
    ENABLE_RU,
    S_REC_ENABLED_B, &PageRecorder::Source::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



static bool IsActive_Source()
{
    return !Recorder::InRecordingMode();
}

DEF_PAGE_2( pSource,                                                                                                                               //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК --- //-V2567
    "ИСТОЧНИК",
    "Выбор записываемых сигналов",
    &cChanA,
    &cChanB,
    PageName::Recorder_Source, &PageRecorder::self, IsActive_Source, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageRecorder::Source::self = static_cast<const Page *>(&pSource);
