#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Menu
{
    /// Инициализация
    void Init();

    void ChangeStateFlashDrive();
    /// Функция должна вызываться в главном цикле.
    void Update();
    /// Функция вызывается при нажатии, повторе и отпускании кнопки
    void ButtonEvent(KeyEvent event);
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    void SetAutoHide(bool active);
    /// Показать/скрыть меню.
    void Show(bool show);
    /// Возвращает true, если меню отображается на экране
    bool IsShown();
    /// Функция временно включает отображение строки навигации меню, если задано настройками.
    void TemporaryEnableStrNavi();
    /// Возвращает адрес открытого элемента меню
    Control *OpenedItem();
    /// Возвращает указатель на текущую главную страницу меню
    PageBase *MainPage();
    /// Если true - меню находится в дополнительном режиме.
    bool IsMinimize();

    Control *ItemUnderButton(Key::E button);

    void SetItemUnderButton(Key::E button, Control *control);

    Page::Name::E GetNameOpenedPage();

    void SaveScreenToDrive();

    void SaveRow(int row);

    PageBase *OpenedPage();
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    Control *CurrentItem();
    /// Закрыть открытый элемент меню
    void CloseOpenedItem();

    void SetItemForHint(void *item);

    void SaveSettings();
    /// Нужно вызывать при отпускании функциональной кнпоки
    void ReleaseFunctionalButton(Key::E key);
    /// Нажно вызывать при "длинном" нажатии функциональной кнопки
    void LongFunctionalButton(Key::E key);
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    extern Control *itemUnderKey;
    /// Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    extern const char *stringForHint;
    /// Item, подсказку для которого нужно выводить в случае включённого режима подсказок.
    extern Control *itemHint;

    /// Нарисовать меню
    void Draw();
    /// С какоей позиции Y рисовать меню. Позиция берётся без учёта заголовка
    int Y();

    class Title
    {
    public:
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 2;
    };

    class Item
    {
    public:
        static const int HEIGHT = 23;

        class Value
        {
        public:
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

    void SetMainPage(PageBase *page);
};
