#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cView,                                                                                                                                 //--- ФУНКЦИЯ - СПЕКТР - Отображение ---
    "Отображение",
    "Включает и выключает отображение спектра",
    DISABLE_RU,
    ENABLE_RU,
    FFT_ENABLED, &PageFFT::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Scale()
{
    return FFT_ENABLED;
}

DEF_CHOICE_2( cScale,                                                                                                                                      //--- ФУНКЦИЯ - СПЕКТР - Шкала ---
    "Шкала",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Логарифм",
    "Линейная",
    set.math.scaleFFT, &PageFFT::self, IsActive_Scale, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Source()
{
    return FFT_ENABLED;
}

DEF_CHOICE_3( cSource,                                                                                                                                  //--- ФУНКЦИЯ - СПЕКТР - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    "Канал 1",
    "Канал 2",
    "Канал 1 + 2",
    set.math.sourceFFT, &PageFFT::self, IsActive_Source, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Window()
{
    return FFT_ENABLED;
}

DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ФУНКЦИЯ - СПЕКТР - Окно ---
    "Окно",
    "Задаёт окно для расчёта спектра",
    "Прямоугольн",
    "Хэмминга",
    "Блэкмена",
    "Ханна",
    WINDOW_FFT, &PageFFT::self, IsActive_Window, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Range()
{
    return FFT_ENABLED && (set.math.scaleFFT == ScaleFFT::Log);
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ФУНКЦИЯ - СПЕКТР - Диапазон ---
    "Диапазон",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "-40дБ",
    "-60дБ",
    "-80дБ",
    MAX_DB_FFT, &PageFFT::self, IsActive_Range, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_Cursors_Source(int x, int y)
{
    String(MATH_CURRENT_CUR_IS_0 ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_GRAPH_BUTTON( bCursors_Source,                                                                                                            //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    &PageFFT::PageCursors::self, Item::Active, OnPress_Cursors_Source, Draw_Cursors_Source
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Cursors()
{
    return FFT_ENABLED;
}

static bool OnArrows_FFT_Cursors(const KeyEvent &event)
{
    if (event.type != TypePress::Press && event.type != TypePress::Repeat)
    {
        return true;
    }

    Key::E key = event.key;

    int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    FFT_POS_CURSOR(MATH_CURRENT_CUR) += (uint8)delta;
    Beeper::RegulatorShiftRotate();

    return true;
}

DEF_PAGE_1( pCursors, // -V641 // -V1027                                                                                                                 //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ ---
    "КУРСОРЫ",
    "Включает курсоры для измерения параметров спектра",
    &bCursors_Source,                       ///< СЕРВИС - СПЕКТР - КУРСОРЫ - Источник
    PageName::Function_FFT_Cursors, &PageFFT::self, IsActive_Cursors, Choice::Changed, Page::BeforeDraw, OnArrows_FFT_Cursors
)

const Page * const PageFFT::PageCursors::self = (const Page *)&pCursors;

static bool IsActive_FFT()
{
    return (set.math.modeDraw != FuncModeDraw::Disable);
}

static void OnOpenClose_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning(Warning::ImpossibleEnableFFT);
    }
}

DEF_PAGE_6( pFFT, // -V641 // -V1027                                                                                                                                //--- СЕРВИС - СПЕКТР ---
    "СПЕКТР",
    "Отображение спектра входного сигнала",
    &cView,         ///< СЕРВИС - СПЕКТР - Отображение
    &cScale,        ///< СЕРВИС - СПЕКТР - Шкала
    &cSource,       ///< СЕРВИС - СПЕКТР - Источник
    &cWindow,       ///< СЕРВИС - СПЕКТР - Окно 
    &pCursors,      ///< СЕРВИС - СПЕКТР - КУРСОРЫ
    &cRange,        ///< СЕРВИС - СПЕКТР - Диапазон
    PageName::Function_FFT, &PageFunction::self, IsActive_FFT, OnOpenClose_FFT, Page::BeforeDraw, E_BfKE
)

const Page * const PageFFT::self = (const Page *)&pFFT;
