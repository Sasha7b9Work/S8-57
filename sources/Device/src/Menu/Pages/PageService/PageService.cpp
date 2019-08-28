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
#include "Menu/Pages/Include/PageService.h"
#include "Keyboard/Keyboard.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include <cstdio>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const Page pageService;

const Page * const PageService::self = (const Page *)&pageService;


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
    &PageService::self, E_BtV, OnPress_ResetSettings
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- СЕРВИС - Поиск сигнала ---
    "Поиск сигн",
    "Устанавливает оптимальные установки осциллографа для сигнала в канале 1",
    &PageService::self, E_BtV, OnPress_AutoSearch
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cSoundVolume,
    "Громкость",
    "Установка уровня громкости",
    "Откл",
    "Мин",
    "Средн",
    "Макс",
    SOUND_VOLUME,
    &PageService::self, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7( pageService, // -V641 // -V1027                                                                                                                                    //--- СЕРВИС ---
    "СЕРВИС",
    "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
    &bResetSettings,                        ///< СЕРВИС - Сброс настроек
    &bAutoSearch,                           ///< СЕРВИС - Поиск сигнала
    PageService::PageCalibrate::self,    ///< СЕРВИС - КАЛИБРОВКА
    &cSoundVolume,                          ///< СЕРВИС - ЗВУК
    PageRTC::self,          ///< СЕРВИС - ВРЕМЯ
    PageService::PageInformation::self,  ///< СЕРВИС - ИНФОРМАЦИЯ
    PageDebug::self,        ///< СЕРВИС - ОТЛАДКА
    PageName::Service, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)
