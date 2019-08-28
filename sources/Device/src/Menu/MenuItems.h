#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


class Page;


struct DataItem
{
    uint8               type;           ///< Тип итема
    const char         *title;          ///< Заголовок итема
    const char         *hint;           ///< Подсказка для режима помощи
    const Page * const *keeper;         ///< Адрес страницы, которой принадлежит. Для Page_Main = 0
    pFuncBV             funcOfActive;   ///< Активен ли данный элемент
    const void         *ad;             ///< Указатель на структуру с данными, специфическими для каждого подкласса Item
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
    /// Вызывается при нажатии кнопки
    void KeyPress() const;
    /// Возвращает true, если контрол находится в активном состоянии (реагирует на органы управления)
    bool IsActive() const { if (data->funcOfActive) { return data->funcOfActive(); }; return true; };

    bool IsCurrentItem() const;
    /// Возвращает адрес родителя
    const Page *Keeper() const { if (data->keeper == nullptr) { return nullptr; } return *data->keeper; }
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

    bool Is(Type::E t) const { return data->type == t; };

    virtual void Draw(int /*x*/, int /*y*/, bool /*opened*/) const {};
    /// Вызывается при "коротком" отпускании
    virtual void KeyRelease() const;
    /// Вызывается при автоматическом срабатывании кнопки (нажатии и удержании более 0.5 сек)
    virtual void KeyAutoRelease() const;
    /// Обработка события кнопки
    virtual bool ProcessKey(KeyEvent) { return false; };
    /// Возвращает высоту в пикселях открытого элемента Choice или Page::Name
    virtual int HeightOpened() const;

    static Item empty;

    static DataItem emptyData;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
struct DataPage
{
    uint8               name;               ///< Имя из перечисления Page::Name
    const Item * const *items;              ///< Здесь указатели на пункты этой страницы
    pFuncVB             funcOnOpenClose;    ///< Будет вызываться при нажатии на свёрнутую страницу и при выходе из этой страницы на предыдущую
    pFuncVV             funcOnDraw;         ///< Будет вызываться после отрисовки кнопок
    pFuncBKE            funcOnKey;          ///< В странице малых кнопок вызывается при нажатии стрелки
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

    virtual void KeyRelease() const;

    virtual void KeyAutoRelease() const;
    /// Обработка события кнопки
    virtual bool ProcessKey(KeyEvent event);
    /// Нарисовать в заданных координатах
    virtual void Draw(int x, int y, bool opened) const;
    /// Возвращает указатель на данные, специфичные для этого класса
    DataPage *OwnData() const { return (DataPage *)data->ad; }

    void ShortPress() const;
    /// Возвращает адрес элемента, соответствующего функциональной кнопкке
    const Item *ItemForFuncKey(Key::E key) const;
    /// Возвращает имя страницы page
    PageName::E GetName() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
struct DataButton
{
    pFuncVV     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
    void FuncOnPress() { if (funcOnPress) { funcOnPress(); } }
    void FuncForDraw(int x, int y) { if (funcForDraw) { funcForDraw(x, y); } }
};

class Button : public Item
{
public:
    Button(const DataItem * const data) : Item(data) {};
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    DataButton *OwnData() const { return (DataButton *)data->ad; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GraphButton ///
struct StructHelpDrawButton
{
    pFuncVII    funcDrawUGO;    ///< Указатель на функцию отрисовки изображения варианта кнопки
    pString     helpUGO;        ///< Подпись к данному изображению.
};

struct DataGraphButton
{
    pFuncVV                     funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                    funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpDrawButton *hintUGO;

    void FuncOnPress() { if (funcOnPress) { funcOnPress(); } }
    void FuncForDraw(int x, int y) { if (funcForDraw) { funcForDraw(x, y); } }
};

/// Описывает кнопку для дополнительного режима меню.
class GraphButton : public Item
{
public:
    GraphButton(const DataItem * const data) : Item(data) {};

    virtual void Draw(int x, int y, bool opened) const;
    void DrawHints(int x, int y, int width) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    DataGraphButton *OwnData() const { return (DataGraphButton *)data->ad; }
    int NumHints() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
struct DataGovernor
{
    int16  *cell;
    int16   min;            ///< Минмальное значение, которое может принимать регулятор.
    int16   max;            ///< Максимальное значение.
    pFuncVV funcOnChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой

    void FuncOnChanged() { if (funcOnChanged) { funcOnChanged(); } }
    void FuncBeforeDraw() { if (funcBeforeDraw) { funcBeforeDraw(); } }
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

    void DrawOpened(int x, int y) const;

    void DrawClosed(int x, int y) const;

    void DrawValue(int x, int y) const;

    char GetSymbol() const;

    int16 GetValue() const;

    void SetValue(int16 v) const;

    virtual void KeyRelease() const;

    virtual void KeyAutoRelease() const;
    /// Обработка события кнопки
    virtual bool ProcessKey(KeyEvent event);

    virtual void Draw(int x, int y, bool opened) const;

    DataGovernor *OwnData() const { return (DataGovernor *)data->ad; }

private:

    void DrawLowPart(int x, int y) const;
    /// Возвращает число знакомест в поле для ввода элемента governor. Зависит от максимального значения, которое может принимать governor.
    uint  NumDigits() const;

    static int8 currentDigit;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct DataChoice
{
    int8       *cell;
    pString    *names;          ///< Варианты выбора.
    pFuncVB     funcOnChanged;  ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII    funcForDraw;    ///< Функция вызывается после отрисовки элемента. 

    void FuncOnChanged(bool active) { if (funcOnChanged) { funcOnChanged(active); } }
    void FuncForDraw(int x, int y) { if (funcForDraw) { funcForDraw(x, y); } }
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
    /// Возвращает цвет, которым нужно заполнять участок выбора
    static Color ColorMenuField(const Choice *choice);

    char GetSymbol();

    virtual void KeyRelease() const;

    virtual void KeyAutoRelease() const;

    virtual void Draw(int x, int y, bool opened) const;

    virtual bool ProcessKey(KeyEvent event);

    virtual int HeightOpened() const;

    DataChoice *OwnData() const { return (DataChoice *)data->ad; }
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
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    virtual int HeightOpened() const { return 27; };
    DataGovernorColor *OwnData() const { return (DataGovernorColor *)data->ad; }
private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;
    static const int widthOpened = 129;
    static const int heightOpened = 27;
};

typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];
