#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCalibrate;



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA::Settings::LoadCalibratorMode();
}

DEF_CHOICE_2(cCalibrator_Calibrator, // -V206                                                                                                     //--- СЕРВИС - КАЛИБРАТОР - Калибратор ---
    "Калибратор", "Calibrator",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Перем", "DC",
    "+4V", "+4V",
    //"0V", "0V",
    CALIBRATOR_MODE, pageCalibrate, FuncActive, OnChanged_Calibrator_Calibrator, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrator_Calibrate()
{
    return !(SET_CALIBR_MODE_A == CalibrationMode::Disable && CALIBR_MODE_B == CalibrationMode::Disable);
}

static void OnPress_Calibrator_Calibrate()
{
}

DEF_BUTTON(bCalibrator_Calibrate,                                                                                                                //--- СЕРВИС - КАЛИБРАТОР - Калибровать ---
    "Калибровать", "Calibrate",
    "Запуск процедуры калибровки",
    "Running the calibration procedure",
    pageCalibrate, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(pageCalibrate, // -V641 // -V1027                                                                                                                    //--- СЕРВИС - КАЛИБРАТОР ---
    "КАЛИБРОВКА", "CALIBRATE",
    "Управлением калибратором и калибровка осциллографа",
    "Control of the calibrator and calibration of an oscillograph",
    &cCalibrator_Calibrator,     // СЕРВИС - КАЛИБРАТОР - Калибратор
    &bCalibrator_Calibrate,      // СЕРВИС - КАЛИБРАТОР - Калибровать
    Page::Name::Service_Calibrator, PageService::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)


const PageBase *PageService::PageCalibrate::pointer = &pageCalibrate;

