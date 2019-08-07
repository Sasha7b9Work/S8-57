#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"

#include "Hardware/Timer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibrate;



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA::Settings::LoadCalibratorMode();
}

DEF_CHOICE_2( cCalibrator_Calibrator, // -V206                                                                                                     //--- СЕРВИС - КАЛИБРАТОР - Калибратор ---
    "Калибратор",
    "Режим работы калибратора",
    "Перем",
    "Пост",
    CALIBRATOR_MODE, pageCalibrate, Choice::FuncActive, OnChanged_Calibrator_Calibrator, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrator_Calibrate()
{
    //return !(SET_CALIBR_MODE_A == CalibrationMode::Disable && CALIBR_MODE_B == CalibrationMode::Disable);
    return true;
}

static void OnPress_Calibrator_Calibrate()
{
    Display::FuncOnWaitStart("Калибровка канала 1", false);

    Timer::PauseOnTime(5000);

    Display::FuncOnWaitStop();

    Display::FuncOnWaitStart("Калибровка канала 2", false);

    Timer::PauseOnTime(5000);

    Display::FuncOnWaitStop();
}

DEF_BUTTON( bCalibrator_Calibrate,                                                                                                                //--- СЕРВИС - КАЛИБРАТОР - Калибровать ---
    "Калибровать",
    "Запуск процедуры калибровки",
    pageCalibrate, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageCalibrate, // -V641 // -V1027                                                                                                                   //--- СЕРВИС - КАЛИБРАТОР ---
    "КАЛИБРОВКА",
    "Управлением калибратором и калибровка осциллографа",
    &cCalibrator_Calibrator,     /// СЕРВИС - КАЛИБРАТОР - Калибратор
    &bCalibrator_Calibrate,      /// СЕРВИС - КАЛИБРАТОР - Калибровать
    Page::Name::Service_Calibrator, PageService::pointer, Page::FuncActive, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)


const PageBase * const PageService::PageCalibrate::pointer = &pageCalibrate;

