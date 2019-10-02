#include "defines.h"
#include "Device.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"


static bool IsActive_Destination()
{
    return !Recorder::IsRunning();
}

DEF_CHOICE_3( cDestination,                                                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - Сохранять в ---
    "Сохранять в",
    "Куда сохранять данные",
    "ОЗУ",
    "Внутр ЗУ",
    "Внешн ЗУ",
    set.rec.storageRecord, &PageRecorder::self, IsActive_Destination, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Start()
{
    return set.rec.enabledChanA || set.rec.enabledChanB || set.rec.enabledSensor;
}

static void Draw_Start(int x, int y)
{
    String("ПУСК").Draw(x, y + 5);
}

static void Draw_Stop(int x, int y)
{
    String("СТОП").Draw(x, y + 5);
}

static void Draw_StartStop(int x, int y)
{
    Recorder::IsRunning() ? Draw_Stop(x, y) : Draw_Start(x, y);
}

static void OnPress_Start()
{
    Recorder::OnPressStart();
}

DEF_GRAPH_BUTTON_HINTS_2( bStart,                                                                                                                 //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПУСК/СТОП ---
    "Пуск",
    "Запуск/останов процесса регистрации",
    &PageRecorder::self, IsActive_Start, OnPress_Start, Draw_StartStop,
    Draw_Start, "Запуск",
    Draw_Stop, "Останов"
)



static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}

DEF_PAGE_4( pRecorder, // -V641 // -V1027                                                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР",
    "Запись и воспроизведение сигналов входов и датчиков",
    PageRecorder::PageSource::self,
    &cDestination,
    PageRecorder::PageShow::self,
    &bStart,
    PageName::Recorder, &PageFunction::self, Item::Active, OnOpenClose_Recorder, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageRecorder::self = (const Page *)& pRecorder;
