#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Menu
{
    /// Инициализация
    static void Init();

    static void ChangeStateFlashDrive();
    /// Функция должна вызываться в главном цикле.
    static void Update();
    /// Нарисовать меню
    static void Draw();
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);
    /// Показать/скрыть меню.
    static void Show(bool show);
    /// Возвращает true, если меню отображается на экране
    static bool IsShown();
    /// Функция временно включает отображение строки навигации меню, если задано настройками.
    static void TemporaryEnableStrNavi();
    /// Возвращает адрес открытого элемента меню
    static Item *OpenedItem();

    static Page::Name::E GetNameOpenedPage();

    static void SaveScreenToDrive();

    static void SaveRow(int row);

    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Item *CurrentItem();
    /// Закрыть открытый элемент меню
    static void CloseOpenedItem();

    static void SetItemForHint(const Item *item);

    static void SaveSettings();
    /// С какоей позиции Y рисовать меню. Позиция берётся без учёта заголовка
    static int Y0();
    /// Возвращает адрес элемента, соответствующего функциональной кнопкке
    static const Item *ItemForFuncKey(Key::E key);
    /// Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    static const char *stringForHint;
    /// Текущая главная страница
    static const Page *mainPage;

    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 2;
    };

    /// Через какое время после последнего нажатия кнопки скрывать меню.
    struct AutoHide
    {
        enum E
        {
            None = 0,   ///< Никогда.
              _5 = 5,   ///< Через 5 секунд.
             _10 = 10,  ///< Через 10 секунд.
             _15 = 15,  ///< Через 15 секунд.
             _30 = 30,  ///< Через 30 секунд.
             _60 = 60   ///< Через 60 секунд.
        } value;
    };
};
