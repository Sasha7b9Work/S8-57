#include "defines.h"
#include "FPGA/Calibrator.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


static void OnChanged_Calibrator(bool)
{
    FPGA::LoadCalibratorMode();
}

DEF_CHOICE_2( cCalibrator, // -V206                                                                                                                //--- СЕРВИС - КАЛИБРАТОР - Калибратор ---
    "Калибратор",
    "Режим работы калибратора",
    "Перем",
    "Пост",
    set.serv.calibratorMode, &PageService::PageCalibrate::self, Item::Active, OnChanged_Calibrator, Choice::AfterDraw
)


static bool IsActive_Calibrate()
{
    return true;
}

static void OnPress_Calibrate()
{
    Calibrator::Calibrate();

    Display::FuncOnWait::Start("Калибровка канала 1", false);
    Timer::PauseOnTime(5000);
    Display::FuncOnWait::Stop();
    Display::FuncOnWait::Start("Калибровка канала 2", false);
    Timer::PauseOnTime(5000);
    Display::FuncOnWait::Stop();
}

DEF_BUTTON( bCalibrate,                                                                                                                           //--- СЕРВИС - КАЛИБРАТОР - Калибровать ---
    "Калибровать",
    "Запуск процедуры калибровки",
    &PageService::PageCalibrate::self, IsActive_Calibrate, OnPress_Calibrate
)


DEF_PAGE_2( pCalibrate,                                                                                                                                         //--- СЕРВИС - КАЛИБРАТОР ---
    "КАЛИБРОВКА",
    "Управлением калибратором и калибровка осциллографа",
    &cCalibrator,
    &bCalibrate,
    PageName::Service_Calibrator, &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageService::PageCalibrate::self = static_cast<const Page *>(&pCalibrate);
