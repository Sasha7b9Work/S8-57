#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include "Display/Font/Font.h"
#include "Display/Display.h"
#include "Display/Symbols.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Utils/Values.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include "Data/DataSettings.h"


using namespace Display::Primitives;

/// Структура используется при работе со страницей установки текущего времени
struct StructRTC
{
    int        curField;
    PackedTime time;
    StructRTC() : curField(0) { time = HAL::RTC_::GetPackedTime(); };
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pRTC, // -V641 // -V1027                                                                                                                                 //--- СЕРВИС - ВРЕМЯ ---
    "ВРЕМЯ",
    "Установка и настройка времени",
    PageRTC::PageSet::self,         ///< СЕРВИС - ВРЕМЯ - Время
    PageRTC::PageCorrect::self,     ///< CЕРВИС - ВРЕМЯ - Коррекция
    PageName::Service_RTC,
    &PageService::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::self = (const Page *)&pRTC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_SetLeft()
{
    Math::CircleDecrease(&CUR_FIELD, 0, 5);
}

static void Draw_Left(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetLeft, Draw_Left
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetRight()
{
    Math::CircleIncrease(&CUR_FIELD, 0, 5);
}

static void Draw_Right(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetRight, Draw_Right
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetUp()
{
    psRTC->ChangeCurrentField(1);
}

static void Draw_Up(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_UP).Draw4SymbolsInRect(x + 2, y + 4);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Up,
    "Больше",
    "Увеличить",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetUp, Draw_Up
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetDown()
{
    psRTC->ChangeCurrentField(-1);
}

static void Draw_Down(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TRIANGLE_DOWN).Draw4SymbolsInRect(x + 2, y + 4);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSet_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetDown, Draw_Down
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetPick()
{
    HAL::RTC_::SetPackedTime(psRTC->time);
    Menu::CloseOpenedItem();
}

static void Draw_Pick(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SELECT).Draw4SymbolsInRect(x + 2 , y);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON(bSet_Pick,
    "Выбрать",
    "Выбор подсвеченного элемента",
    &PageRTC::PageSet::self, E_BtV, OnPress_SetPick, Draw_Pick
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    Integer value((int)fields[numField]);

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

    Font::SetCurrent(Font::Type::_Big64);
    int spacing = Font::GetSpacing();
    Font::SetSpacing(5);
    
    for (int i = 0; i < 6; i++)
    {
        DrawField(i);
    }
    
    Font::SetCurrent(Font::Type::_8);
    Font::SetSpacing(spacing);

    Integer((int)(TIME_MS - time)).ToString(false).Draw(5, 5);
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

static bool OnKey_Set(const KeyEvent &event)
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
    }

    return true;
}

DEF_PAGE_5( pSet, //-V641 //-V1027
    "УСТАНОВКА",
    "Установка текущего времени",
    &bSet_Left,
    &bSet_Right,
    &bSet_Up,
    &bSet_Down,
    &bSet_Pick,
    PageName::Service_RTC_Set,
    &PageRTC::self, E_BtV, OnOpenClose_Set, BeforeDraw_Set, OnKey_Set
)

const Page * const PageRTC::PageSet::self = (const Page *)&pSet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_CorrectionLeft()
{
}

DEF_GRAPH_BUTTON( bCorrection_Left,
    "Влево",
    "Предыдущий элемент",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionLeft, Draw_Left
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionRight()
{
}

DEF_GRAPH_BUTTON( bCorrection_Right,
    "Вправо",
    "Следующий элемент",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionRight, Draw_Right
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionUp()
{
}

DEF_GRAPH_BUTTON( bCorrection_Up,
    "Больше",
    "Увеличить",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionUp, Draw_Up
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionDown()
{
}

DEF_GRAPH_BUTTON( bCorrection_Down,
    "Меньше",
    "Уменьшить",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionDown, Draw_Down
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CorrectionPick()
{
}

DEF_GRAPH_BUTTON( bCorrection_Pick,
    "Выбор",
    "Активировать подсвеченный элемент",
    &PageRTC::PageCorrect::self, E_BtV, OnPress_CorrectionPick, Draw_Pick
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pCorrection, //-V641 //-V1027
    "КОРРЕКЦИЯ",
    "Коррекция скорости хода часов",
    &bCorrection_Left,
    &bCorrection_Right,
    &bCorrection_Up,
    &bCorrection_Down,
    &bCorrection_Pick,
    PageName::Service_RTC_Correct,
    &PageRTC::self, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageRTC::PageCorrect::self = (const Page *)&pCorrection;
