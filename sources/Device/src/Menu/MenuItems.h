#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


class Page;


struct DataItem
{
    uint8              type;            ///< Тип итема
    int8               num;             ///< Число вариантов для Choice или число контролов для Page
    uint8              name;            ///< Имя из перечисления Page::Name
    const Page *const *keeper;          ///< Адрес страницы, которой принадлежит. Для Page_Main = 0
    pFuncBV            funcOfActive;    ///< Активен ли данный элемент
    const pString     *titleHint;       ///< Название страницы. Также подсказка для режима помощи
    const void * const ad;              ///< Указатель на структуру с данными, специфическими для каждого подкласса Item
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
    const Item * const *items;      ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                    ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB     funcOnEnterExit;    ///< Будет вызываться при нажатии на свёрнутую страницу и при выходе из этой страницы на предыдущую
    pFuncVV     funcOnDraw;         ///< Будет вызываться после отрисовки кнопок
    pFuncBKE    funcOnKey;          ///< В странице малых кнопок вызывается при нажатии стрелки

    void FuncOnDraw() { if (funcOnDraw) { funcOnDraw(); } };
    void FuncOnEnterExit(bool enter) { if (funcOnEnterExit) { return funcOnEnterExit(enter); } };
    bool FuncOnKey(KeyEvent event) { if (funcOnKey) { return funcOnKey(event); }; return false; };
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
            Memory_Last,                ///< ПАМЯТЬ - ПОСЛЕДНИЕ
            Memory_Internal,            ///< ПАМЯТЬ - ВНУТР ЗУ
            Memory_Drive,               ///< ПАМЯТЬ - ВНЕШН ЗУ
            Memory_Drive_Manager,       ///< ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ
            Memory_Drive_Mask,          ///< ПАМЯТЬ - ВНЕШН ЗУ - МАСКА
            Memory_SetName,             ///< \brief Появляется перед сохранением файла на флешку для задания имени файла при соответствующей опции 
                                        ///< ВНЕШН ЗУ - Имя файла
            Service,                    ///< СЕРВИС
            Service_Calibrator,         ///< СЕРВИС - КАЛИБРАТОР
            Service_Ethernet,           ///< СЕРВИС - ETHERNET
            Service_RTC,                ///< СЕРВИС - ВРЕМЯ
            Service_Battery,            ///< СЕРВИС - БАТАРЕЯ
            Service_FFT_Cursors,        ///< СЕРВИС - СПЕКТР - КУРСОРЫ
            Service_Function,           ///< СЕРВИС - ФУНКЦИЯ
            Service_Information,        ///< СЕРВИС - ИНФОРМАЦИЯ
            Help,                       ///< ПОМОЩЬ
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
            Debug_Settings,             ///< ОТЛАДКА - НАСТРОЙКИ
            Debug_SerialNumber,         ///< ОТЛАДКА - С/Н
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
    Button(const DataItem * const head, pFuncVV funcPress, pFuncVII funcDraw);
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
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
    GraphButton(const DataItem * const head, const StructHelpDrawButton *hintUGO, pFuncVV funcPress, pFuncVII funcDraw);

    virtual void Draw(int x, int y, bool opened) const;
    void DrawHints(int x, int y, int width) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
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
    Governor(const DataItem * const head, int16 *cell, int16 min, int16 max, pFuncVV funcChanged, pFuncVV funcDraw);

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

private:

    void DrawLowPart(int x, int y) const;
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
    Choice(const DataItem * const data, pString *names, int8 *cell, pFuncVB funcChanged, pFuncVII funcDraw);
    /// Запускает процесс изменения значения на delta
    void  StartChange(int delta) const;
    /// Рассчитывает следующий кадр анимации.
    float Step() const;
    /// Изменяет значение choice в зависимости от величины и знака delta.
    void  ChangeIndex(int delta) const;
    /// Возвращает количество вариантов выбора в элементе по адресу choice
    int   NumSubItems() const { return data->num; };

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
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class GovernorColor : public Item
{
public:
    ColorType  *ct;                 ///< Структура для описания цвета.
    pFuncVV     funcOnChanged;      ///< Эту функцию нужно вызывать после изменения значения элемента.
    GovernorColor(const DataItem * const head, ColorType *ct, pFuncVV funcChanged);
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    virtual int HeightOpened() const { return 27; };
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
