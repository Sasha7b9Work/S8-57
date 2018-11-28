#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Menu
 *  @{
 *  @defgroup Pages
 *  @{
 *  @}
 */

#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
friend class Page;

public:
    
    static PageBase *pageMain;

    /// Инициализация
    static void Init();
    /// Возвращает true, если item - адрес главной страницы меню.
    static bool IsMainPage(const void *item);

    static void ChangeStateFlashDrive();
    /// Функция должна вызываться в главном цикле.
    static void Update();
    /// Функция обработки короткого нажатия кнопки (менее 0.5 сек.).
    static void ShortPressureButton(Key::E button);
    /// Функция обработки длинного нажатия кнопки (более 0.5 сек.).
    static void LongPressureButton(Key::E button);
    /// Функция вызывается при нажатии, повторе и отпускании кнопки
    static void ButtonEvent(KeyEvent event);
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);
    /// Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    static char *StringNavigation(char buffer[100]);
    /// Показать/скрыть меню.
    static void Show(bool show);
    /// Возвращает true, если меню отображается на экране
    static bool IsShown();

    static void OpenItemTime();
    /// Функция временно включает отображение строки навигации меню, если задано настройками.
    static void TemporaryEnableStrNavi();
    /// Ввыполнить эту функцию после Menu_UpdateInput().
    static void RunAfterUpdate(pFuncVV func);
    /// Возвращает адрес открытого элемента меню
    static Control *OpenedItem();
    /// Если true - меню находится в дополнительном режиме.
    static bool IsMinimize();

    static Page::Name::E GetNameOpenedPage();
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Control *CurrentItem();
    /// Закрыть открытый элемент меню
    static void CloseOpenedItem();
    /// Уменьшает или увеличивает значение Governor, GovernorColor или Choice по адресу item в зависимости от знака delta
    static void ChangeItem(Control *item, int delta);
    /// Вернуть указатель на малую кнопку, соответствующую данной кнопки панели.
    static const SButton* GetDrawButton(Key::E button);

    static void SetItemForHint(void *item);

    static void SaveSettings();
    /// Нужно вызывать при отпускании функциональной кнпоки
    static void ReleaseFunctionalButton(Key::E key);
    /// Нажно вызывать при "длинном" нажатии функциональной кнопки
    static void LongFunctionalButton(Key::E key);
    ///\brief  Здесь хранится адрес элемента меню, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0, 
    /// если ни одна кнопка не нажата.
    static Control *itemUnderKey;
    /// Строка подсказки, которую надо выводить в случае включённого режима подсказок.
    static const char *stringForHint;
    /// Item, подсказку для которого нужно выводить в случае включённого режима подсказок.
    static Control *itemHint;

    /// Нарисовать меню
    static void Draw();
    /// С какоей позиции Y рисовать меню. Позиция берётся без учёта заголовка
    static int Y();

private:

    static void *RetLastOpened(Page *_page, Control::Type *_type);
    /// Обработка короткого нажатия кнопки
    static void ProcessingShortPressureButton();
    /// Обработка длинного нажатия кнопки
    static void ProcessingLongPressureButton();
    /// Обработка опускания кнопки вниз
    static void ProcessingPressButton();
    /// Обработка поднятия кнопки вверх
    static void ProcessingReleaseButton();
    /// Обработка события таймера автоматического сокрытия меню
    static void OnTimerAutoHide();
    /// Функция, которая отключит вывод строки навигации меню
    static void OnTimerStrNaviAutoHide();
    /// Возвращает true, если лампочка УСТАНОВКА должна гореть
    static bool NeedForFireSetLED();
    
    static void ProcessButtonForHint(Key::E button);
    
    static void ShortPress_ChoiceReg(void *choice);
    
    static void ResetItemsUnderButton();
    /// Возвращает true, если данная кнопка обрабатыватся в данном режиме
    static bool IsProcessed(KeyEvent *event);
    /// Возвращает страницу меню, которая должна открываться по нажатию кнопки button.
    static const void *PageForButton(Key::E button);
    /// Если произошло короткое нажатие кнопки, то здесь хранится имя этой кнопки до обработки  этого нажатия.
    static Key::E shortPressureButton;
    /// Если произошло длинное нажатие кнопки, то здесь хранится имя этой кнопки до обработки этого нажатия.
    static Key::E longPressureButton;
    /// При нажатии кнопки её имя записывается в эту переменную и хранится там до обратоки события нажатия кнопки.
    static Key::E pressButton;
    /// При отпускании кнопки её имя записывается в эту переменную и хранится там до обработки события отпускания кнопки.
    static Key::E releaseButton;
    /// Эта функция будет вызывана один раз после Menu::Update().
    static pFuncVV funcAterUpdate;
    /// Время последнего нажатия кнопки. Нужно для того, чтобы периодически сохранять настройки
    static uint timeLastPressedButton;
    /// Элементы управления, назначенные в данный момент соответствующим кнопкам
    static Control *itemUnderButton[Key::Number];
    /// Эта функция будет вызываться каждый кадр
    static pFuncVV funcUpdate;

public:

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
        static const int WIDTH = 320 / 5;
        static const int WIDTH_OPENED = WIDTH;

        class Value
        {
        public:
            static const int HEIGHT = 13;
            static const int WIDTH = Menu::Item::WIDTH - 4;
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
};


/** @}
 */
