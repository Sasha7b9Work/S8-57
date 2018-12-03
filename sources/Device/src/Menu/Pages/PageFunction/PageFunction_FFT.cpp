#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Include/PageFunction.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase ppFFT;
extern const PageBase pppFFT_Cursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cFFT_View,                                                                                       //--- ФУНКЦИЯ - СПЕКТР - Отображение ---
    "Отображение", "Display",
    "Включает и выключает отображение спектра",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FFT_ENABLED, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cFFT_Scale,                                                                                            //--- ФУНКЦИЯ - СПЕКТР - Шкала ---
    "Шкала", "Scale",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "Логарифм", "Log",
    "Линейная", "Linear",
    SCALE_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cFFT_Source,                                                                                        //--- ФУНКЦИЯ - СПЕКТР - Источник ---
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Selecting the source for the calculation of the spectrum",
    "Канал 1", "Chan 1",
    "Канал 2", "Chan 2",
    "Канал 1 + 2", "Chan 1 + 2",
    SOURCE_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(cFFT_Window,                                                                                            //--- ФУНКЦИЯ - СПЕКТР - Окно ---
    "Окно", "Window",
    "Задаёт окно для расчёта спектра",
    "Sets the window to calculate the spectrum",
    "Прямоугольн", "Rectangle",
    "Хэмминга", "Hamming",
    "Блэкмена", "Blackman",
    "Ханна", "Hann",
    WINDOW_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_FFT_Range()
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3(cFFT_Range,                                                                                         //--- ФУНКЦИЯ - СПЕКТР - Диапазон ---
    "Диапазон", "Range",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "Here you can set the limit of monitoring the power spectrum",
    "-40дБ", "-40dB",
    "-60дБ", "-60dB",
    "-80дБ", "-80dB",
    MAX_DB_FFT, ppFFT, IsActive_FFT_Range, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
DEF_SMALL_BUTTON_EXIT(bFFT_Cursors_Exit,                                                                  //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ - Выход ---
    pppFFT_Cursors, FuncActive, OnPressSB_Exit, DrawSB_Exit
)
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_FFT_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_FFT_Cursors_Source(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, MATH_CURRENT_CUR_IS_0 ? "1" : "2");
}

DEF_SMALL_BUTTON(bFFT_Cursors_Source,                                                                  //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ - Источник ---
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Source choice for calculation of a range",
    pppFFT_Cursors, FuncActive, OnPress_FFT_Cursors_Source, Draw_FFT_Cursors_Source
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT_Cursors()
{
    return FFT_ENABLED;
}

static bool HandlerKey_FFT_Cursors(KeyEvent event)
{
    Key::E key = event.key;

    int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    FFT_POS_CURSOR(MATH_CURRENT_CUR) += (uint8)delta;
    Sound::RegulatorShiftRotate();

    return true;
}

/*
DEF_PAGE_SB(pppFFT_Cursors,                                                                                       //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ ---
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    &bFFT_Cursors_Exit,     ///< СЕРВИС - СПЕКТР - КУРСОРЫ - Выход
    &bFFT_Cursors_Source,   ///< СЕРВИС - СПЕКТР - КУРСОРЫ - Источник
    0,
    0,
    0,
    0,
    Page::Name::SB_Service_FFT_Cursors, &ppFFT, IsActive_FFT_Cursors, EmptyPressPage, FuncDrawPage, OnRegSet_FFT_Cursors
)
*/

DEF_PAGE_1(pppFFT_Cursors,                                                                                                                               //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ ---
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    &bFFT_Cursors_Source,   ///< СЕРВИС - СПЕКТР - КУРСОРЫ - Источник
    Page::Name::SB_Service_FFT_Cursors, &ppFFT, IsActive_FFT_Cursors, EmptyPressPage, FuncDrawPage, HandlerKey_FFT_Cursors
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


DEF_PAGE_6(ppFFT,                                                                                                            //--- СЕРВИС - СПЕКТР ---
    "СПЕКТР", "SPECTRUM",
    "Отображение спектра входного сигнала",
    "Mapping the input signal spectrum",
    &cFFT_View,         ///< СЕРВИС - СПЕКТР - Отображение
    &cFFT_Scale,        ///< СЕРВИС - СПЕКТР - Шкала
    &cFFT_Source,       ///< СЕРВИС - СПЕКТР - Источник
    &cFFT_Window,       ///< СЕРВИС - СПЕКТР - Окно 
    &pppFFT_Cursors,    ///< СЕРВИС - СПЕКТР - КУРСОРЫ
    &cFFT_Range,        ///< СЕРВИС - СПЕКТР - Диапазон
    Page::Name::Function_FFT, PageFunction::pointer, IsActive_FFT, OnPress_FFT, FuncDrawPage, FuncRegSetPage
)
