#include "defines.h"
#include "Data/DataSettings.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


/// Структура используется при работе со страницей установки текущего времени
struct StructRTC
{
    int        curField;
    PackedTime time;
    StructRTC() : curField(0) { time = HAL_RTC::GetPackedTime(); };
    /// Изменить значение текущего поля на +/- 1
    void ChangeCurrentField(int delta)
    {
        typedef void (PackedTime::*PackedTimeFunc)(int);

        static const PackedTimeFunc funcs[] =
        {
            &PackedTime::ChangeHours, &PackedTime::ChangeMinutes, &PackedTime::ChangeSeconds,
            &PackedTime::ChangeDay,   &PackedTime::ChangeMonth,   &PackedTime::ChangeYear
        };

        PackedTimeFunc func = funcs[curField];

        (time.*func)(delta);
    };
};

/// Указатель на структуру с рабочей информацией. Память под структуру выделяется при открытии страницы и освобождается при закрытии.
/// Также нужно проверять значение этого указателя в функции отрисовки. Если оно равно nullptr, значит, страница была открыта нестандартным способом и нужно провести инициализацию.
static StructRTC *psRTC = nullptr;

#define CUR_FIELD (psRTC->curField)
#define TIME      (psRTC->time)



static void OnPress_SetLeft()
{
    Math::CircleDecrease(&CUR_FIELD, 0, 5);
}

static void Draw_Left(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bSet_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::self, Item::Active, OnPress_SetLeft, Draw_Left
)


static void OnPress_SetRight()
{
    Math::CircleIncrease(&CUR_FIELD, 0, 5);
}

static void Draw_Right(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bSet_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::self, Item::Active, OnPress_SetRight, Draw_Right
)


static void OnPress_SetUp()
{
    psRTC->ChangeCurrentField(1);
}

static void Draw_Up(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 4);
}

DEF_GRAPH_BUTTON( bSet_Up,
    "Больше",
    "Увеличить",
    &PageRTC::self, Item::Active, OnPress_SetUp, Draw_Up
)


static void OnPress_SetDown()
{
    psRTC->ChangeCurrentField(-1);
}

static void Draw_Down(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 4);
}

DEF_GRAPH_BUTTON( bSet_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::self, Item::Active, OnPress_SetDown, Draw_Down
)


static void OnPress_SetPick()
{
    HAL_RTC::SetPackedTime(psRTC->time);
    Menu::CloseOpenedItem();
}

static void Draw_Pick(int x, int y)
{
    Char(SymbolUGO2::SELECT).Draw4SymbolsInRect(x + 2 , y);
}

DEF_GRAPH_BUTTON(bSet_Pick,
    "Выбрать",
    "Выбор подсвеченного элемента",
    &PageRTC::self, Item::Active, OnPress_SetPick, Draw_Pick
)


static void DrawDigit(int x, int y, int digit)
{
    Integer(digit).ToString(false).Draw(x, y);
}

static void DrawField(int numField)
{
    int x0 = 30;
    int y0 = 20;

    int posX = (numField % 3);

    int x = x0 + posX * 93;
    int y = numField < 3 ? y0 : y0 + 100;

    uint fields[6] = { TIME.hours, TIME.minutes, TIME.seconds, TIME.day, TIME.month, TIME.year };

    Color::FILL.SetAsCurrent();

    if (numField == CUR_FIELD)
    {
        Region(72, 67).Fill(x - 2, y - 2, Color::FILL);
        Color::BACK.SetAsCurrent();
    }

    Integer value(static_cast<int>(fields[numField]));

    DrawDigit(x, y, value[1]);
    DrawDigit(x + 38, y, value[0]);

    const char separator[2] = { (numField < 3) ? ':' : '.', '\0' };

    if (posX < 2)
    {
        Text(separator).Draw(x + 76, (numField < 3) ? y - 14 : y, Color::FILL);
    }
}

static void DrawTime()
{
    uint time = TIME_MS;

    Font::SetCurrent(TypeFont::_Big64);
    int spacing = Font::GetSpacing();
    Font::SetSpacing(5);
    
    for (int i = 0; i < 6; i++)
    {
        DrawField(i);
    }
    
    Font::SetCurrent(TypeFont::_8);
    Font::SetSpacing(spacing);

    Integer(static_cast<int>(TIME_MS - time)).ToString(false).Draw(5, 5);
}

static void OnOpenClose_Set(bool open)
{
    if (open)
    {
        psRTC = new StructRTC();
    }
    else
    {
        delete psRTC;
    }
}

static void BeforeDraw_Set()
{
    if (psRTC == nullptr)                   // Если страница открыта непредусмотренным способом
    {
        OnOpenClose_Set(true);              // подведём подготовительные операции
    }

    Painter::BeginScene(Color::BACK);

    DrawTime();
}

static bool HandlerKey_Set(const KeyEvent &event)
{
    if (event.IsRelease())
    {
        if (event.key == Key::Left)
        {
            OnPress_SetLeft();
            return true;
        }
        else if (event.key == Key::Right)
        {
            OnPress_SetRight();
            return true;
        }
        else if (event.key == Key::Up)
        {
            OnPress_SetUp();
            return true;
        }
        else if (event.key == Key::Down)
        {
            OnPress_SetDown();
            return true;
        }
        else
        {
            // здесь ничего
        }
    }

    if (event.key == Key::Enter ||
        event.key == Key::F1 ||
        event.key == Key::F2 ||
        event.key == Key::F3 ||
        event.key == Key::F4 ||
        event.key == Key::F5)
    {
        return false;                   /// Здесь мы возвращаем признак того, что эти кнопки не обработаны - они будут обрабатываться обычным способом.
    }

    return true;                        /// А все остальные кнопки якобы обработаны - чтобы не происходило неправильных переключений на другие страницы
}

DEF_PAGE_5( pSet,
    "Время",
    "Установка текущего времени",
    &bSet_Left,
    &bSet_Right,
    &bSet_Up,
    &bSet_Down,
    &bSet_Pick,
    PageName::Service_RTC,
    &PageService::self, Item::Active, Page::NormalTitle, OnOpenClose_Set, BeforeDraw_Set, HandlerKey_Set
)

const Page * const PageRTC::self = static_cast<const Page *>(&pSet);
