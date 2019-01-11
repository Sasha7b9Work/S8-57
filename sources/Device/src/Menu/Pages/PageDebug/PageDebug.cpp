#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/EEPROM.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase mainPage;
extern const PageBase pageDebug;
extern const PageBase ppADC;
extern const PageBase pppADC_Balance;
extern const PageBase pppADC_Stretch;
extern const PageBase pppADC_Shift;
extern const PageBase ppChannels;
extern const PageBase ppSettings;
extern const PageBase ppSerialNumber;

const PageBase *PageDebug::pointer = &pageDebug;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этой структуре будут храниться данные серийного номера при открытой странице ppSerialNumer
typedef struct
{
    int number;     ///< Соответственно, порядковый номер.
    int year;       ///< Соответственно, год.
    int curDigt;    ///< Соответственно, номером (0) или годом (1) управляет ручка УСТАНОВКА.
} StructForSN;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, (int8)NRST_BALANCE_ADC_A},
        {0, SET_BALANCE_ADC_B, (int8)NRST_BALANCE_ADC_B}
    };

    shiftADCA = shift[Chan::A][NRST_BALANCE_ADC_TYPE];
    shiftADCB = shift[Chan::B][NRST_BALANCE_ADC_TYPE];
}

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);
}

DEF_CHOICE_3(       cADC_Balance_Mode,                                                                        //--- ОТЛАДКА - АЦП - БАЛАНС - Режим ---
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной",   "Manual",
    NRST_BALANCE_ADC_TYPE, pppADC_Balance, FuncActive, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_Balance_ShiftAB()
{
    return NRST_BALANCE_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Balance_ShiftA()
{
    NRST_BALANCE_ADC_A = shiftADCA;
}

DEF_GOVERNOR( gADC_Balance_ShiftA,                                                                                                              //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---
    "Смещение 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Balance_ShiftB()
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR( gADC_Balance_ShiftB,                                                                                                              //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 2 ---
    "Смещение 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pppADC_Balance, // -V641                                                                                                                         //--- ОТЛАДКА - АЦП - БАЛАНС ---
    "БАЛАНС", "BALANCE",
    "",
    "",
    &cADC_Balance_Mode,      // ОТЛАДКА - АЦП - БАЛАНС - Режим
    &gADC_Balance_ShiftA,    // ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
    &gADC_Balance_ShiftB,    // ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
    Page::Name::Debug_ADC_Balance, &ppADC, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 stretchA;
static int16 stretchB;

void PageDebug::OnChanged_ADC_Stretch_Mode(bool)
{
    if (NRST_STRETCH_ADC_TYPE_IS_DISABLE)
    {
        stretchA = NRST_STRETCH_ADC_A(StretchADC::Disable) = 0;
        stretchB = NRST_STRETCH_ADC_B(StretchADC::Disable) = 0;
    }
    else
    {
        stretchA = NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE);
        stretchB = NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE);
    }
}

DEF_CHOICE_3(       cADC_Stretch_Mode,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ---
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "Реальный", "Real",
    "Ручной",   "Manual",
    NRST_STRETCH_ADC_TYPE, pppADC_Stretch, FuncActive, PageDebug::OnChanged_ADC_Stretch_Mode, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_StretchAB()
{
    return NRST_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_A()
{
    NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE) = stretchA;
}

DEF_GOVERNOR(       gADC_Stretch_A,                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к ---
    "Растяжка 1к", "Stretch 1ch",
    "Задаёт ручную растяжку первого канала.\n1 единица = 0.0001",
    "Sets the manual stretching of the first channel.\n1 = 0.0001",
    stretchA, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Stretch_B()
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR(       gADC_Stretch_B,                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к ---
    "Растяжка 2к", "Stretch 2ch",
    "Задаёт ручную растяжку второго канала.\n1 единица = 0.0001",
    "Sets the manual stretching of the second channel.\n1 = 0.0001",
    stretchB, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_B, FuncBeforeDraw
)

/*
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak20mV,                                                               //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 1к ---
    "20мВ/1В 1к", "20mV/1V 1k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak50mV,                                                                  //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 50мВ 1к ---
    "50мВ 1к", "50mV 1k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak100mV,                                                             //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 1к ---
    "100мВ/5В 1к", "100mV/5V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_100mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak2V,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 1к ---
    "2В 1к", "2V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk20mV,                                                               //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 2к ---
    "20мВ/1В 2к", "20mV/1V 2k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk50mV,                                                         //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Доп смещ 50мВ 2к ---
    "50мВ 2к", "50mV 2k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk100mV,                                                             //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 2к ---
    "100мВ/5В 2к", "100mV/5V 2k",
    "",
    "",
    NRST_ADD_STRETCH_100mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk2V,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 2к ---
    "2В 2к", "2V 2ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static const ChoiceBase emptyChoice = {Control::Type::Choice, 0, false, Page::Name::NoPage, 0, FuncActive, {}, 0, 0, 0, 0};

DEF_PAGE_3( pppADC_Stretch, // -V641                                                                                                                       //--- ОТЛАДКА - АЦП - РАСТЯЖКА ---
    "РАСТЯЖКА", "STRETCH",
    "Устанавливает режим и величину растяжки (для ручного режима)",
    "Sets mode and the value of stretching (manual mode)",
    &cADC_Stretch_Mode,      // ОТЛАДКА - АЦП - РАСТЯЖКА - Режим
    &gADC_Stretch_A,         // ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к
    &gADC_Stretch_B,
    Page::Name::Debug_ADC_Stretch, &ppADC, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ADC_Shift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < 3; range++)
        {
            RSHIFT_ADD_STABLE(ch, range) = 0;
        }
    }
    FPGA::SetRShift(Chan::A, SET_RSHIFT_A);
    FPGA::SetRShift(Chan::B, SET_RSHIFT_B);
}

DEF_BUTTON( bADC_Shift_Reset,                                                                                                                       //-- ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс ---
    "Сброс", "Reset",
    "",
    "",
    pppADC_Shift, FuncActive, OnPress_ADC_Shift_Reset, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_A()
{
    FPGA::SetRShift(Chan::A, SET_RSHIFT_A);
}

DEF_GOVERNOR( gADC_Shift_A2mV,                                                                                                            //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост ---
    "См 1к 2мВ пост", "Shift 1ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range::_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_B()
{
    FPGA::SetRShift(Chan::B, SET_RSHIFT_B);
}

DEF_GOVERNOR( gADC_Shift_B2mV,                                                                                                            //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост ---
    "См 2к 2мВ пост", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range::_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_A5mV,                                                                                                            //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост ---
    "См 1к 5мВ пост", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range::_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_B5mV,                                                                                                            //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост ---
    "См 2к 5мВ пост", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range::_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_A10mV,                                                                                                          //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост ---
    "См 1к 10мВ пост", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range::_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_B10mV,                                                                                                          //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост ---
    "См 2к 10мВ пост", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range::_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7( pppADC_Shift, // -V641  // -V1027                                                                                                              //--- ОТЛАДКА - АЦП - ДОП СМЕЩ ---
    "ДОП СМЕЩ", "ADD RSHFIT",
    "",
    "",
    &bADC_Shift_Reset,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс
    &gADC_Shift_A2mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост
    &gADC_Shift_B2mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост
    &gADC_Shift_A5mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост
    &gADC_Shift_B5mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост
    &gADC_Shift_A10mV,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост
    &gADC_Shift_B10mV,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост
    Page::Name::Debug_ADC_Shift, &ppADC, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( ppADC, // -V641                                                                                                                                           //--- ОТЛАДКА - АЦП ---
    "АЦП", "ADC",
    "",
    "",
    &pppADC_Balance, // ОТЛАДКА - АЦП - БАЛАНС
    &pppADC_Stretch, // ОТЛАДКА - АЦП - РАСТЯЖКА
    &pppADC_Shift,   // ОТЛАДКА - АЦП - ДОП СМЕЩ
    Page::Name::Debug_ADC, &pageDebug, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Channels_BandwidthA(bool)
{
    FPGA::SetBandwidth(Chan::A);
}

DEF_CHOICE_7( cChannels_BandwidthA,                                                                                                                     //--- ОТЛАДКА - КАНЛАЫ - Полоса 1 ---
    "Полоса 1", "Bandwidth 1",
    "Здесь можно выбрать полосу, которая будет действовать в КАНАЛ1-Полоса при выборе значения Полная",
    "Here you can select the bandwidth, which will operate in CHANNEL1-Bandwidth when set to Full",
    "Полная", "Full",
    "20МГц", "20MHz",
    "100МГц", "100MHz",
    "200МГц", "200MHz",
    "350МГц", "350MHz",
    "650МГц", "650MHz",
    "750МГц", "750MHz",
    BANDWIDTH_DEBUG(Chan::A), ppChannels, FuncActive, OnChanged_Channels_BandwidthA, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Channels_BandwidthB(bool)
{
    FPGA::SetBandwidth(Chan::B);
}

DEF_CHOICE_7(       cChannels_BandwidthB,                                                                        //--- ОТЛАДКА - КАНЛАЫ - Полоса 2 ---
    "Полоса 2", "Bandwidth 2",
    "Здесь можно выбрать полосу, которая будет действовать в КАНАЛ2-Полоса при выборе значения Полная",
    "Here you can select the bandwidth, which will operate in CHANNEL2-Bandwidth when set to Full",
    "Полная", "Full",
    "20МГц",  "20MHz",
    "100МГц", "100MHz",
    "200МГц", "200MHz",
    "350МГц", "350MHz",
    "650МГц", "650MHz",
    "750МГц", "750MHz",
    BANDWIDTH_DEBUG(Chan::B), ppChannels, FuncActive, OnChanged_Channels_BandwidthB, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( ppChannels, // -V641                                                                                                                                   //--- ОТЛАДКА - КАНАЛЫ ---
    "КАНАЛЫ", "CHANNELS",
    "",
    "",
    &cChannels_BandwidthA,   // ОТЛАДКА - КАНАЛЫ - Полоса 1
    &cChannels_BandwidthB,   // ОТЛАДКА - КАНАЛЫ - Полоса 2
    Page::Name::Debug_Channels, &pageDebug, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cStats,                                                                                                                                    //--- ОТЛАДКА - Статистика ---
    "Статистика", "Statistics",
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в памяти/количество сохраняемых в памяти "
    "сигналов",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "Не показывать", "Hide",
    "Показывать",    "Show",
    SHOW_STAT, pageDebug, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDebug::OnChanged_DisplayOrientation(bool)
{
    Display::SetOrientation(DISPLAY_ORIENTATION);
}

DEF_CHOICE_2(       cDisplayOrientation,                                                                                                                       //--- ОТЛАДКА - Ориентация ---
    "Ориентация", "DisplayOrientation",
    "Устанавливает ориентацию дисплея",
    "Sets display orientation",
    "Прямая",   "Direct",
    "Обратная", "Back",
    DISPLAY_ORIENTATION, pageDebug, FuncActive, PageDebug::OnChanged_DisplayOrientation, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 pred;

static void OnChanged_Pred()
{
    FPGA::pred = (uint16)(~pred);
}

DEF_GOVERNOR(       mgPred,                                                                                             //--- ОТЛАДКА - Предзапуск ---
    "Предзапуск", "",
    "", "",
    pred, 0, 15000, pageDebug, FuncActive, OnChanged_Pred, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 post;

static void OnChanged_Post()
{
    FPGA::post = (uint16)~post;
}

DEF_GOVERNOR(       mgPost,                                                                                            //--- ОТЛАДКА - Послезапуск ---
    "Послезапуск", "",
    "", "",
    post, 0, 15000, pageDebug, FuncActive, OnChanged_Post, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Settings_Exit()
{
    Display::SetDrawMode(Display::DrawMode::Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(  bSettings_Exit,                                                                          //--- ОТЛАДКА - НАСТРОЙКИ - Выход ---
    ppSettings, FuncActive, OnPress_Settings_Exit, DrawSB_Exit
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DebugShowSetInfo_Draw()
{
    Painter::BeginScene(Color::BACK);
    Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);

    int x0 = 30;
    int y0 = 25;
    int dY = 10;
    int y = y0 - dY;

#define Y_AND_INCREASE (y += dY, y)
#define DRAW_TEXT(str)                  Painter::DrawText(x0, Y_AND_INCREASE, str)
#define DRAW_FORMAT(str, value)         String(str, value).Draw(x0, Y_AND_INCREASE)
#define DRAW_FORMAT2(str, val1, val2)   String(str, val1, val2).Draw(x0, Y_AND_INCREASE)

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "Размер основной структуры %d", sizeof(set));
    DRAW_FORMAT("Размер основной структуры : %d", sizeof(set));
    String("Несбрасываемая структура:").Draw(x0, Y_AND_INCREASE);

    int x = String("rShiftAdd :").Draw(x0, Y_AND_INCREASE) + 5;

    int ddY = 0;

    for (int type = 0; type < 2; type++)
    {
        for (int ch = 0; ch < 2; ch++)
        {
            for (int range = 0; range < Range::Number; ++range)
            {
                String("%d", NRST_RSHIFT_ADD(ch, range, type)).Draw(x + range * 20, y + dY * ddY);
            }
            ddY++;
        }
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", NRST_CORRECTION_TIME);
    DRAW_FORMAT2("balanceADC : %d %d", NRST_BALANCE_ADC_A, NRST_BALANCE_ADC_B);
    DRAW_FORMAT("numAveForRand : %d", NRST_NUM_AVE_FOR_RAND);

    pString s[3] = {"выключено", "настроено автоматически", "задано вручную"};
    DRAW_FORMAT("balanceADCtype : %s", (NRST_BALANCE_ADC_TYPE < 3 ? s[NRST_BALANCE_ADC_TYPE] : "!!! неправильное значение !!!"));
    DRAW_FORMAT("stretchADCtype : %s", (NRST_STRETCH_ADC_TYPE < 3 ? s[NRST_STRETCH_ADC_TYPE] : "!!! неправильное значение !!!"));

    x = String("stretchADC :").Draw(x0, Y_AND_INCREASE) + 5;

    for (int ch = 0; ch < 2; ch++)
    {
        for (int num = 0; num < 3; num++)
        {
            String("%d", NRST_STRETCH_ADC(ch, num)).Draw(x + num * 20, y + dY * ch);
        }
    }

    y += dY;

#define DRAW_STRETCH(name) DRAW_FORMAT2(#name " : %d %d", set.nrst_##name[0], set.nrst_##name[1])

    /*
    DRAW_STRETCH(AddStretch20mV);
    DRAW_STRETCH(AddStretch50mV);
    DRAW_STRETCH(AddStretch100mV);
    DRAW_STRETCH(AddStretch2V);
    */

    DRAW_FORMAT("numSmoothForRand : %d", NRST_NUM_SMOOTH_FOR_RAND);

    Menu::Draw();
    Painter::EndScene();
}

static void OnPress_Settings(bool)
{
    Display::SetDrawMode(Display::DrawMode::Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_SB( ppSettings, // -V641 // -V1027                                                                                                                     //--- ОТЛАДКА - НАСТРОЙКИ ---
    "НАСТРОЙКИ", "SETTINGS",
    "Показать информацию о настройках",
    "Show settings information",
    &bSettings_Exit,            // ОТЛАДКА - НАСТРОЙКИ - Выход
    0,
    0,
    0,
    0,
    0,
    Page::Name::SB_Debug_Settings, &pageDebug, FuncActive, OnPress_Settings, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    Display::FuncOnWaitStart(DICT(DSaveFirmware), false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-54.bin", &structForWrite);

    uint8 *address = (uint8 *)0x08020000;   // -V566
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStop();

    Display::ShowWarning(Warning::FirmwareSaved);
}

DEF_BUTTON(         bSaveFirmware,                                                                                  //--- ОТЛАДКА - Сохр. прошивку ---
    "Сохр. прошивку", "Save firmware",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    pageDebug, IsActive_SaveFirmware, OnPress_SaveFirmware, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Exit()
{
    OnPressSB_Exit();
    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON_EXIT(  bSerialNumber_Exit,                                                                            //--- ОТЛАДКА - С/Н - Выход ---
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Exit, DrawSB_Exit
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Change()
{
    ACCESS_EXTRAMEM(StructForSN, s);
    ++s->curDigt;
    s->curDigt %= 2;
    Painter::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_TAB);
    Painter::SetFont(Font::Type::_8);
}

DEF_SMALL_BUTTON(   bSerialNumber_Change,                                                                           //--- ОТЛАДКА - С/Н - Вставить ---
    "Вставить", "Insert",
    "Вставляет выбраный символ",
    "Inserts the chosen symbol",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Save()
{
    ACCESS_EXTRAMEM(StructForSN, s);

    char stringSN[20];

    snprintf(stringSN, 19, "%02d %04d", s->number, s->year);

    if (!OTPmem::SaveSerialNumber(stringSN))
    {
        Display::ShowWarning(Warning::FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    Painter::SetFont(Font::Type::_8);
}

DEF_SMALL_BUTTON(   bSerialNumber_Save,                                                                            //--- ОТЛАДКА - С/Н - Сохранить ---
    "Сохранить", "Save",
    "Записывает серийный номер в OTP",
    "Records the serial number in OTP",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_EnterSerialNumber()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Painter::DrawRectangle(x0, y0, width, height, Color::FILL);
    Painter::FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::BACK);

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);

    bool selNumber = s->curDigt == 0;

    char buffer[20];
    snprintf(buffer, 19, "%02d", s->number);

    Color colorText = Color::FILL;
    Color colorBackground = Color::BACK;

    if (selNumber)
    {
        colorText = Color::FLASH_01;
        colorBackground = Color::FLASH_10;
    }

    int y = y0 + 50;

    Painter::SetColor(colorText);
    int x = Painter::DrawTextOnBackground(x0 + deltaX, y, buffer, colorBackground);

    colorText = Color::FLASH_01;
    colorBackground = Color::FLASH_10;

    if (selNumber)
    {
        colorText = Color::FILL;
        colorBackground = Color::BACK;
    }

    snprintf(buffer, 19, "%04d", s->year);

    Painter::SetColor(colorText);
    Painter::DrawTextOnBackground(x + 5, y, buffer, colorBackground);

    // Теперь выведем информацию об оставшемся месте в OTP-памяти для записи

    int allShots = 0;
    String serialNumber = OTPmem::GetSerialNumber(&allShots);

    String("Текущий сохранённый номер %s", serialNumber.CString()[0] == 0 ? "-- ----" : serialNumber.CString()).Draw(x0 + deltaX, y0 + 130, Color::FILL);

    String("Осталось места для %d попыток", allShots).Draw(x0 + deltaX, y0 + 100, Color::FILL);
}

static void OnPress_SerialNumber(bool)
{
    Display::SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number =  1; // -V522
    s->year = 2017;
    s->curDigt = 0;
}

static bool HandlerKey_SerialNumber(KeyEvent event)
{
    pFuncVpIII p = event.IsAboveZero() ? (Math::CircleIncrease<int>) : (Math::CircleDecrease<int>);

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        p(&s->number, 1, 99);
    }
    else
    {
        p(&s->year, 2016, 2050);
    }
    Sound::GovernorChangedValue();

    return true;
}

DEF_PAGE_SB( ppSerialNumber, // -V641 // -V1027                                                                                                                       //--- ОТЛАДКА - С/Н ---
    "С/Н", "S/N",
    "Запись серийного номера в OTP-память. ВНИМАНИЕ!!! ОТP-память - память с однократной записью.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,            // ОТЛАДКА - С/Н - Выход
    &bSerialNumber_Change,          // ОТЛАДКА - С/Н - Перейти
    0,
    0,
    0,
    &bSerialNumber_Save,            // ОТЛАДКА - С/Н - Сохранить
    Page::Name::SB_Debug_SerialNumber, &pageDebug, FuncActive, OnPress_SerialNumber, FuncDrawPage, HandlerKey_SerialNumber
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_EraseData()
{
    Display::FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    EEPROM::DeleteAllData();
    Display::FuncOnWaitStop();
}

DEF_BUTTON(         bEraseData,                                                                                     //--- ОТЛАДКА - Стереть данные ---
    "Стереть данне", "Erase data",
    "Стирает сохранённые данные из ППЗУ",
    "Erase all saved datas from EEPROM",
    pageDebug, FuncActive, OnPress_EraseData, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageDebug, // -V641                                                                                                                                             //--- ОТЛАДКА ---
    "ОТЛАДКА", "DEBUG",
    "",
    "",
    PageDebug::PageConsole::pointer,    ///< ОТЛАДКА - КОНСОЛЬ
    &ppADC,                             ///< ОТЛАДКА - АЦП
    PageDebug::PageRand::pointer,       ///< ОТЛАДКА - РАНД-ТОР
    &ppChannels,		                ///< ОТЛАДКА - КАНАЛЫ
    &cStats,			                ///< ОТЛАДКА - Статистика
//    &cDisplayOrientation,       ///< ОТЛАДКА - Ориентация
//    &mgPred,			          ///< ОТЛАДКА - Предзапуск
//    &mgPost,			          ///< ОТЛАДКА - Послезапуск
//    &ppSettings,		          ///< ОТЛАДКА - НАСТРОЙКИ
//    &bSaveFirmware,             ///< ОТЛАДКА - Сохр. прошивку
//    &ppSerialNumber,            ///< ОТЛАДКА - С/Н
//    &bEraseData,                ///< ОТЛАДКА - Стереть данные
    Page::Name::Debug, Menu::pageMain, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
