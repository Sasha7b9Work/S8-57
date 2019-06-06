#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase ppFFT;
extern const PageBase pppFFT_Cursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cFFT_View,                                                                                                                             //--- ФУНКЦИЯ - СПЕКТР - Отображение ---
    "Отображение",
    "Включает и выключает отображение спектра",
    DISABLE_RU,
    ENABLE_RU,
    FFT_ENABLED, ppFFT, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cFFT_Scale,                                                                                                                                  //--- ФУНКЦИЯ - СПЕКТР - Шкала ---
    "Шкала",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Логарифм",
    "Линейная",
    SCALE_FFT, ppFFT, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3( cFFT_Source,                                                                                                                              //--- ФУНКЦИЯ - СПЕКТР - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    "Канал 1",
    "Канал 2",
    "Канал 1 + 2",
    SOURCE_FFT, ppFFT, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cFFT_Window,                                                                                                                                  //--- ФУНКЦИЯ - СПЕКТР - Окно ---
    "Окно",
    "Задаёт окно для расчёта спектра",
    "Прямоугольн",
    "Хэмминга",
    "Блэкмена",
    "Ханна",
    WINDOW_FFT, ppFFT, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_FFT_Range()
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3( cFFT_Range,                                                                                                                               //--- ФУНКЦИЯ - СПЕКТР - Диапазон ---
    "Диапазон",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "-40дБ",
    "-60дБ",
    "-80дБ",
    MAX_DB_FFT, ppFFT, IsActive_FFT_Range, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_FFT_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_FFT_Cursors_Source(int x, int y)
{
    String(MATH_CURRENT_CUR_IS_0 ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_SMALL_BUTTON( bFFT_Cursors_Source,                                                                                                        //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    pppFFT_Cursors, FuncActive, OnPress_FFT_Cursors_Source, Draw_FFT_Cursors_Source
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT_Cursors()
{
    return FFT_ENABLED;
}

static bool HandlerKey_FFT_Cursors(KeyEvent event)
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

DEF_PAGE_1( pppFFT_Cursors, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ ---
    "КУРСОРЫ",
    "Включает курсоры для измерения параметров спектра",
    &bFFT_Cursors_Source,                       ///< СЕРВИС - СПЕКТР - КУРСОРЫ - Источник
    Page::Name::SB_Service_FFT_Cursors, 
    &ppFFT,                                     ///< СЕРВИС - СПЕКТР
    IsActive_FFT_Cursors, FuncPressPage, FuncDrawPage, HandlerKey_FFT_Cursors
)



static bool IsActive_FFT()
{
    return !FUNC_MODE_DRAW_IS_ENABLED;
}

static void OnPress_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning(Warning::ImpossibleEnableFFT);
    }
}

const PageBase *PageFunction::PageFFT::pointer = &ppFFT;


DEF_PAGE_6( ppFFT, // -V641 // -V1027                                                                                                                               //--- СЕРВИС - СПЕКТР ---
    "СПЕКТР",
    "Отображение спектра входного сигнала",
    &cFFT_View,         ///< СЕРВИС - СПЕКТР - Отображение
    &cFFT_Scale,        ///< СЕРВИС - СПЕКТР - Шкала
    &cFFT_Source,       ///< СЕРВИС - СПЕКТР - Источник
    &cFFT_Window,       ///< СЕРВИС - СПЕКТР - Окно 
    &pppFFT_Cursors,    ///< СЕРВИС - СПЕКТР - КУРСОРЫ
    &cFFT_Range,        ///< СЕРВИС - СПЕКТР - Диапазон
    Page::Name::Function_FFT, PageFunction::pointer, IsActive_FFT, OnPress_FFT, FuncDrawPage, FuncRegSetPage
)
