#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


class Page;

typedef void (*pFuncDrawUGO)(int, int);
typedef void (*pFuncDraw)(int, int);
typedef void (*pFuncAfterDraw)(int, int);
typedef bool (*pFuncActive)();


struct DataItem
{
    uint8               type;       ///< Тип итема
    const char         *title;      ///< Заголовок итема
    const char         *hint;       ///< Подсказка для режима помощи
    const Page * const *keeper;     ///< Адрес страницы, которой принадлежит. Для Page_Main = 0
    pFuncActive         funcActive; ///< Указатель на функцию, которая определяет, активен ли данный итем
    const void         *ad;         ///< Указатель на структуру с данными, специфическими для каждого подкласса Item
};


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
            GovernorColor,  ///< Позволяет выбрать цвет.
            GraphButton,    ///< Кнопка для режима малых кнопок
            Number
        } value;

        explicit Type(E v) : value(v) {};
    };

    const DataItem *data;
    
    Item(const DataItem * const data = nullptr);
    /// Количество пунктов меню, умещающиееся на экране
    static const int NUM_ON_DISPLAY = 5;
    /// Возвращает true, если кнопка, соответствующая элементу меню item, находится в нажатом положении
    bool IsPressed() const;
    /// Сделать/разделать текущим
    void SetCurrent(bool active) const;
    /// Возвращает true, если элемент меню по адрему item открыт
    bool IsOpened() const;

    void Open(bool open) const;
    /// Возвращает название элемента, как оно выглядит на дисплее прибора
    String Title() const;
    /// Возвращает true, если контрол находится в активном состоянии (реагирует на органы управления)
    bool IsActive() const { return data->funcActive(); };

    bool IsCurrentItem() const;
    /// Возвращает адрес родителя
    const Page *Keeper() const { return (data->keeper) ? *data->keeper : nullptr; }
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

    void DrawCommonHiPart(int x, int y, bool opened) const;

    struct Value
    {
        static const int HEIGHT = 13;
    };

    bool Is(Type::E t) const { return data->type == static_cast<uint8>(t); };

    virtual void Draw(int /*x*/, int /*y*/, bool /*opened*/) const {};

    virtual void HandlerFX(TypePress::E type) const;
    /// Обработка события кнопки
    virtual bool HandlerKey(const KeyEvent &) { return false; };
    /// Возвращает высоту в пикселях открытого элемента Choice или Page::Name
    virtual int HeightOpened() const;

    static Item empty;

    static DataItem emptyData;

    static bool Active() { return true; }

    /// Возвращает цвет фона заголовка итема
    Color ColorTitleBackground() const { return IsPressed() ? Color::FILL : Color::BACK; };
    /// Возвращает цвет, которым нужно рисовать на заголовке итема
    Color ColorTitleText() const
    {
        if (!IsActive())  { return ColorBackground(this);  }

        return IsPressed() ? Color::BACK : Color::FILL;
    }
    /// Цвет обводки итема
    Color ColorFrame() const { return Color::FILL; };
    /// Возвращает цвет, которым нужно заполнять участок выбора
    Color ColorBackground(const Item *choice) const;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
struct DataPage
{
    uint8               name;               ///< Имя из перечисления Page::Name
    const Item * const *items;              ///< Здесь указатели на пункты этой страницы
    pFuncVB             funcOnOpenClose;    ///< Будет вызываться при открытии/закрытии страницы. При открытии в неё передаётся true, при закрытии - false
    pFuncVV             funcBeforeDraw;     ///< Будет вызываться после отрисовки страницы
    pFuncBKE            handlerKey;         ///< При нажатии кнопки в первую очередь вызывается эта функция (при открытой странице). Если она обработала событие клавиатуры, то возвращает true
};

