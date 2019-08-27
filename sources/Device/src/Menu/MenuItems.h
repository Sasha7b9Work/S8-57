#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


/*
    Структуры вида XxxDef испльзутся ТОЛЬКО для инициализации структур
*/

#define GRAPH_BUTTON_FROM_PAGE(page, numButton)     ((GraphButton *)((Page *)page)->items[numButton])

#define MENU_ITEMS_ON_DISPLAY       5   ///< Сколько пунктов меню помещается на экране по горизонтали.

#define IS_PAGE(item)           (item->type == Item::Type::Page)
#define NOT_PAGE(item)          (item->type != Item::Type::Page)
#define IS_PAGE_SB(item)        (item->isPageSB)
#define IS_CHOICE(item)         (item->type == Item::Type::Choice)
#define IS_CHOICE_REG(item)     (item->type == Item::Type::ChoiceReg)
#define NOT_CHOICE_REG(item)    (item->type != Item::Type::ChoiceReg)
#define IS_GOVERNOR(item)       (item->type == Item::Type::Governor)
#define NOT_GOVERNOR(item)      (item->type != Item::Type::Governor)
#define IS_GOVERNOR_COLOR(item) (item->type == Item::Type::GovernorColor)
#define IS_TIME(item)           (item->type == Item::Type::Time)
#define IS_BUTTON(item)         (item->type == Item::Type::Button)
#define IS_GRAPH_BUTTON(item)   (item->type == Item::Type::GraphButton)

class Page;
    
class Item
{
public:
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
            GraphButton,    ///< Кнопка для режима малых кнопок
            ChoiceParameter,
            Number
        } value;

        explicit Type(E v) : value(v) {};
    };
    
    uint8               type;            ///< Тип итема
    int8                num;             ///< Число вариантов для Choice или число контролов для Page
    uint8               name;            ///< Имя из перечисления Page::Name
    const Page *const  *keeper;          ///< Адрес страницы, которой принадлежит. Для Page_Main = 0
    pFuncBV             funcOfActive;    ///< Активен ли данный элемент
    const char * const *titleHint;       ///< Название страницы. Также подсказка для режима помощи
    Item(uint8 _type = Item::Type::None, const char * const *_titleHint = nullptr, const Page *const *_keeper = nullptr, int8 _num = 0, pFuncBV funcActive = nullptr) :
        type(_type), num(_num), keeper(_keeper), funcOfActive(funcActive), titleHint(_titleHint)
    {
    };
    /// Возвращает высоту в пикселях открытого элемента Choice или Page::Name
    int HeightOpened() const;
    /// Возвращает true, если кнопка, соответствующая элементу меню item, находится в нажатом положении
    bool IsPressed() const;
    /// Сделать/разделать текущим
    void SetCurrent(bool active) const;
    /// Возвращает true, если элемент меню по адрему item открыт
    bool IsOpened() const;

    void Open(bool open) const;
    /// Возвращает название элемента, как оно выглядит на дисплее прибора
    String Title() const;
    /// Обработка события кнопки
    bool ProcessKey(KeyEvent event);
    /// Вызывается при нажатии кнопки
    void KeyPress() const;
    /// Вызывается при "коротком" отпускании
    void KeyRelease() const;
    /// Вызывается при автоматическом срабатывании кнопки (нажатии и удержании более 0.5 сек)
    void KeyAutoRelease() const;
    /// Возвращает true, если контрол находится в активном состоянии (реагирует на органы управления)
    bool IsActive() const { if (funcOfActive) { return funcOfActive(); } return true; };

    void Draw(int x, int y, bool opened) const;

    bool IsCurrentItem() const;
    /// Возвращает адрес родителя
    const Page *Keeper() const { if (keeper) { return *keeper; }; return nullptr; }
    /// Возвращает true, если в древе предков стоит keeper
    bool ExistKeeper(const Page *keeper) const;
    /// Имеет родителя - не является главной страницей меню
    bool HaveParent() const { return Keeper() != nullptr; };
    /// Позиция итема по горизонтали - закрытого либо раскрытого.
    int PositionOnScreenX() const;
    /// Возвращает свою позицию в списке родителя
    int PositionInKeeperList() const;
    /// Высота итема
    int Height() const;
    /// Ширина итема. pos - позиция итема на экране.
    int Width(int pos = -1) const;
    /// Возвращает цвет фона заголовка итема
    Color ColorTitleBackground() const;
    /// Возвращает цвет, которым нужно рисовать на заголовке итема
    Color ColorTitleDraw() const;
    /// Цвет обводки итема
    Color ColorFrame() const;

    struct Value
    {
        static const int HEIGHT = 13;
    };

    static Item empty;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///

