#include "defines.h"
#include "Data/Reader.h"
#include "Display/Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageService.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdio>


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
        {0, set.ch[Chan::A].balanceShiftADC, static_cast<int8>(set.dbg.nrst.balanceADC[Chan::A])},
        {0, set.ch[Chan::B].balanceShiftADC, static_cast<int8>(set.dbg.nrst.balanceADC[Chan::B])}
    };

    shiftADCA = shift[Chan::A][set.dbg.nrst.balanceADCtype];
    shiftADCB = shift[Chan::B][set.dbg.nrst.balanceADCtype];
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
    set.dbg.nrst.balanceADCtype, &PageDebug::PageADC::PageBalance::self, Item::Active, OnChanged_Balance_Mode, Draw_Balance_Mode
)


static bool IsActive_ShiftAB()
{
    return (set.dbg.nrst.balanceADCtype == BalanceADC::Hand);
}

static void OnChanged_ShiftA()
{
    set.dbg.nrst.balanceADC[Chan::A] = shiftADCA;
}

DEF_GOVERNOR( gShiftA,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---
    "Смещение 1",
    "",
    shiftADCA, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftA
)


static void OnChanged_ShiftB()
{
    set.dbg.nrst.balanceADC[Chan::B] = shiftADCB;
}

DEF_GOVERNOR( gShiftB,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 2 ---
    "Смещение 2",
    "",
    shiftADCB, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftB
)


DEF_PAGE_3( pBalance,                                                                                                                       //--- ОТЛАДКА - АЦП - БАЛАНС ---
    "БАЛАНС",
    "",
    &cBalance_Mode,
    &gShiftA,
    &gShiftB,
    PageName::Debug_ADC_Balance,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageADC::PageBalance::self = static_cast<const Page *>(&pBalance);


static int16 stretchA;
static int16 stretchB;

void PageDebug::PageADC::PageStretch::OnChanged_Mode(bool)
{
    if (set.dbg.nrst.stretchADCtype == StretchADC::Disable)
    {
        stretchA = set.dbg.nrst.stretchADC[Chan::A][StretchADC::Disable] = 0;
        stretchB = set.dbg.nrst.stretchADC[Chan::B][StretchADC::Disable] = 0;
    }
    else
    {
        stretchA = set.dbg.nrst.stretchADC[Chan::A][set.dbg.nrst.stretchADCtype];
        stretchB = set.dbg.nrst.stretchADC[Chan::B][set.dbg.nrst.stretchADCtype];
    }
}

DEF_CHOICE_3( cStretch_Mode,                                                                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    "Ручной",
    set.dbg.nrst.stretchADCtype, &PageDebug::PageADC::PageStretch::self, Item::Active, PageDebug::PageADC::PageStretch::OnChanged_Mode, Choice::AfterDraw
)


static bool IsActive_StretchAB()
{
    return (set.dbg.nrst.stretchADCtype == StretchADC::Hand);
}

static void OnChanged_Stretch_A()
{
    set.dbg.nrst.stretchADC[Chan::A][set.dbg.nrst.stretchADCtype] = stretchA;
}

DEF_GOVERNOR( gStretch_A,                                                                                                                //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к ---
    "Растяжка 1к",
    "Задаёт ручную растяжку первого канала.\n1 единица = 0.0001",
    stretchA, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_A
)


static void OnChanged_Stretch_B()
{
    set.dbg.nrst.stretchADC[Chan::B][set.dbg.nrst.stretchADCtype] = stretchB;
}

DEF_GOVERNOR( gStretch_B,                                                                                                                //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к ---
    "Растяжка 2к",
    "Задаёт ручную растяжку второго канала.\n1 единица = 0.0001",
    stretchB, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_B
)

//static const ChoiceBase emptyChoice = {Item::Type::Choice, 0, false, Page::Name::NoPage, 0, 0, {}, 0, 0, 0, 0};

DEF_PAGE_3( pStretch,                                                                                                                     //--- ОТЛАДКА - АЦП - РАСТЯЖКА ---
    "РАСТЯЖКА",
    "Устанавливает режим и величину растяжки (для ручного режима)",
    &cStretch_Mode,
    &gStretch_A,
    &gStretch_B,
    PageName::Debug_ADC_Stretch,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageADC::PageStretch::self = static_cast<const Page *>(&pStretch);



static void OnPress_ADC_Shift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < 3; range++)
        {
            set.dbg.nrst.rShiftAddStable[ch][range] = 0;
        }
    }
    RShift::Set(Chan::A, RShift::Value(Chan::A));
    RShift::Set(Chan::B, RShift::Value(Chan::B));
}