/// Описывает страницу меню
class Page : public Item
{
public:
    Page(const DataItem * const data) : Item(data) {};
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

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y) const;

    void DrawPagesUGO(int right, int bottom) const;

    void DrawNestingPage(int left, int bottom) const;
    /// true, если является вложенной подстраницей страницы parent
    bool IsSubPage(const Page *parent);
    /// Обработка события кнопки
    virtual bool HandlerKey(const KeyEvent &event);
    /// Реакция на событие функциональной клавиши, соотвествующей итему
    virtual void HandlerFX(TypePress::E type) const;

    virtual void Draw(int x, int y, bool opened) const;

    const DataPage *OwnData() const { return static_cast<const DataPage *>(data->ad); }

    const Item *ItemForFuncKey(Key::E key) const;

    PageName::E GetName() const;

    static void BeforeDraw() {}

    static void OpenClose(bool) {}

    static bool HandlerKeyEvent(const KeyEvent &) { return false; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
struct DataButton
{
    pFuncVV     handlerPress;   ///< Функция, которая вызывается при нажатии на кнопку.
};

class Button : public Item
{
public:
    Button(const DataItem * const data) : Item(data) {};
    virtual void Draw(int x, int y, bool opened) const;
    virtual void HandlerFX(TypePress::E type) const;
    const DataButton *OwnData() const { return static_cast<const DataButton *>(data->ad); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GraphButton ///
struct StructHelpDrawButton
{
    pFuncDrawUGO    funcDrawUGO;    ///< Указатель на функцию отрисовки изображения варианта кнопки
    pString         helpUGO;        ///< Подпись к данному изображению.
};

struct DataGraphButton
{
    pFuncVV                     handlerPress;   ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncDraw                   funcDraw;       ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpDrawButton *hintUGO;
};

/// Описывает кнопку для дополнительного режима меню.
class GraphButton : public Item
{
public:
    GraphButton(const DataItem * const data) : Item(data) {};

    virtual void Draw(int x, int y, bool opened) const;
    void DrawHints(int x, int y, int width) const;
    virtual void HandlerFX(TypePress::E type) const;
    const DataGraphButton *OwnData() const { return static_cast<const DataGraphButton *>(data->ad); }
    int NumHints() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
struct DataGovernor
{
    int16  *cell;
    int16   min;            ///< Минмальное значение, которое может принимать регулятор.
    int16   max;            ///< Максимальное значение.
    pFuncVV handlerChange;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
};

/// Описывает регулятор.
class Governor : public Item
{
public:
    Governor(const DataItem * const data) : Item(data) {};
    /// Возвращает следующее большее значение, которое может принять governor.
    int16 NextValue() const;
    /// Возвращает следующее меньшее значение, которое может принять governor.
    int16 PrevValue() const;
    /// Рассчитывате следующий кадр анимации.
    float Step() const;
    /// Изменяет значение в текущей позиции при раскрытом элементе.
    void ChangeValue(int16 delta);
    /// При открытом элементе переставляет курсор на следующую позицию.
    void NextPosition() const;
    /// При открытом элементе переставляет курсор не предыдущую позицию
    void PrevPosition();
    /// Нарисовать в раскрытом виде
    void DrawOpened(int x, int y) const;
    /// Нарисовать в закрытом виде (в строке меню)
    void DrawClosed(int x, int y) const;
    /// Отобразить значение
    void DrawValue(int x, int y) const;
    /// Возвращает символ ручки, соответствующий текущему значению
    char GetSymbol() const;
    /// Возвращает значение, установленное в регуляторе
    int16 GetValue() const;
    /// Задаёт новое значение
    void SetValue(int16 v) const;
    /// Обработка события кнопки
    virtual bool HandlerKey(const KeyEvent &event);
    /// Обработка события функциональной кнопки, соответствующей данному итему
    virtual void HandlerFX(TypePress::E type) const;

    virtual void Draw(int x, int y, bool opened) const;

    const DataGovernor *OwnData() const { return static_cast<const DataGovernor *>(data->ad); }

private:

    void DrawLowPart(int x, int y) const;
    /// Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    uint  NumDigits() const;
    /// Нарисовать значение регулятора в режиме поразрядной регулировки. setPosFromEnd - подсвеченный (активный) разряд, начиная с конца. Если selPosFromEnd == -1, подсвечивать не нужно
    void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool fillNull) const;

    static int8 currentDigit;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct DataChoice
{
    int8           *cell;
    pString        *names;          ///< Варианты выбора.
    pFuncVB         handlerChange;  ///< Функция должна вызываться после изменения значения элемента.
    pFuncAfterDraw  funcAfterDraw;  ///< Функция вызывается после отрисовки элемента. 
};

class Choice : public Item
{
public:
    Choice(const DataItem * const data) : Item(data) {};
    /// Запускает процесс изменения значения на delta
    void  StartChange(int delta) const;
    /// Рассчитывает следующий кадр анимации.
    float Step() const;
    /// Изменяет значение choice в зависимости от величины и знака delta.
    void  ChangeIndex(int delta) const;
    /// Возвращает количество вариантов выбора в элементе по адресу choice
    int   NumChoices() const;

    void  DrawOpened(int x, int y) const;

    void  DrawClosed(int x, int y) const;
    /// Возвращает имя текущего варианта выбора элемента choice, как оно записано в исходном коде программы
    String NameCurrentSubItem() const;
    /// Возвращает имя следующего варианта выбора элемента choice, как оно записано в исходном коде программы
    const char *NameNextSubItem() const;

    const char *NamePrevSubItem() const;
    /// Возвращает имя варианта выбора элемента choice в позиции i как оно записано в исходном коде программы
    String NameSubItem(int i) const;

    char GetSymbol();

    virtual void Draw(int x, int y, bool opened) const;
    /// Обработка события кнопки
    virtual bool HandlerKey(const KeyEvent &event);
    /// Обработка события функциональной кнопки, соответствующей данному итему
    virtual void HandlerFX(TypePress::E type) const;

    virtual int HeightOpened() const;

    const DataChoice *OwnData() const { return static_cast<const DataChoice *>(data->ad); }

    static void Changed(bool) {};

    static void AfterDraw(int, int) {};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
struct DataGovernorColor
{
    ColorType  *ct;                 ///< Структура для описания цвета.
};

class GovernorColor : public Item
{
public:
    GovernorColor(const DataItem * const data) : Item(data) {};
    virtual void Draw(int x, int y, bool opened) const;
    /// Обработка события функциональной кнопки, соответствующей данному итему на странице
    virtual void HandlerFX(TypePress::E type) const;
    virtual int HeightOpened() const { return 27; };
    const DataGovernorColor *OwnData() const { return static_cast<const DataGovernorColor *>(data->ad); }
private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;
    static const int widthOpened = 129;
    static const int heightOpened = 27;
};
