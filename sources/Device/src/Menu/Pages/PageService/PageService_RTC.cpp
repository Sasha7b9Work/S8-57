#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


extern const Page pageRTC;
extern const Page pageRTC_Set;
extern const Page pageRTC_Correction;

const Page * const PageRTC::self = (const Page *)&pageRTC;
const Page * const PageRTC::PageCorrect::self = (const Page *)&pageRTC_Correction;
const Page * const PageRTC::PageSet::self = (const Page *)&pageRTC_Set;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageRTC, // -V641 // -V1027                                                                                                                              //--- СЕРВИС - ВРЕМЯ ---
    "ВРЕМЯ",
    "Установка и настройка времени",
    PageRTC::PageSet::self,         ///< СЕРВИС - ВРЕМЯ - Время
    PageRTC::PageCorrect::self,     ///< CЕРВИС - ВРЕМЯ - Коррекция
    PageName::Service_RTC,
    &PageService::self, 0, 0, 0, 0
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
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetRight,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetUp,
    "Больше",
    "Увеличить",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetDown,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_BUTTON( bSetPick,
    "Выбрать",
    "Выбор подсвеченного элемента",
    &PageRTC::PageSet::self, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Set, //-V641 //-V1027
    "УСТАНОВКА",
    "Установка текущего времени",
    &bSetLeft,
    &bSetRight,
    &bSetUp,
    &bSetDown,
    &bSetPick,
    PageName::Service_RTC_Set,
    &PageRTC::self, 0, 0, 0, 0
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
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrRight,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrUp,
    "Больше",
    "Увеличить",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrDown,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_BUTTON( bCorrPick,
    "Выбор",
    "Активировать подсвеченный элемент",
    &PageRTC::PageCorrect::self, 0, 0, 0
)

DEF_PAGE_5( pageRTC_Correction, //-V641 //-V1027
    "КОРРЕКЦИЯ",
    "Коррекция скорости хода часов",
    &bCorrLeft,
    &bCorrRight,
    &bCorrUp,
    &bCorrDown,
    &bCorrPick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, 0, 0, 0, 0
)