DEF_BUTTON( bShift_Reset,                                                                                                                       //-- ОТЛАДКА - АЦП - ДОП СМЕЩ - Сброс ---
    "Сброс",
    "",
    &PageDebug::PageADC::PageShift::self, Item::Active, OnPress_ADC_Shift_Reset
)


static void OnChanged_Shift_A()
{
    RShift::Set(Chan::A, RShift::Value(Chan::A));
}

DEF_GOVERNOR( gShift_A2mV,                                                                                                         //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 2мВ пост ---
    "См 1к 2мВ пост",
    "",
    set.dbg.nrst.rShiftAddStable[Chan::A][Range::_2mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)


static void OnChanged_Shift_B()
{
    RShift::Set(Chan::B, RShift::Value(Chan::B));
}

DEF_GOVERNOR( gShift_B2mV,                                                                                                                //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 2мВ пост ---
    "См 2к 2мВ пост",
    "",
    set.dbg.nrst.rShiftAddStable[Chan::B][Range::_2mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)


DEF_GOVERNOR( gShift_A5mV,                                                                                                                //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 5мВ пост ---
    "См 1к 5мВ пост",
    "",
    set.dbg.nrst.rShiftAddStable[Chan::A][Range::_5mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)


DEF_GOVERNOR( gShift_B5mV,                                                                                                                //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 5мВ пост ---
    "См 2к 5мВ пост",
    "",
    set.dbg.nrst.rShiftAddStable[Chan::B][Range::_5mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)


DEF_GOVERNOR( gShift_A10mV,                                                                                                              //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 1к 10мВ пост ---
    "См 1к 10мВ пост",
    "",
    set.dbg.nrst.rShiftAddStable[Chan::A][Range::_10mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)


DEF_GOVERNOR( gShift_B10mV,                                                                                                              //--- ОТЛАДКА - АЦП - ДОП СМЕЩ - См 2к 10мВ пост ---
    "См 2к 10мВ пост",
    "",
    set.dbg.nrst.rShiftAddStable[Chan::B][Range::_10mV], -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)


DEF_PAGE_7( pShift,                                                                                                                                        //--- ОТЛАДКА - АЦП - ДОП СМЕЩ ---
    "ДОП СМЕЩ",
    "",
    &bShift_Reset,
    &gShift_A2mV,
    &gShift_B2mV,
    &gShift_A5mV,
    &gShift_B5mV,
    &gShift_A10mV,
    &gShift_B10mV,
    PageName::Debug_ADC_Shift,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageADC::PageShift::self = static_cast<const Page *>(&pShift);


DEF_PAGE_3( pADC,  //-V1027
    "АЦП",
    "",
    &pBalance,
    &pStretch,
    PageDebug::PageADC::PageShift::self,
    PageName::Debug_ADC,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageADC::self = static_cast<const Page *>(&pADC);


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
            for (int range = 0; range < Range::Count; ++range)
            {
                String("%d", set.dbg.nrst.rShiftAdd[ch][range][type]).Draw(x + range * 20, y + dY * ddY);
            }
            ddY++;
        }
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", set.dbg.nrst.correctionTime); //-V2528
    DRAW_FORMAT2("balanceADC : %d %d", set.dbg.nrst.balanceADC[Chan::A], set.dbg.nrst.balanceADC[Chan::B]); //-V2528
    DRAW_FORMAT("numAveForRand : %d", set.dbg.nrst.numAveForRand); //-V2528

    pString s[3] = {"выключено", "настроено автоматически", "задано вручную"};
    DRAW_FORMAT("balanceADCtype : %s", (set.dbg.nrst.balanceADCtype < 3 ? s[set.dbg.nrst.balanceADCtype] : "!!! неправильное значение !!!")); //-V547 //-V2528
    DRAW_FORMAT("stretchADCtype : %s", (set.dbg.nrst.stretchADCtype < 3 ? s[set.dbg.nrst.stretchADCtype] : "!!! неправильное значение !!!")); //-V547 //-V2528

    x = String("stretchADC :").Draw(x0, INC_Y) + 5; //-V2528

    for (int ch = 0; ch < 2; ch++)
    {
        for (int num = 0; num < 3; num++)
        {
            String("%d", set.dbg.nrst.stretchADC[ch][num]).Draw(x + num * 20, y + dY * ch);
        }
    }

    y += dY;

#define DRAW_STRETCH(name) DRAW_FORMAT2(#name " : %d %d", set.nrst_##name[0], set.nrst_##name[1])

    DRAW_FORMAT("numSmoothForRand : %d", set.dbg.nrst.numSmoothForRand); //-V2528

    Menu::Draw();
    Painter::EndScene();
}


static void OnOpenClose_Settings(bool)
{
    Display::SetDrawMode(DrawMode::Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_5( pSettings,                                                                                                                         //--- ОТЛАДКА - НАСТРОЙКИ ---
    "НАСТРОЙКИ",
    "Показать информацию о настройках",
    0,
    0,
    0,
    0,
    0,
    PageName::Debug_Settings,
    &PageDebug::self, Item::Active, Page::NormalTitle, OnOpenClose_Settings, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageSettings::self = static_cast<const Page *>(&pSettings);



static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    Display::FuncOnWaitStart("Сохраняю прошивку", false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite(NAME_FILE_FIRMWARE, &structForWrite);

    uint8 *address = reinterpret_cast<uint8 *>(0x08020000); //-V566
    uint8 *endAddress = address + SIZE_FILE_FIRMWARE; //-V566

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStop();

    Display::ShowWarning("Прошивка сохранена");
}

DEF_BUTTON( bSaveFirmware,                                                                                                                                 //--- ОТЛАДКА - Сохр. прошивку ---
    "Сохр. прошивку",
    "Сохранение прошивки - секторов 5, 6, 7 общим объёмом 3 х 128 кБ, где хранится программа",
    &PageDebug::self, IsActive_SaveFirmware, OnPress_SaveFirmware
)


static void OnPress_SerialNumber_Change()
{
    Color::ChangeFlash(true);
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Char(SymbolUGO2::TAB).Draw4SymbolsInRect(x + 2, y + 2);
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
    Char(SymbolUGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
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

static bool HandlerKey_SerialNumber(const KeyEvent &)
{
    return false;
}

DEF_PAGE_2( pSerialNumber,                                                                                                                                            //--- ОТЛАДКА - С/Н ---
    "С/Н",
    "Запись серийного номера в OTP-память. ВНИМАНИЕ!!! ОТP-память - память с однократной записью.",
    &bSerialNumber_Change,
    &bSerialNumber_Save,
    PageName::Debug_SerialNumber,
    &PageDebug::self, Item::Active, Page::NormalTitle, OnOpenClose_SerialNumber, Page::BeforeDraw, HandlerKey_SerialNumber
)

const Page * const PageDebug::PageSerialNumber::self = static_cast<const Page *>(&pSerialNumber);


DEF_PAGE_6( pDebug,                                                                                                                                                         //--- ОТЛАДКА ---
    "ОТЛАДКА",
    "",
    PageDebug::PageConsole::self,
    &pADC,
    PageDebug::PageRand::self,
    &cStats,
    &bSaveFirmware,
    PageDebug::PageTests::self,
    PageName::Debug,
    &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::self = static_cast<const Page *>(&pDebug);



float GetStretchADC(Chan::E ch)
{
    static const int16 *addStretch[Range::Count][2] =
    {
        {0, 0},  // 2mV
        {0, 0},  // 5mV
        {0, 0},  // 10mV
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0}, // 200mV
        {0, 0}, // 500mV
        {0, 0},
        {0, 0},
        {0, 0}
    };

    const int16 *address = addStretch[Range::Get(ch)][ch];

    int16 stretch = set.dbg.nrst.stretchADC[ch][set.dbg.nrst.stretchADCtype];

    if (address)
    {
        stretch += (*address);
    }

    return stretch * 1e-4F + 1.0F;
}


//------------------------------
void SetStretchADC(Chan::E ch, float kStretch)
{
    set.dbg.nrst.stretchADC[ch][set.dbg.nrst.stretchADCtype] = static_cast<int16>((kStretch - 1.0F) * 1e4F);
}

