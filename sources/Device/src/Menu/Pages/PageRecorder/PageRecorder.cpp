#include "defines.h"
#include "Device.h"
#include "FlashDrive/FlashDrive.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Recorder/Sensor.h"


static bool IsActive_Destination()
{
    return !Recorder::IsRunning();
}

DEF_CHOICE_3( cTypeMemory,                                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - Память ---
    "Память",
    "Куда сохранять данные",
    "ОЗУ",
    "Внутр ЗУ",
    "Внешн ЗУ",
    S_REC_TYPE_MEMORY, &PageRecorder::self, IsActive_Destination, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Start()
{
    return (S_REC_ENABLED_A || S_REC_ENABLED_B || S_REC_ENABLED_SENSOR);
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

DEF_GRAPH_BUTTON_HINTS_2( bStart,                                                                                                                 //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПУСК/СТОП ---
    "Пуск",
    "Запуск/останов процесса регистрации",
    &PageRecorder::self, IsActive_Start, Recorder::OnPressStart, Draw_StartStop,
    Draw_Start, "Запуск",
    Draw_Stop, "Останов"
)


static void OnPress_Exit()
{
    Menu::CloseOpenedItem();
}


DEF_BUTTON(bExit,
    "Выход",
    "",
    &PageRecorder::self, Item::Active, OnPress_Exit
)


static void OnOpenClose_Recorder(bool open)
{
    Device::SetMode(open ? Device::Mode::Recorder : Device::Mode::Osci);

    if(open)
    {
//        FDrive::DeInit();
//        Sensor::Init();
    }
    else
    {
//        Sensor::DeInit();
//        FDrive::Init();
    }
}

DEF_PAGE_5( pRecorder,                                                                                                                                        //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР",
    "Запись и воспроизведение сигналов входов и датчиков",
    PageRecorder::Source::self,
    PageRecorder::Show::self,
    &Item::empty,
    &bStart,
    &bExit,
    PageName::Recorder, &PageFunction::self, Item::Active, Page::NormalTitle, OnOpenClose_Recorder, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageRecorder::self = static_cast<const Page *>(&pRecorder);
