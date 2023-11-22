// 2023/11/22 13:42:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


#define S_MENU_SHOW                     (set.menu._show)
#define S_MENU_POS_ACT_ITEM(name)       (set.menu._posActItem[name])
#define S_MENU_CURRENT_SUB_PAGE(name)   (set.menu._currentSubPage[name])
#define S_MENU_MAIN_PAGE                (set.menu._mainPage)


struct SettingsMenu
{
    bool        _show;                               // Если true, то нужно показывать текущую страницу
    int8        _posActItem[PageName::Count];        // \brief Позиция активного пункта. bit7 == 1 - item is opened, 0x7f - нет активного пункта.
    int8        _currentSubPage[PageName::Count];    // Номер текущей подстраницы.
    PageName::E _mainPage;                           // Имя текущей главной страницы
};



namespace Menu
{
    struct Position
    {
        // Возвращает позицию активного пункта на странице name
        static int8 &ActItem(PageName::E name);
    };

    void Init();

    void ChangeStateFlashDrive();

    void Update();

    void Draw();

    // Установить время автоматического сокрытия меню в соответствии с установками.
    void SetAutoHide(bool active);

    void Show();

    void Hide();

    bool IsShown();

    Item *OpenedItem();

    // Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    Item *CurrentItem();

    Page *OpenedPage();

    // Возвращает указатель на текущую главную страницу
    const Page *GetMainPage();

    void CloseOpenedItem();

    void SetItemForHint(const Item *item);

    // С какоей позиции Y рисовать меню. Позиция берётся без учёта заголовка
    int Y0();

    const Item *ItemUnderFunctionalKey(Key::E key);

    // Возвращает указаетль на страницу с именем name
    Page *PageFromName(PageName::E name);

    // Устанавливает текущую главную страницу
    void SetMainPage(const Page *page);

    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 1;
    };
};
