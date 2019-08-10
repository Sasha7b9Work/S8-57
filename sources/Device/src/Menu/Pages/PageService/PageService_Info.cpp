#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"
#include <cstdio>


extern const PageBase pageServiceInfo;

const Page * const PageService::PageInformation::self = (const Page *)&pageServiceInfo;

using namespace Display::Primitives;

static void Information_Draw()
{
    Painter::BeginScene(Color::BACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, Color::FILL);
    y += dY;
    String("ИНФОРМАЦИЯ").Draw(x, y);
    y += dY;

    char buffer[100];

    std::sprintf(buffer, "%s : %s", "Модель", MODEL_RU);
    String(buffer).Draw(x, y);

    y += 2 * dY;

    String("Программное обеспечение :").Draw(x, y);
    y += dY;

    std::sprintf(buffer, "версия %s", NUM_VER);

    String(buffer).Draw(x, y);
    y += dY;

    //Painter::DrawFormText(x, y, Color::FILL, "CRC32 : %X", Hardware::CalculateCRC32());

    dY = -10;

    Text("Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ").DrawInCenterRect(0, 190 + dY, 320, 20);
    Text("Отдел маркетинга: тел./факс. 8-017-262-57-50").DrawInCenterRect(0, 205 + dY, 320, 20);
    Text("Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-262-57-51").DrawInCenterRect(0, 220 + dY, 320, 20);

    Menu::Draw();
    Painter::EndScene();
}


static void OnPress_Close()
{
    Menu::CloseOpenedItem();
}

DEF_BUTTON( bClose,
    "Закрыть",
    "Закрыть страницу информации",
    &PageService::PageInformation::self, 0, OnPress_Close, 0
)

static void OnPress_Page(bool enter)
{
    if (enter)
    {
        Display::SetAddDrawFunction(Information_Draw);
        static const Key::E keys[2] = { Key::Enter, Key::None };
        Menu::LockKeyboard(keys);
    }
    else
    {
        Display::RemoveAddDrawFunction();
        Menu::UnlockKeyboard();
    }
}

DEF_PAGE_1 (pageServiceInfo, //-V641 //-V1027
    "ИНФОРМАЦИЯ",
    "Информация об изделии",
    &bClose,
    Page::Name::Service_Info, &PageService::self, 0, OnPress_Page, 0, 0
)
