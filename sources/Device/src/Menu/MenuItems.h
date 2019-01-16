#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuItems Menu Items
 *  @{
 */

extern int8 gCurDigit;

#define MENU_ITEMS_ON_DISPLAY       5   ///< Сколько пунктов меню помещается на экране по вертикали.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Общая часть для всех типов элементов меню
#define COMMON_PART_MENU_ITEM                                                                           \
    uint8           type;           /* Тип итема */                                                     \
    int8            num;            /* Число вариантов для Choice или число контролов для Page*/        \
    bool            isPageSB;       /* Если true, то это страница малых кнопок */                       \
    uint8           name;           /* Имя из перечисления Page::Name */                                  \
    const PageBase  *keeper;        /* Адрес страницы, которой принадлежит. Для Page_Main = 0 */        \
    pFuncBV         funcOfActive;   /* Активен ли данный элемент */                                     \
    const char      *titleHint[4]   /* Название страницы на русском и английском языках. Также подсказка для режима помощи */

class PageBase;

#define IS_PAGE(item)           (item->type == Control::Type::Page)
#define NOT_PAGE(item)          (item->type != Control::Type::Page)
#define IS_PAGE_SB(item)        (item->isPageSB)
#define IS_CHOICE(item)         (item->type == Control::Type::Choice)
#define IS_CHOICE_REG(item)     (item->type == Control::Type::ChoiceReg)
#define NOT_CHOICE_REG(item)    (item->type != Control::Type::ChoiceReg)
#define IS_GOVERNOR(item)       (item->type == Control::Type::Governor)
#define NOT_GOVERNOR(item)      (item->type != Control::Type::Governor)
#define IS_GOVERNOR_COLOR(item) (item->type == Control::Type::GovernorColor)
#define IS_TIME(item)           (item->type == Control::Type::Time)
    
#define KEEPER(item)            ((PageBase *)item->keeper)


class Control
{
public:
    COMMON_PART_MENU_ITEM;
    /// Возвращает высоту в пикселях открытого элемента Choice или Page::Name
    int HeightOpened() const;
    /// @brief Возвращает true, если элемент меню item затенён (находится не на самом верхнем слое. Как правило, это означает, что раскрыт 
    /// раскрывающийся элемент меню вроде Choice или Governor
    bool IsShade() const;
    /// Возвращает true, если кнопка, соответствующая элементу меню item, находится в нажатом положении
    bool IsPressed() const;
    /// Сделать/разделать текущим
    void SetCurrent(bool active);
    /// Возвращает true, если элемент меню по адрему item открыт
    bool IsOpened() const;

    void Open(bool open);
    /// Возвращает название элемента, как оно выглядит на дисплее прибора
    String Title() const;
    /// Обработка события кнопки
    bool ProcessKey(KeyEvent event);
    /// Вызывается при "коротком" нажатии
    void ShortPress();
    /// Вызывается при "длинном" нажатии
    void LongPress();
    /// Возвращает true, если контрол находится в активном состоянии (реагирует на органы управления)
    bool IsAcitve() { return funcOfActive(); };

    void Draw(int x, int y, bool opened);

    bool IsCurrentItem() const;
    /// Вывести информацию в лог
    void LogInfo() const;
    /// Возвращает изображение регулятора, соответствующее его текущему положению
    char GetSymbol();
    /// Возвращает ширину контрола
    int Width() { return 320 / 5; };
    /// Возвращает true, если тип контрола - Page
    bool IsPage() const;
    /// Возвращает адрес родителя
    const PageBase *Keeper() { return keeper; }
    /// Возвращает true, если в древе предков стоит keeper
    bool ExistKeeper(const PageBase *keeper);

    /// Разные виды пунктов меню
    struct Type
    {
        enum E
        {
            None,
            Choice,         ///< Пункт выбора - позволяет выбрать одно из нескольких заданных значений.
            Button,         ///< Кнопка.
            Page,           ///< Страница.
            Governor,       ///< Регулятор - позволяет выбрать любое целое числовое значение из заранее заданного диапазаона. Диапазон не может превышать [ -(1 << 16) / 2 , (1 << 16) / 2]
            Time,           ///< Позволяет ввести время.
            GovernorColor,  ///< Позволяет выбрать цвет.
            ChoiceReg,      ///< Элемент выбора, в котором выбор осуществляется не кнопкой, а ручкой
            DrawButton,     ///< Кнопка для режима малых кнопок
            ChoiceParameter,
            Number
        } value;

        explicit Type(E v) : value(v) {};
    };
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///

typedef bool(*pFuncBKE)(KeyEvent);

/// Описывает страницу меню.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    const Control * const *items;   ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                    ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB     funcOnEnterExit;    ///< Будет вызываться при нажатии на свёрнутую страницу и при выходе из этой страницы на предыдущую
    pFuncVV     funcOnDraw;         ///< Будет вызываться после отрисовки кнопок
    pFuncBKE    funcRegSet;         ///< Вызывается при нажатии кнопок ВЛЕВО/ВПРАВО. Если отработала, возвращает true
    bool CurrentItemIsOpened() const;
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SMALL_BUTTON_FROM_PAGE(page, numButton)     ((SButton *)((Page *)page)->items[numButton])

class Page : public Control
{
public:
    const Control * const *items;   ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                    ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB     funcOnEnterExit;    ///< Будет вызываться при нажатии на свёрнутую страницу и при выходе из этой страницы на предыдущую
    pFuncVV     funcOnDraw;         ///< Будет вызываться после отрисовки кнопок
    pFuncBKE    funcKey;            ///< В странице малых кнопок вызывается при нажатии стрелки
    /// Возвращает true, если текущий элемент страницы открыт
    bool CurrentItemIsOpened() const;
    /// Dозвращает число подстраниц в странице по адресу page
    int NumSubPages() const;        
    /// Возвращает количество элементов в странице по адресу page
    int NumItems() const;
    /// Установить текущей данную страницу.
    void SetAsCurrent();
    /// Возвращает номер текущей подстраницы (той, которая индицируется в данный момент)
    int8 CurrentSubPage() const;
    /// Установить текущей данную подстраницу
    void SetCurrentSubPage(int8 pos);
    ///< Устанавливает позицию активного пункта меню
    void SetPosActItem(int8 pos);
    ///< Возвращает указатель на заданный элемент страницы
    Control *Item(int numElement) const;
    /// \todo Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая 
    /// подстраница 1, это будет 5 и т.д.
    int PosItemOnLeft();
    /// Вызывает функцию короткого нажатия кнопки над итемом numItem страницы page
    void ShortPressOnItem(uint numItem);
    /// Возвращает позицию текущего элемента странцы page
    int8 PosCurrentItem() const;
    /// Изменить номер текущей подстраницы на значение delta
    void ChangeSubPage(int delta);
    /// Нарисовать в заданных координатах
    void Draw(int x, int y, bool opened);
    /// Обработка события кнопки
    bool ProcessKey(KeyEvent event);

    void DrawTitle(int x, int y);

    void DrawItems(int x, int y);

    static int ItemOpenedPosX(Control *item);

    void DrawPagesUGO(int right, int bottom);

    void DrawNestingPage(int left, int bottom);

    struct Name
    {
        enum E
        {
            Main,                                   ///< Главная страница меню

            Function,                               ///< ФУНКЦИЯ
            Function_FrequencyCounter,              ///< ФУНКЦИЯ - ЧАСТОТОМЕР
            Function_FFT,                           ///< ФУНКЦИЯ - СПЕКТР
            Function_Multimeter,                    ///< ФУНКЦИЯ - МУЛЬТИМЕТР
            Function_Tester,                        ///< ФУНКЦИЯ - ТЕСТЕР
            Function_Recorder,                      ///< ФУНКЦИЯ - РЕГИСТРАТОР
            Function_Recorder_Source,               ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК
            Function_Recorder_Record,               ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ
            Function_Recorder_Show,                 ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР
            Function_Recorder_Show_Operations,      ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ
            Function_Recorder_Show_View,            ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ПРОСМОТР
            Function_Recorder_Show_View_Cursors,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ПРОСМОТР - КУРСОРЫ

            Measures,                   ///< ИЗМЕРЕНИЯ
            Measures_Auto,              ///< ИЗМЕРЕНИЯ - АВТОМАТ
            Measures_Auto_Tune,         ///< ИЗМЕРЕНИЯ - АВТОМАТ - НАСТРОИТЬ
            Measures_Cursors,           ///< ИЗМЕРЕНИЯ - КУРСОРЫ
            Measures_Cursors_Set,       ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ

            Display,                    ///< ДИСПЕЙ
            Display_Accumulation,       ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ
            Display_Average,            ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ
            Display_Grid,               ///< ДИСПЛЕЙ - СЕТКА
            Display_Settings,           ///< ДИСПЛЕЙ - НАСТРОЙКИ
            Display_Settings_Colors,    ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА

            ChannelA,                   ///< КАНАЛ 1
            ChannelB,                   ///< КАНАЛ 2
            Trig,                       ///< СИНХР
            Trig_Search,                ///< СИНХР - ПОИСК
            Trig_HoldOff,               ///< СИНХР - УДЕРЖАНИЕ
            Time,                       ///< РАЗВЕРТКА
            Memory,                     ///< ПАМЯТЬ
            SB_Memory_Last,             ///< ПАМЯТЬ - ПОСЛЕДНИЕ
            SB_Memory_Internal,         ///< ПАМЯТЬ - ВНУТР ЗУ
            Memory_Drive,               ///< ПАМЯТЬ - ВНЕШН ЗУ
            SB_Memory_Drive_Manager,    ///< ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ
            SB_Memory_Drive_Mask,       ///< ПАМЯТЬ - ВНЕШН ЗУ - МАСКА
            SB_Memory_SetName,          ///< \brief Появляется перед сохранением файла на флешку для задания имени файла при соответствующей опции 
                                        ///< ВНЕШН ЗУ - Имя файла
            Service,                    ///< СЕРВИС
            Service_Calibrator,         ///< СЕРВИС - КАЛИБРАТОР
            Service_Ethernet,           ///< СЕРВИС - ETHERNET
            Service_Sound,              ///< СЕРВИС - ЗВУК
            Service_RTC,                ///< СЕРВИС - ВРЕМЯ
            SB_Service_FFT_Cursors,     ///< СЕРВИС - СПЕКТР - КУРСОРЫ
            SB_Service_Function,        ///< СЕРВИС - ФУНКЦИЯ
            SB_Service_Information,     ///< СЕРВИС - ИНФОРМАЦИЯ
            SB_Help,                    ///< ПОМОЩЬ
            Debug,                      ///< ОТЛАДКА
            Debug_Console,              ///< ОТЛАДКА - КОНСОЛЬ
            Debug_Console_Registers,    ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ
            Debug_ADC,                  ///< ОТЛАДКА - АЦП
            Debug_ADC_Balance,          ///< ОТЛАДКА - АЦП - БАЛАНС
            Debug_ADC_Stretch,          ///< ОТЛАДКА - АЦП - РАСТЯЖКА
            Debug_ADC_Shift,            ///< ОТЛАДКА - АЦП - ДОП СМЕЩ
            Debug_ADC_AltShift,         ///< ОТЛАДКА - АЦП - ДОП СМЕЩ ПАМ
            Debug_Rand,                 ///< ОТЛАДКА - РАНД-ТОР
            Debug_Channels,             ///< ОТЛАДКА - КАНАЛЫ
            SB_Debug_Settings,          ///< ОТЛАДКА - НАСТРОЙКИ
            SB_Debug_SerialNumber,      ///< ОТЛАДКА - С/Н
            Number,
            NoPage
        } value;
    };

    Page::Name::E GetName() const;     ///< Возвращает имя страницы page
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
/// Описывает кнопку.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
};

class Button : public Control
{
public:
    pFuncVV     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
    void Draw(int x, int y);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
struct StructHelpDrawButton
{
    pFuncVII    funcDrawUGO;    ///< Указатель на функцию отрисовки изображения варианта кнопки
    pString     helpUGO[2];     ///< Подпись к данному изображению.
};


/// Описывает кнопку для дополнительного режима меню.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV                         funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                        funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpDrawButton     *hintUGO;
    int                             numHints;
};


class SButton : public Control
{
public:
    pFuncVV                     funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                    funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpDrawButton *hintUGO; 
    int                         numHints;
    void Draw(int x, int y);
    void DrawHints(int x, int y, int width);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
/// Описывает регулятор.
class GovernorBase
{
public:
    COMMON_PART_MENU_ITEM;
    int    *cell;
    int     minValue;       ///< Минмальное значение, которое может принимать регулятор.
    int     maxValue;       ///< Максимальное значение.
    pFuncVV funcOfChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой
};

class Governor : public Control
{
public:
    int    *cell;
    int     minValue;       ///< Минмальное значение, которое может принимать регулятор.
    int     maxValue;       ///< Максимальное значение.
    pFuncVV funcOfChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой
    /// Обработка события кнопки
    bool ProcessKey(KeyEvent event);
    /// Запускает процессс анимации инкремента или декремента элемента меню типа Governor (в зависимости от знака delta).
    void StartChange(int detla);
    /// Возвращает следующее большее значение, которое может принять governor.
    int NextValue();
    /// Возвращает следующее меньшее значение, которое может принять governor.
    int PrevValue();
    /// Рассчитывате следующий кадр анимации.
    float Step();
    /// Изменяет значение в текущей позиции при раскрытом элементе.
    void ChangeValue(int delta);
    /// При открытом элементе переставляет курсор на следующую позицию.
    void NextPosition();
    /// При открытом элементе переставляет курсор не предыдущую позицию
    void PrevPosition();

    void Draw(int x, int y, bool opened);

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y);

    void DrawValue(int x, int y);

    char GetSymbol();

private:

    void DrawLowPart(int x, int y, bool shade);
    /// Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    uint  NumDigits();
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;
    int8                *cell;
    const char  * const *names;             ///< Варианты выбора на русском и английском языках.
    pFuncVB			    funcOnChanged;      ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII            funcForDraw;        ///< Функция вызывается после отрисовки элемента. 
};

class Choice : public Control
{
public:
    int8       *cell;
    pString    *names;          ///< Варианты выбора на русском и английском языках.
    pFuncVB     funcOnChanged;  ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII    funcForDraw;    ///< Функция вызывается после отрисовки элемента. 
    /// Запускает процесс изменения значения на delta
    void  StartChange(int delta);
    /// Рассчитывает следующий кадр анимации.
    float Step();
    /// Изменяет значение choice в зависимости от величины и знака delta.
    void  ChangeIndex(int delta);
    /// Возвращает количество вариантов выбора в элементе по адресу choice
    int   NumSubItems();

    bool ProcessKey(KeyEvent event);

    void  Draw(int x, int y, bool opened);

    void  DrawOpened(int x, int y);

    void  DrawClosed(int x, int y);
    /// Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы
    String NameCurrentSubItem() const;
    /// Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы
    const char *NameNextSubItem();

    const char *NamePrevSubItem();
    /// Возвращает имя варианта выбора элемента choice в позиции i как оно записано в исходном коде программы
    String NameSubItem(int i);
    /// Возвращает цвет, которым нужно заполнять участок выбора
    static Color ColorMenuField(const Choice *choice);

    char GetSymbol();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class ColorType;

class GovernorColorBase
{
public:
    COMMON_PART_MENU_ITEM;
    ColorType  *ct;                 ///< Структура для описания цвета.
    pFuncVV     funcOnChanged;      ///< Эту функцию нужно вызывать после изменения значения элемента.
};

class GovernorColor : public Control
{
public:
    ColorType  *ct;                 ///< Структура для описания цвета.
    pFuncVV     funcOnChanged;      ///< Эту функцию нужно вызывать после изменения значения элемента.
    void ChangeValue(int delta);    ///< Изменить яркость цвета в governor.
    void Draw(int x, int y, bool opened);
private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y, int delta);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Time ///

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

class TimeBase
{
public:
    COMMON_PART_MENU_ITEM;
    int8 *curField;   ///< Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
};

/// Устанавливает и показывает время.
class TimeControl : public Control
{
public:
    int8 *curField;   ///< Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
    void SetOpened();
    void IncCurrentPosition();
    void SetNewTime();
    void SelectNextPosition();
    void DecCurrentPosition();
    void Draw(int x, int y, bool opened);
    void DrawClosed(int x, int y);
    void DrawOpened(int x, int y);
    char GetSymbol();
};

#define CHOICE_RUN_FUNC_CHANGED(c, val)     \
    if(c->funcOnChanged)                    \
    {                                       \
        c->funcOnChanged(val);              \
    }


typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];

    
#include <Menu/MenuItemsAdd.h>

    
/** @}  @}
 */
