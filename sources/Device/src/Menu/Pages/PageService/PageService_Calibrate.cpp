#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"


extern const Page pageCalibrate;

const Page * const PageService::PageCalibrate::self = (const Page *)&pageCalibrate;


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
    CALIBRATOR_MODE, &PageService::PageCalibrate::self, E_BtV, OnChanged_Calibrator_Calibrator, E_VII
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
    &PageService::PageCalibrate::self, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageCalibrate, // -V641 // -V1027                                                                                                                   //--- СЕРВИС - КАЛИБРАТОР ---
    "КАЛИБРОВКА",
    "Управлением калибратором и калибровка осциллографа",
    &cCalibrator_Calibrator,     /// СЕРВИС - КАЛИБРАТОР - Калибратор
    &bCalibrator_Calibrate,      /// СЕРВИС - КАЛИБРАТОР - Калибровать
    PageName::Service_Calibrator, &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)
