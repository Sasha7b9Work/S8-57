#include "defines.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageService.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdio>


using namespace Display::Primitives;



/// В этой структуре будут храниться данные серийного номера при открытой странице ppSerialNumer
typedef struct
{
    int number;     ///< Соответственно, порядковый номер.
    int year;       ///< Соответственно, год.
    int curDigt;    ///< Соответственно, номером (0) или годом (1) управляет ручка УСТАНОВКА.
} StructForSN;


static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, set.ch[Chan::A].balanceShiftADC, (int8)set.nrst.balanceADC[Chan::A]},
        {0, set.ch[Chan::B].balanceShiftADC, (int8)set.nrst.balanceADC[Chan::B]}
    };

    shiftADCA = shift[Chan::A][set.nrst.balanceADCtype];
    shiftADCB = shift[Chan::B][set.nrst.balanceADCtype];
}

static void OnChanged_Balance_Mode(bool)
{
    Draw_Balance_Mode(0, 0);
}

DEF_CHOICE_3( cBalance_Mode,                                                                                                                         //--- ОТЛАДКА - АЦП - БАЛАНС - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    "Ручной",
    set.nrst.balanceADCtype, &PageDebug::PageADC::PageBalance::self, Item::Active, OnChanged_Balance_Mode, Draw_Balance_Mode
)


static bool IsActive_ShiftAB()
{
    return (set.nrst.balanceADCtype == BalanceADC::Hand);
}

static void OnChanged_ShiftA()
{
    set.nrst.balanceADC[Chan::A] = shiftADCA;
}

DEF_GOVERNOR( gShiftA,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---
    "Смещение 1",
    "",
    shiftADCA, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftA
)


static void OnChanged_ShiftB()
{
    set.nrst.balanceADC[Chan::B] = shiftADCB;
}

DEF_GOVERNOR( gShiftB,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 2 ---
    "Смещение 2",
    "",
    shiftADCB, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftB
)


DEF_PAGE_3( pBalance, // -V641 // -V1027                                                                                                                     //--- ОТЛАДКА - АЦП - БАЛАНС ---
    "БАЛАНС",
    "",
    &cBalance_Mode,     ///< ОТЛАДКА - АЦП - БАЛАНС - Режим
    &gShiftA,           ///< ОТЛАДКА - АЦП - БАЛАНС - Смещение 1
    &gShiftB,           ///< ОТЛАДКА - АЦП - БАЛАНС - Смещение 2
    PageName::Debug_ADC_Balance,
    &PageDebug::PageADC::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::PageBalance::self = (const Page *)&pBalance;


static int16 stretchA;
static int16 stretchB;

void PageDebug::PageADC::PageStretch::OnChanged_Mode(bool)
{
    if (set.nrst.stretchADCtype == StretchADC::Disable)
    {
        stretchA = set.nrst.stretchADC[Chan::A][StretchADC::Disable] = 0;
        stretchB = set.nrst.stretchADC[Chan::B][StretchADC::Disable] = 0;
    }
    else
    {
        stretchA = set.nrst.stretchADC[Chan::A][set.nrst.stretchADCtype];
        stretchB = set.nrst.stretchADC[Chan::B][set.nrst.stretchADCtype];
    }
}

DEF_CHOICE_3( cStretch_Mode,                                                                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    "Ручной",
    set.nrst.stretchADCtype, &PageDebug::PageADC::PageStretch::self, Item::Active, PageDebug::PageADC::PageStretch::OnChanged_Mode, Choice::AfterDraw
)


static bool IsActive_StretchAB()
{
    return (set.nrst.stretchADCtype == StretchADC::Hand);
}

static void OnChanged_Stretch_A()
{
    set.nrst.stretchADC[Chan::A][set.nrst.stretchADCtype] = stretchA;
}

DEF_GOVERNOR( gStretch_A,                                                                                                                //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к ---
    "Растяжка 1к",
    "Задаёт ручную растяжку первого канала.\n1 единица = 0.0001",
    stretchA, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_A
)


static void OnChanged_Stretch_B()
{
    set.nrst.stretchADC[Chan::B][set.nrst.stretchADCtype] = stretchB;
}

DEF_GOVERNOR( gStretch_B,                                                                                                                //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к ---
    "Растяжка 2к",
    "Задаёт ручную растяжку второго канала.\n1 единица = 0.0001",
    stretchB, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_B
)

/*

_DEF_GOVERNOR(       gADC_Stretch_Ak20mV,                                                               //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 1к ---
    "20мВ/1В 1к", "20mV/1V 1k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Ak50mV,                                                                  //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 50мВ 1к ---
    "50мВ 1к", "50mV 1k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Ak100mV,                                                             //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 1к ---
    "100мВ/5В 1к", "100mV/5V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_100mV_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Ak2V,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 1к ---
    "2В 1к", "2V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Bk20mV,                                                               //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 20мВ/1В 2к ---
    "20мВ/1В 2к", "20mV/1V 2k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Bk50mV,                                                         //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Доп смещ 50мВ 2к ---
    "50мВ 2к", "50mV 2k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Bk100mV,                                                             //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 100мВ/5В 2к ---
    "100мВ/5В 2к", "100mV/5V 2k",
    "",
    "",
    NRST_ADD_STRETCH_100mV_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)


_DEF_GOVERNOR(       gADC_Stretch_Bk2V,                                                                      //--- ОТЛАДКА - АЦП - РАСТЯЖКА - 2В 2к ---
    "2В 2к", "2V 2ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)
*/


//static const ChoiceBase emptyChoice = {Item::Type::Choice, 0, false, Page::Name::NoPage, 0, 0, {}, 0, 0, 0, 0};

DEF_PAGE_3( pStretch, // -V641 // -V1027                                                                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА ---
    "РАСТЯЖКА",
    "Устанавливает режим и величину растяжки (для ручного режима)",
    &cStretch_Mode,      ///< ОТЛАДКА - АЦП - РАСТЯЖКА - Режим
    &gStretch_A,         ///< ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к
    &gStretch_B,
    PageName::Debug_ADC_Stretch,
    &PageDebug::PageADC::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::PageStretch::self = (const Page *)&pStretch;



static void OnPress_ADC_Shift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < 3; range++)
        {
            set.nrst.rShiftAddStable[ch][range] = 0;
        }
    }
    RShift::Set(Chan::A, SET_RSHIFT_A);
    RShift::Set(Chan::B, SET_RSHIFT_B);
}

