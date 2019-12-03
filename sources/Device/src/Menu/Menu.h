#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


/// Возвращает позицию активного пункта на странице Page::Name.
#define MENU_POS_ACT_ITEM(name)     (set.menu.posActItem[name])


struct SettingsMenu
{
    bool    show;                               ///< Если true, то нужно показывать текущую страницу
    int8    posActItem[PageName::Count];        ///< \brief Позиция активного пункта. bit7 == 1 - item is opened, 0x7f - нет активного пункта.
    int8    currentSubPage[PageName::Count];    ///< Номер текущей подстраницы.
};



struct Menu
{
    static void Init();

    static void ChangeStateFlashDrive();

    static void Update();

    static void Draw();
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);
    
    static void Show();

    static void Hide();

    static bool IsShown();

    static Item *OpenedItem();
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Item *CurrentItem();

    static Page *OpenedPage();

    static void CloseOpenedItem();

    static void SetItemForHint(const Item *item);

    static void SaveSettings();
    /// С какоей позиции Y рисовать меню. Позиция берётся без учёта заголовка
    static int Y0();

    static const Item *ItemUnderFunctionalKey(Key::E key);
    /// Текущая главная страница
    static const Page *mainPage;

    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 1;
    };

private:
    /// Последний открытый контрол на дереве странице page
    static Item *LastOpened(Page *page);
    /// Обработка события таймера автоматического сокрытия меню
    static void OnTimerAutoHide();

    static void ProcessButtonForHint(Key::E button);
    /// Написать подсказку
    static void DrawHint();
    /// Закрыть все страницы, которые не могут быть открытыми при включении
    static void CloseAllBadOpenedPages();
    // Закрыть parent, если он является хранителем page
    static void CloseIfSubPage(Page *parent, Page *page);

    static void ClosePage(Page *page);

    static void DrawHintItem(int x, int y, int width);
    /// Итем, для которого нужно выводить подсказку
    static Item *itemHint;
    /// Нужно для того, чтобы периодически сохранять настройки
    static uint timeLastKeyboardEvent;
    /// Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    static const char *stringForHint;
};