/// Описывает страницу меню
class Page : public Item
{
public:
    const Item * const *items;      ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                    ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB     funcOnEnterExit;    ///< Будет вызываться при нажатии на свёрнутую страницу и при выходе из этой страницы на предыдущую
    pFuncVV     funcOnDraw;         ///< Будет вызываться после отрисовки кнопок
    pFuncBKE    funcKey;            ///< В странице малых кнопок вызывается при нажатии стрелки
    Page(const char * const * titleHint, const Page * const *keeper, const Item * const *_items, int8 num, pFuncBV funcActive, pFuncVB funcEnterExit, pFuncVV funcDraw, pFuncBKE _funcKey) :
        Item(Item::Type::Page, titleHint, keeper, num, funcActive), items(_items), funcOnEnterExit(funcEnterExit), funcOnDraw(funcDraw), funcKey(_funcKey) {};
    /// Возвращает true, если текущий элемент страницы открыт
    bool CurrentItemIsOpened() const;
    /// Dозвращает число подстраниц в странице по адресу page
    int NumSubPages() const;        
    /// Возвращает количество элементов в странице по адресу page
    int NumItems() const;
    /// Установить текущей данную страницу.
    void SetAsCurrent() const;
    /// Возвращает номер текущей подстраницы (той, которая индицируется в данный момент)
    int8 CurrentSubPage() const;
    /// Установить текущей данную подстраницу
    void SetCurrentSubPage(int8 pos) const;
    ///< Устанавливает позицию активного пункта меню
    void SetPosActItem(int8 pos) const;
    ///< Возвращает указатель на заданный элемент страницы
    Item *GetItem(int numItem) const;
    /// \todo Возвращает позицию первого элемента страницы по адресу page на экране. Если текущая подстраница 0, это будет 0, если текущая 
    /// подстраница 1, это будет 5 и т.д.
    int PosItemOnLeft() const;
    /// Возвращает позицию текущего элемента странцы page
    int8 PosCurrentItem() const;
    /// Изменить номер текущей подстраницы на значение delta
    void ChangeSubPage(int delta);
    /// Нарисовать в заданных координатах
    void Draw(int x, int y, bool opened) const;
    /// Обработка события кнопки
    bool ProcessKey(KeyEvent event);

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y) const;

    void DrawPagesUGO(int right, int bottom) const;

    void DrawNestingPage(int left, int bottom) const;
    /// true, если является вложенной подстраницей страницы parent
    bool IsSubPage(const Page *parent);

    void ShortPress() const;
    /// Возвращает адрес элемента, соответствующего функциональной кнопкке
    const Item *ItemForFuncKey(Key::E key) const;

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
            Service_RTC,                ///< СЕРВИС - ВРЕМЯ
            Service_Battery,            ///< СЕРВИС - БАТАРЕЯ
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
            Debug_Multimeter,           ///< ОТЛАДКА - МУЛЬТИМЕТР
            SB_Debug_Settings,          ///< ОТЛАДКА - НАСТРОЙКИ
            SB_Debug_SerialNumber,      ///< ОТЛАДКА - С/Н
            Measures_Math,              ///< ИЗМЕРЕНИЯ - МАТЕМАТИКА
            Function_Multimeter_Cal,    ///< ФУНКЦИЯ - МУЛЬТИМЕТР - КАЛИБРОВКА
            Display_View,               ///< ДИСПЛЕЙ - ОТОБРАЖЕНИЕ
            Service_Info,               ///< СЕРВИС - ИНФОРМАЦИЯ
            Service_RTC_Set,            ///< СЕРВИС - ВРЕМЯ - УСТАНОВИТЬ
            Service_RTC_Correct,        ///< СЕРВИС - ВРЕМЯ - КОРРЕКЦИЯ
            Number,
            NoPage
        } value;
    };

    /// Возвращает имя страницы page
    Page::Name::E GetName() const;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
