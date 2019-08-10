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
    /// Функция вызывается при нажатии, повторе и отпускании кнопки
    static void ButtonEvent(KeyEvent event);
    /// Заблокировать клавиатуру. Будут разрешены только кнопки из массива keys, заканчивающегося нулём
    static void LockKeyboard(const Key::E * const keys);
    /// Разблокировать клавиатуру
    static void UnlockKeyboard();
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);
    /// Показать/скрыть меню.
    static void Show(bool show);
    /// Возвращает true, если меню отображается на экране
    static bool IsShown();
    /// Функция временно включает отображение строки навигации меню, если задано настройками.
    static void TemporaryEnableStrNavi();
    /// Возвращает адрес открытого элемента меню
    static Control *OpenedItem();
    /// Возвращает указатель на текущую главную страницу меню
    static Page *MainPage();
    /// Если true - меню находится в дополнительном режиме.
    static bool IsMinimize();

    static const Control *ItemUnderButton(Key::E button);

    static void SetItemUnderButton(Key::E button, const Control *control);

    static Page::Name::E GetNameOpenedPage();

    static void SaveScreenToDrive();

    static void SaveRow(int row);

    static Page *OpenedPage();
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Control *CurrentItem();
    /// Закрыть открытый элемент меню
    static void CloseOpenedItem();

    static void SetItemForHint(void *item);

    static void SaveSettings();
    /// Нужно вызывать при отпускании функциональной кнпоки
    static void ReleaseFunctionalButton(Key::E key);
    /// Нажно вызывать при "длинном" нажатии функциональной кнопки
    static void LongFunctionalButton(Key::E key);
    /// С какоей позиции Y рисовать меню. Позиция берётся без учёта заголовка
    static int Y();

    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static const Control *itemUnderKey;
    /// Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    static const char *stringForHint;
    /// Item, подсказку для которого нужно выводить в случае включённого режима подсказок.
    static Control *itemHint;

    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 2;
    };

    struct Item
    {
        static const int HEIGHT = 23;

        struct Value
        {
            static const int HEIGHT = 13;
            static const int WIDTH = 320 / 5 - 4; // -V112
        };
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

    static void SetMainPage(Page *page);
};
