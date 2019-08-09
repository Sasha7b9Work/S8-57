#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


extern const PageBase pageRTC;
extern const PageBase pageRTC_Set;
extern const PageBase pageRTC_Correction;

const PageBase * const PageService::PageRTC::pointer = &pageRTC;
const PageBase * const PageService::PageRTC::PageCorrect::pointer = &pageRTC_Correction;
const PageBase * const PageService::PageRTC::PageSet::pointer = &pageRTC_Set;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageRTC, // -V641 // -V1027                                                                                                                              //--- СЕРВИС - ВРЕМЯ ---
    "ВРЕМЯ",
    "Установка и настройка времени",
    PageService::PageRTC::PageSet::pointer,         ///< СЕРВИС - ВРЕМЯ - Время
    PageService::PageRTC::PageCorrect::pointer,     ///< CЕРВИС - ВРЕМЯ - Коррекция
    Page::Name::Service_RTC, PageService::pointer, 0, 0, 0, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static int8 dServicetime = 0;
//static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
//DEF_TIME(tRTC_Time,                                                                                                                                         //--- СЕРВИС - ВРЕМЯ - Время ---
//    "Время"
//    ,
//    "Установка текущего времени.\nПорядок работы:\n"
//    "Нажать на элемент меню \"Время\". Откроется меню установки текущего времени. Короткими нажатиями кнопки на цифровой клавиатуре, соответсвующей "
//    "элементу управления \"Время\", выделить часы, минуты, секунды, год, месяц, или число. Выделенный элемент обозначается мигающей областью. "
//    "Вращением ручки УСТАНОВКА установить необходимое значение. Затем выделить пункт \"Сохранить\", нажать и удреживать более 0.5 сек кнопку на "
//    "панели управления. Меню установки текущего временя закроется с сохранением нового текущего времени. Нажатие длительное удержание кнопки на "
//    "любом другом элементе приведёт к закрытию меню установки текущего вре    мени без сохранения нового текущего времени"
//    ,
//    pageRTC_Set, 0, dServicetime, hours, minutes, secondes, month, day, year
//)

DEF_BUTTON( bSetLeft,
    "Влево",
    "Предыдущий элемент",
    pageRTC_Set, 0, 0, 0
)

DEF_BUTTON( bSetRight,
    "Вправо",
    "Следующий элемент",
    pageRTC_Set, 0, 0, 0
)

DEF_BUTTON( bSetUp,
    "Больше",
    "Увеличить",
    pageRTC_Set, 0, 0, 0
)

DEF_BUTTON( bSetDown,
    "Меньше",
    "Уменьшить",
    pageRTC_Set, 0, 0, 0
)

DEF_BUTTON( bSetPick,
    "Выбрать",
    "Выбор подсвеченного элемента",
    pageRTC_Set, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Set, //-V641 //-V1027
    "УСТАНОВКА",
    "Установка текущего времени",
    &bSetLeft,
    &bSetRight,
    &bSetUp,
    &bSetDown,
    &bSetPick,
    Page::Name::Service_RTC_Set, PageService::PageRTC::pointer, 0, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void OnChanged_Time_Correction()
//{
//}
//
//DEF_GOVERNOR(tRTC_Correction,                                                                                                                           //--- СЕРВИС - ВРЕМЯ - Коррекция ---
//    "Коррекция",
//    "Установка корректирующего коэффициента для компенсации хода времени",
//    NRST_CORRECTION_TIME, -63, 63, pageRTC_Correction, 0, OnChanged_Time_Correction, 0
//)

DEF_BUTTON( bCorrLeft,
    "Влево",
    "Предыдущий элемент",
    pageRTC_Correction, 0, 0, 0
)

DEF_BUTTON( bCorrRight,
    "Вправо",
    "Следующий элемент",
    pageRTC_Correction, 0, 0, 0
)

DEF_BUTTON( bCorrUp,
    "Больше",
    "Увеличить",
    pageRTC_Correction, 0, 0, 0
)

DEF_BUTTON( bCorrDown,
    "Меньше",
    "Уменьшить",
    pageRTC_Correction, 0, 0, 0
)

DEF_BUTTON( bCorrPick,
    "Выбор",
    "Активировать подсвеченный элемент",
    pageRTC_Correction, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Correction, //-V641 //-V1027
    "КОРРЕКЦИЯ",
    "Коррекция скорости хода часов",
    &bCorrLeft,
    &bCorrRight,
    &bCorrUp,
    &bCorrDown,
    &bCorrPick,
    Page::Name::Service_RTC_Correct, PageService::PageRTC::pointer, 0, 0, 0, 0
)
