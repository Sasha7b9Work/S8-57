#include "defines.h"
#include "Settings/Settings.h"


DEF_GOVERNOR( gTimeDisableDisplay,
    "Дисплей",
    "Время отключения дисплея",
    S_SERV_TIME_DISABLE_DISPLAY, 0, 240, &PagePowerSaving::self, Item::Active, Governor::Changed
)


DEF_GOVERNOR( gTimeDisablePower,
    "Питание",
    "Время отключения питания",
    S_SERV_TIME_DISABLE_POWER, 0, 240, &PagePowerSaving::self, Item::Active, Governor::Changed
)


DEF_PAGE_2( pPowerSaving,
    "ЭН.СБЕР",
    "Настройка параметров энергосбережения",
    &gTimeDisableDisplay,
    &gTimeDisablePower,
    PageName::Service_PowerSaving,
    &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PagePowerSaving::self = static_cast<const Page *>(&pPowerSaving);