DEF_BUTTON( bShift_Reset,                                                                                                                       //-- ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс ---
    "Сброс",
    "",
    &PageDebug::PageADC::PageShift::self, Item::Active, OnPress_ADC_Shift_Reset
)


static void OnChanged_Shift_A()
{
    RShift::Set(Chan::A, SET_RSHIFT_A);
}

DEF_GOVERNOR( gShift_A2mV, //-V641                                                                                                        //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост ---
    "См 1к 2мВ пост",
    "",
    set.nrst.rShiftAddStable[Chan::A][Range::_2mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)


static void OnChanged_Shift_B()
{
    RShift::Set(Chan::B, SET_RSHIFT_B);
}

DEF_GOVERNOR( gShift_B2mV,                                                                                                                //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост ---
    "См 2к 2мВ пост",
    "",
    set.nrst.rShiftAddStable[Chan::B][Range::_2mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)


DEF_GOVERNOR( gShift_A5mV,                                                                                                                //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост ---
    "См 1к 5мВ пост",
    "",
    set.nrst.rShiftAddStable[Chan::A][Range::_5mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)


DEF_GOVERNOR( gShift_B5mV,                                                                                                                //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост ---
    "См 2к 5мВ пост",
    "",
    set.nrst.rShiftAddStable[Chan::B][Range::_5mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)


DEF_GOVERNOR( gShift_A10mV,                                                                                                              //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост ---
    "См 1к 10мВ пост",
    "",
    set.nrst.rShiftAddStable[Chan::A][Range::_10mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)


DEF_GOVERNOR( gShift_B10mV,                                                                                                              //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост ---
    "См 2к 10мВ пост",
    "",
    set.nrst.rShiftAddStable[Chan::B][Range::_10mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)


DEF_PAGE_7( pShift, // -V641  // -V1027                                                                                                                    //--- ОТЛАДКА - АЦП - ДОП СМЕЩ ---
    "ДОП СМЕЩ",
    "",
    &bShift_Reset,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс
    &gShift_A2mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост
    &gShift_B2mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост
    &gShift_A5mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост
    &gShift_B5mV,    // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост
    &gShift_A10mV,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост
    &gShift_B10mV,   // ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост
    PageName::Debug_ADC_Shift,
    &PageDebug::PageADC::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::PageShift::self = (const Page *)&pShift;


DEF_PAGE_3( pADC, //-V641 //-V1027
    "АЦП",
    "",
    &pBalance,                              // ОТЛАДКА - АЦП - БАЛАНС
    &pStretch,                              // ОТЛАДКА - АЦП - РАСТЯЖКА
    PageDebug::PageADC::PageShift::self,    // ОТЛАДКА - АЦП - ДОП СМЕЩ
    PageName::Debug_ADC,
    &PageDebug::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::self = (const Page *)&pADC;


DEF_CHOICE_2( cStats,                                                                                                                                          //--- ОТЛАДКА - Статистика ---
    "Статистика",
    "Показывать/не показывать время/кадр, кадров в секунду, количество сигналов с последними настройками в памяти/количество сохраняемых в памяти сигналов"
    ,
    "Не показывать",
    "Показывать",
    set.dbg.ShowStats, &PageDebug::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static int IncreaseY(int &y, int dY)
{
    y += dY;
    return y;
}


static void DebugShowSetInfo_Draw()
{
    Painter::BeginScene(Color::BACK);
    Rectangle(319, 239).Draw(0, 0, Color::FILL);

    int x0 = 30;
    int y0 = 25;
    int dY = 10;
    int y = y0 - dY;

#define INC_Y IncreaseY(y, dY)
#define DRAW_TEXT(str)                  Painter::DrawText(x0, INC_Y, str)
#define DRAW_FORMAT(str, value)         String(str, value).Draw(x0, INC_Y)
#define DRAW_FORMAT2(str, val1, val2)   String(str, val1, val2).Draw(x0, INC_Y)

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "Размер основной структуры %d", sizeof(set));
    DRAW_FORMAT("Размер основной структуры : %d", sizeof(set)); //-V2528
    String("Несбрасываемая структура:").Draw(x0, INC_Y); //-V2528

    int x = String("rShiftAdd :").Draw(x0, INC_Y) + 5; //-V2528

    int ddY = 0;

    for (int type = 0; type < 2; type++)
    {
        for (int ch = 0; ch < 2; ch++)
        {
            for (int range = 0; range < Range::Size; ++range)
            {
                String("%d", set.nrst.rShiftAdd[ch][range][type]).Draw(x + range * 20, y + dY * ddY);
            }
            ddY++;
        }
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", set.nrst.correctionTime); //-V2528
    DRAW_FORMAT2("balanceADC : %d %d", set.nrst.balanceADC[Chan::A], set.nrst.balanceADC[Chan::B]); //-V2528
    DRAW_FORMAT("numAveForRand : %d", set.nrst.numAveForRand); //-V2528

    pString s[3] = {"выключено", "настроено автоматически", "задано вручную"};
    DRAW_FORMAT("balanceADCtype : %s", (set.nrst.balanceADCtype < 3 ? s[set.nrst.balanceADCtype] : "!!! неправильное значение !!!")); //-V547 //-V2528
    DRAW_FORMAT("stretchADCtype : %s", (set.nrst.stretchADCtype < 3 ? s[set.nrst.stretchADCtype] : "!!! неправильное значение !!!")); //-V547 //-V2528

    x = String("stretchADC :").Draw(x0, INC_Y) + 5; //-V2528

    for (int ch = 0; ch < 2; ch++)
    {
        for (int num = 0; num < 3; num++)
        {
            String("%d", set.nrst.stretchADC[ch][num]).Draw(x + num * 20, y + dY * ch);
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

    DRAW_FORMAT("numSmoothForRand : %d", set.nrst.numSmoothForRand); //-V2528

    Menu::Draw();
    Painter::EndScene();
}


static void OnOpenClose_Settings(bool)
{
    Display::SetDrawMode(Display::DrawMode::Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_5( pSettings, // -V641 // -V1027                                                                                                                       //--- ОТЛАДКА - НАСТРОЙКИ ---
    "НАСТРОЙКИ",
    "Показать информацию о настройках",
    0,
    0,
    0,
    0,
    0,
    PageName::Debug_Settings,
    &PageDebug::self, Item::Active, OnOpenClose_Settings, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageSettings::self = (const Page *)&pSettings;



static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    Display::FuncOnWaitStart("Сохраняю прошивку", false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite(NAME_FILE_FIRMWARE, &structForWrite);

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + SIZE_FILE_FIRMWARE; //-V566

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

DEF_BUTTON( bSaveFirmware,                                                                                                                                 //--- ОТЛАДКА - Сохр. прошивку ---
    "Сохр. прошивку",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    &PageDebug::self, IsActive_SaveFirmware, OnPress_SaveFirmware
)


static void OnPress_SerialNumber_Change()
{
    Color::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);

    Char(SYMBOL_TAB).Draw4SymbolsInRect(x + 2, y + 2);

    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSerialNumber_Change,                                                                                                                    //--- ОТЛАДКА - С/Н - Вставить ---
    "Вставить",
    "Вставляет выбраный символ",
    &PageDebug::PageSerialNumber::self, Item::Active, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
)


static void OnPress_SerialNumber_Save()
{
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSerialNumber_Save,                                                                                                                     //--- ОТЛАДКА - С/Н - Сохранить ---
    "Сохранить",
    "Записывает серийный номер в OTP",
    &PageDebug::PageSerialNumber::self, Item::Active, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
)


static void Draw_EnterSerialNumber()
{
}

static void OnOpenClose_SerialNumber(bool)
{
    Draw_EnterSerialNumber();
}

static bool OnArrows_SerialNumber(const KeyEvent &)
{
    return true;
}

DEF_PAGE_2( pSerialNumber, // -V641 // -V1027                                                                                                                         //--- ОТЛАДКА - С/Н ---
    "С/Н",
    "Запись серийного номера в OTP-память. ВНИМАНИЕ!!! ОТP-память - память с однократной записью.",
    &bSerialNumber_Change,          // ОТЛАДКА - С/Н - Перейти
    &bSerialNumber_Save,            // ОТЛАДКА - С/Н - Сохранить
    PageName::Debug_SerialNumber,
    &PageDebug::self, Item::Active, OnOpenClose_SerialNumber, Page::BeforeDraw, OnArrows_SerialNumber
)

const Page * const PageDebug::PageSerialNumber::self = (const Page *)&pSerialNumber;


//static void OnPress_EraseData()
//{
//    Display::FuncOnWaitStart("Удаляю сохранённые данные", false);
//    ::Memory::DeleteAllData();
//    Display::FuncOnWaitStop();
//}

//_DEF_BUTTON( bEraseData,                                                                                                                                    //--- ОТЛАДКА - Стереть данные ---
//    "Стереть данне",
//    "Стирает сохранённые данные из ППЗУ",
//    &PageDebug::self, 0, OnPress_EraseData, 0
//)


DEF_PAGE_5( pDebug, // -V641 // -V1027                                                                                                                                   //--- ОТЛАДКА ---
    "ОТЛАДКА",
    "",
    PageDebug::PageConsole::self,   ///< ОТЛАДКА - КОНСОЛЬ
    &pADC,                                         ///< ОТЛАДКА - АЦП
    PageDebug::PageRand::self,      ///< ОТЛАДКА - РАНД-ТОР
    &cStats,			                            ///< ОТЛАДКА - Статистика
    &bSaveFirmware,                                 ///< ОТЛАДКА - Сохр. прошивку
//    &cDisplayOrientation,                           ///< ОТЛАДКА - Ориентация
//    &mgPred,			                            ///< ОТЛАДКА - Предзапуск
//    &mgPost,			                            ///< ОТЛАДКА - Послезапуск
//    &pSettings,		                            ///< ОТЛАДКА - НАСТРОЙКИ
//    &pSerialNumber,                              ///< ОТЛАДКА - С/Н
//    &bEraseData,                                  ///< ОТЛАДКА - Стереть данные
    PageName::Debug,
    &PageService::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::self = (const Page *)&pDebug;



float GetStretchADC(Chan::E ch)
{
    static const int16 *addStretch[Range::Size][2] =
    {
        {0, 0},  // 2mV
        {0, 0},  // 5mV
        {0, 0},  // 10mV
        {0, 0},
        {0, 0},
        {0, 0},
        /*
        {&NRST_ADD_STRETCH_20mV_A, &NRST_ADD_STRETCH_20mV_B},   // 20mV
        {&NRST_ADD_STRETCH_50mV_A, &NRST_ADD_STRETCH_50mV_B},   // 50mV
        {&NRST_ADD_STRETCH_100mV_A, &NRST_ADD_STRETCH_100mV_B}, // 100mV
        */
        {0, 0}, // 200mV
        {0, 0}, // 500mV
        {0, 0},
        {0, 0},
        {0, 0}
        /*
        {&NRST_ADD_STRETCH_20mV_A, &NRST_ADD_STRETCH_20mV_B},   // 1V
        {&NRST_ADD_STRETCH_2V_A, &NRST_ADD_STRETCH_2V_B},       // 2V
        {&NRST_ADD_STRETCH_100mV_A, &NRST_ADD_STRETCH_100mV_B}  // 5V
        */
    };

    const int16 *address = addStretch[set.ch[ch].range][ch];

    int16 stretch = set.nrst.stretchADC[ch][set.nrst.stretchADCtype];

    if (address)
    {
        stretch += (*address);
    }

    return stretch * 1e-4F + 1.0F;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void SetStretchADC(Chan::E ch, float kStretch)
{
    set.nrst.stretchADC[ch][set.nrst.stretchADCtype] = (int16)((kStretch - 1.0F) * 1e4F);
}

