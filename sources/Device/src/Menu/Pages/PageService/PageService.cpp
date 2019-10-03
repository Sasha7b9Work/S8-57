#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


static void OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::State::InModeOsci())
    {
        Osci::Start();
    }
}

DEF_BUTTON( bResetSettings,                                                                                                                                 //--- СЕРВИС - Сброс настроек ---
    "Сброс настр",
    "Сброс настроек на настройки по умолчанию",
    &PageService::self, Item::Active, OnPress_ResetSettings
)


static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- СЕРВИС - Поиск сигнала ---
    "Поиск сигн",
    "Устанавливает оптимальные установки осциллографа для сигнала в канале 1",
    &PageService::self, Item::Active, OnPress_AutoSearch
)


DEF_CHOICE_4( cSoundVolume,
    "Громкость",
    "Установка уровня громкости",
    "Откл",
    "Мин",
    "Средн",
    "Макс",
    set.serv.soundVolume, &PageService::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_7( pService, // -V641 // -V1027                                                                                                                                     //--- СЕРВИС ---
    "СЕРВИС",
    "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
    &bResetSettings,
    &bAutoSearch,
    PageService::PageCalibrate::self,
    &cSoundVolume,
    PageRTC::self,
    PageService::PageInformation::self,
    PageDebug::self,
    PageName::Service, nullptr, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageService::self = (const Page *)&pService;
