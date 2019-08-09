#include "defines.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageService.h"
#include "Keyboard/Keyboard.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include <cstdio>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const PageBase pageService;

const PageBase *PageService::pointer = &pageService;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::State::InModeOsci())
    {
        FPGA::OnPressStart();
    }
}

DEF_BUTTON( bResetSettings,                                                                                                                                 //--- СЕРВИС - Сброс настроек ---
    "Сброс настр",
    "Сброс настроек на настройки по умолчанию",
    pageService, EmptyFuncBtV, OnPress_ResetSettings, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- СЕРВИС - Поиск сигнала ---
    "Поиск сигн",
    "Устанавливает оптимальные установки осциллографа для сигнала в канале 1",
    pageService, 0, OnPress_AutoSearch, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cSoundVolume,
    "Громкость",
    "Установка уровня громкости",
    "Откл",
    "Мин",
    "Средн",
    "Макс",
    SOUND_VOLUME, pageService, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7( pageService, // -V641 // -V1027                                                                                                                                    //--- СЕРВИС ---
    "СЕРВИС",
    "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
    &bResetSettings,                        ///< СЕРВИС - Сброс настроек
    &bAutoSearch,                           ///< СЕРВИС - Поиск сигнала
    PageService::PageCalibrate::pointer,    ///< СЕРВИС - КАЛИБРОВКА
    &cSoundVolume,                          ///< СЕРВИС - ЗВУК
    PageService::PageRTC::pointer,          ///< СЕРВИС - ВРЕМЯ
    PageService::PageInformation::pointer,  ///< СЕРВИС - ИНФОРМАЦИЯ
    PageService::PageDebug::pointer,        ///< СЕРВИС - ОТЛАДКА
    Page::Name::Service, nullptr, 0, 0, 0, 0
)
