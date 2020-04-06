#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


void PageService::OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::InModeOsci())
    {
        Osci::Start(true);
    }
}

DEF_BUTTON( bResetSettings,                                                                                                                                 //--- СЕРВИС - Сброс настроек ---
    "Сброс настр",
    "Сброс настроек на настройки по умолчанию",
    &PageService::self, Item::Active, PageService::OnPress_ResetSettings
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
    set.serv.enumVolume, &PageService::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_7_VAR( pService,                                                                                                                                                        //--- СЕРВИС ---
    "СЕРВИС",
    "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
    &bResetSettings,
    &bAutoSearch,
    PageService::Calibrate::self,
    &cSoundVolume,
    PageRTC::self,
    PageService::Information::self,
    //&Item::empty,
    PageDebug::self,
    PageName::Service, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageService::self = static_cast<const Page *>(&pService);


void PageService::DecodePassword(const KeyEvent &event)
{
#define NUM_SYMBOLS 10

    static const Key::E password[NUM_SYMBOLS] =
    {
        Key::F1, Key::F1,
        Key::F2, Key::F2,
        Key::F3, Key::F3,
        Key::F4, Key::F4,
        Key::F5, Key::F5
    };

    // Число совпавших символов
    static int charsMatch = 0;

    if(Menu::IsShown())
    {
        charsMatch = 0;
        return;
    }

    if(event.type != TypePress::Release)
    {
        return;
    }

    if(password[charsMatch++] == event.key)
    {
        if(charsMatch == NUM_SYMBOLS)
        {
            Page *page = const_cast<Page *>(&pService);

            Item **items = const_cast<Item **>(page->OwnData()->items);

            items[6] = const_cast<Page *>(PageDebug::self);

            DISPLAY_SHOW_WARNING("Доступ к меню ОТЛАДКА открыт");
        }
    }
    else
    {
        charsMatch = 0;
    }
}