class Button : public Item
{
public:
    pFuncVV     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
    Button(const char * const * titleHint, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcPress, pFuncVII funcDraw) :
        Item(Item::Type::Button, titleHint, keeper, 0, funcActive),
        funcOnPress(funcPress), funcForDraw(funcDraw)
    {};
    void Draw(int x, int y) const;
    void KeyRelease() const;
    void KeyAutoRelease() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
struct StructHelpDrawButton
{
    pFuncVII    funcDrawUGO;    ///< Указатель на функцию отрисовки изображения варианта кнопки
    pString     helpUGO;        ///< Подпись к данному изображению.
};


/// Описывает кнопку для дополнительного режима меню.
class GraphButton : public Item
{
public:
    pFuncVV                     funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                    funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpDrawButton *hintUGO; 
    int                         numHints;
    GraphButton(const char * const * titleHint, const StructHelpDrawButton *_hintUGO, int num, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcPress, pFuncVII funcDraw) :
        Item(Item::Type::GraphButton, titleHint, keeper, 0, funcActive),
        funcOnPress(funcPress), funcForDraw(funcDraw), hintUGO(_hintUGO), numHints(num) {};

    void Draw(int x, int y) const;
    void DrawHints(int x, int y, int width) const;
    void KeyRelease() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///

/// Описывает регулятор.
class Governor : public Item
{
public:
    int16  *cell;
    int16   minValue;       ///< Минмальное значение, которое может принимать регулятор.
    int16   maxValue;       ///< Максимальное значение.
    pFuncVV funcOfChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой
    Governor(const char * const * titleHint, int16 *_cell, int16 min, int16 max, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcChanged, pFuncVV funcDraw) :
        Item(Item::Type::Governor, titleHint, keeper, 0, funcActive),
        cell(_cell), minValue(min), maxValue(max), funcOfChanged(funcChanged), funcBeforeDraw(funcDraw) {};
    /// Обработка события кнопки
    bool ProcessKey(KeyEvent event);
    /// Возвращает следующее большее значение, которое может принять governor.
    int16 NextValue();
    /// Возвращает следующее меньшее значение, которое может принять governor.
    int16 PrevValue();
    /// Рассчитывате следующий кадр анимации.
    float Step();
    /// Изменяет значение в текущей позиции при раскрытом элементе.
    void ChangeValue(int16 delta);
    /// При открытом элементе переставляет курсор на следующую позицию.
    void NextPosition() const;
    /// При открытом элементе переставляет курсор не предыдущую позицию
    void PrevPosition();

    void Draw(int x, int y, bool opened);

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y);

    void DrawValue(int x, int y);

    char GetSymbol();

    int16 GetValue();

    void SetValue(int16 v);

    void KeyRelease() const;

private:

    void DrawLowPart(int x, int y);
    /// Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    uint  NumDigits() const;

    static int8 currentDigit;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class Choice : public Item
{
public:
    int8       *cell;
    pString    *names;          ///< Варианты выбора.
    pFuncVB     funcOnChanged;  ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII    funcForDraw;    ///< Функция вызывается после отрисовки элемента. 
    Choice(const char * const * titleHint, pString *_names, int8 num, int8 *_cell, const Page * const *keeper, pFuncBV funcActive, pFuncVB funcChanged, pFuncVII funcDraw) :
        Item(Item::Type::Choice, titleHint, keeper, num, funcActive),
        cell(_cell), names(_names), funcOnChanged(funcChanged), funcForDraw(funcDraw) {};
    /// Запускает процесс изменения значения на delta
    void  StartChange(int delta) const;
    /// Рассчитывает следующий кадр анимации.
    float Step();
    /// Изменяет значение choice в зависимости от величины и знака delta.
    void  ChangeIndex(int delta) const;
    /// Возвращает количество вариантов выбора в элементе по адресу choice
    int   NumSubItems() const { return num; };

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
    /// Вызывает функцию funcOnChanged, если таковая имеется
    void Change(bool active) const { if (funcOnChanged) { funcOnChanged(active); } }

    void KeyRelease() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class GovernorColor : public Item
{
public:
    ColorType  *ct;                 ///< Структура для описания цвета.
    pFuncVV     funcOnChanged;      ///< Эту функцию нужно вызывать после изменения значения элемента.
    GovernorColor(const char * const * titleHint, ColorType *_ct, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcChanged) :
        Item(Item::Type::GovernorColor, titleHint, keeper, 0, funcActive),
        ct(_ct), funcOnChanged(funcChanged) {};
    void Draw(int x, int y, bool opened);
    void KeyRelease() const;
private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    static const int widthOpened = 129;
    static const int heightOpened = 27;
};

typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];
