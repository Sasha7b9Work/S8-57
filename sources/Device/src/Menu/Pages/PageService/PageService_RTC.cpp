#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pRTC, // -V641 // -V1027                                                                                                                                 //--- СЕРВИС - ВРЕМЯ ---
    "ВРЕМЯ",
    "Установка и настройка времени",
    PageRTC::PageSet::self,         ///< СЕРВИС - ВРЕМЯ - Время
    PageRTC::PageCorrect::self,     ///< CЕРВИС - ВРЕМЯ - Коррекция
    PageName::Service_RTC,
    &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::self = (const Page *)&pRTC;


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
//    pSet, 0, dServicetime, hours, minutes, secondes, month, day, year
//)

DEF_BUTTON( bSet_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Up,
    "Больше",
    "Увеличить",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_BUTTON( bSet_Pick,
    "Выбрать",
    "Выбор подсвеченного элемента",
    &PageRTC::PageSet::self, E_BtV, E_VV
)

DEF_PAGE_5( pSet, //-V641 //-V1027
    "УСТАНОВКА",
    "Установка текущего времени",
    &bSet_Left,
    &bSet_Right,
    &bSet_Up,
    &bSet_Down,
    &bSet_Pick,
    PageName::Service_RTC_Set,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageSet::self = (const Page *)&pSet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void OnChanged_Time_Correction()
//{
//}
//
//_DEF_GOVERNOR(tRTC_Correction,                                                                                                                           //--- СЕРВИС - ВРЕМЯ - Коррекция ---
//    "Коррекция",
//    "Установка корректирующего коэффициента для компенсации хода времени",
//    NRST_CORRECTION_TIME, -63, 63, pCorrection, 0, OnChanged_Time_Correction, 0
//)

DEF_BUTTON( bCorrection_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Up,
    "Больше",
    "Увеличить",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

DEF_BUTTON( bCorrection_Pick,
    "Выбор",
    "Активировать подсвеченный элемент",
    &PageRTC::PageCorrect::self, E_BtV, E_VV
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pCorrection, //-V641 //-V1027
    "КОРРЕКЦИЯ",
    "Коррекция скорости хода часов",
    &bCorrection_Left,
    &bCorrection_Right,
    &bCorrection_Up,
    &bCorrection_Down,
    &bCorrection_Pick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageCorrect::self = (const Page *)&pCorrection;
