// 2023/11/22 13:42:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


namespace PageService
{
    namespace Battery
    {
        DEF_CHOICE_2(cShow,                                                                                                                                  //--- ������ - ������� - ���������� ---  
            "����������",
            "��������/��������� ����� ����������� � ���������� ������� �� �����",
            DISABLE_RU,
            ENABLE_RU,
            S_SERV_SHOW_INFO_VOLTAGE, &PageService::Battery::self, Item::Active, Choice::Changed, Choice::AfterDraw
        )


        DEF_PAGE_1(pBattery,                                                                                                                                              //--- ������ - ������� --- //-V2567
            "�������",
            "������� ������ � ��������������",
            &cShow,
            PageName::Service_Battery, &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
        )
    }
}

const Page *const PageService::Battery::self = static_cast<const Page *>(&PageService::Battery::pBattery);
