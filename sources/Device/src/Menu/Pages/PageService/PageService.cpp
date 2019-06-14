#include "defines.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageService.h"
#include "Keyboard/Keyboard.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include <cstdio>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const PageBase pService;
extern const PageBase ppFunction;
extern const PageBase ppSound;
extern const PageBase ppRTC;
extern const PageBase ppInformation;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetSettings()
{
    Settings::Load(true);
    if (Device::State::InModeOsci())
    {
        FPGA::OnPressStart();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bResetSettings,                                                                                                                                 //--- СЕРВИС - Сброс настроек ---
    "Сброс настр",
    "Сброс настроек на настройки по умолчанию",
    pService, EmptyFuncBtV, OnPress_ResetSettings, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON( bAutoSearch,                                                                                                                                     //--- СЕРВИС - Поиск сигнала ---
    "Поиск сигн",
    "Устанавливает оптимальные установки осциллографа для сигнала в канале 1",
    pService, FuncActive, OnPress_AutoSearch, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Screen()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        Math::CircleIncrease<int8>((int8 *)&FUNC_MODE_DRAW, 0, 2);
    }
}

static void Draw_Function_Screen_Disable(int x, int y)
{
    String("Выx").Draw(x + 2, y + 5);
}

static void Draw_Function_Screen_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
	HLine line(13);
	line.Draw(x + 3, y + 9);
	line.Draw(x + 3, y + 10);
}

static void Draw_Function_Screen_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}

static void Draw_Function_Screen(int x, int y)
{
    const pFuncVII funcs[3] =
    {
        Draw_Function_Screen_Disable,
        Draw_Function_Screen_Separate,
        Draw_Function_Screen_Together
    };
    funcs[FUNC_MODE_DRAW](x, y);
}

DEF_SMALL_BUTTON_HINTS_3( bFunction_Screen,                                                                                                                //--- СЕРВИС - ФУНКЦИЯ - Экран ---
    "Экран",
    "Выбирает режим отображения математического сигнала",
    ppFunction, FuncActive, OnPress_Function_Screen, Draw_Function_Screen,
    Draw_Function_Screen_Disable,   "Вывод математической функции отключён",
    Draw_Function_Screen_Separate,  "Сигналы и математическая функция выводятся в разных окнах",
    Draw_Function_Screen_Together,  "Сигналы и математическая функция выводятся в одном окне"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Type()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}

static void Draw_Function_Type_Sum(int x, int y)
{
    HLine(10).Draw(x + 4, y + 9);
    VLine(10).Draw(x + 9, y + 4);
}

static void Draw_Function_Type_Mul(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
    Font::SetCurrent(Font::Type::_8);
}

static void Draw_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Function_Type_Sum, Draw_Function_Type_Mul};
    funcs[MATH_FUNC](x, y);
}

DEF_SMALL_BUTTON_HINTS_2( bFunction_Type,                                                                                                                    //--- СЕРВИС - ФУНКЦИЯ - Вид ---
    "Вид",
    "Выбор математической функции",
    ppFunction, FuncActive, OnPress_Function_Type, Draw_Function_Type,
    Draw_Function_Type_Sum, "Сложение",
    Draw_Function_Type_Mul, "Умножение"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_ModeRegSet()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}

static void Draw_Function_ModeRegSet_Range(int x, int y)
{
    Char('M').Draw(x + 7, y + 5);
}

static void Draw_Function_ModeRegSet_RShift(int x, int y)
{
    String("См").Draw(x + 5, y + 5);
}

static void Draw_Function_ModeRegSet(int x, int y)
{
    static const pFuncVII funcs[2] = {Draw_Function_ModeRegSet_Range, Draw_Function_ModeRegSet_RShift};
    funcs[MATH_MODE_REG_SET](x, y);
}

DEF_SMALL_BUTTON_HINTS_2( bFunction_ModeRegSet,                                                                                            //--- СЕРВИС - ФУНКЦИЯ - Режим ручки УСТАНОВКА ---
    "Режим ручки УСТАНОВКА",
    "Выбор режима ручки УСТАНОВКА - управление масштабом или смещением",
    ppFunction, FuncActive, OnPress_Function_ModeRegSet, Draw_Function_ModeRegSet,
    Draw_Function_ModeRegSet_Range,  "Управление масштабом",
    Draw_Function_ModeRegSet_RShift, "Управление смещением"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeA()
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_DIVIDER = (int8)SET_DIVIDER(Chan::A);
}

static void Draw_Function_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}

DEF_SMALL_BUTTON( bFunction_RangeA,                                                                                                          //--- СЕРВИС - ФУНКЦИЯ - Масштаб 1-го канала ---
    "Масштаб 1-го канала",
    "Использует масштаб первого канала для отображения результата",
    ppFunction, FuncActive, OnPress_Function_RangeA, Draw_Function_RangeA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeB()
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_DIVIDER = (int8)SET_DIVIDER(Chan::B);
}

static void Draw_Function_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}

DEF_SMALL_BUTTON( bFunction_RangeB,                                                                                                          //--- СЕРВИС - ФУНКЦИЯ - Масштаб 2-го канала ---
    "Масштаб 2-го канала",
    "Использует масштаб второго канала для отображения результата",
    ppFunction, FuncActive, OnPress_Function_RangeB, Draw_Function_RangeB
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Function()
{
    return !FFT_ENABLED;
}

static void OnPress_Function(bool)
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
}

static bool HandlerKey_Function(KeyEvent event) // -V2506
{
    if (!FUNC_MODE_DRAW_IS_ENABLED)
    {
        return false;
    }

    Key::E key = event.key;

    int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        uint16 prevRShift = SET_RSHIFT_MATH;
        uint16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShift::MAX)
            {
                rShift += 4 * STEP_RSHIFT;  // -V112
                LIMIT_ABOVE(rShift, RShift::MAX);
                if (prevRShift < RShift::ZERO && rShift > RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Beeper::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
        else
        {
            if (rShift > RShift::MIN)
            {
                rShift -= 4 * STEP_RSHIFT; // -V112
                LIMIT_BELOW(rShift, RShift::MIN);
                if (prevRShift > RShift::ZERO && rShift < RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Beeper::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = FPGA::Math::RShift2Abs(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < Range::Size - 1)
            {
                SET_RANGE_MATH = (Range::E)((uint8)(SET_RANGE_MATH + 1));  // SET_RANGE_MATH++;
                SET_RSHIFT_MATH = (uint16)FPGA::Math::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Beeper::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                SET_RANGE_MATH = (Range::E)((uint8)(SET_RANGE_MATH - 1));  // SET_RANGE_MATH--;
                SET_RSHIFT_MATH = (uint16)FPGA::Math::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Beeper::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else
        {
            // ничего делать не нужно
        }
    }

    return true;
}


DEF_PAGE_5( ppFunction, // -V641                                                                                                                                   //--- СЕРВИС - ФУНКЦИЯ ---
    "ФУНКЦИЯ",
    "Установка и выбор математической функции - сложения или умножения",
    &bFunction_Screen,      // СЕРВИС - ФУНКЦИЯ - Экран
    &bFunction_Type,        // СЕРВИС - ФУНКЦИЯ - Вид
    &bFunction_ModeRegSet,  // СЕРВИС - ФУНКЦИЯ - Режим ручки УСТАНОВКА
    &bFunction_RangeA,      // СЕРВИС - ФУНКЦИЯ - Масштаб 1-го канала
    &bFunction_RangeB,
    Page::Name::SB_Service_Function, &pService, IsActive_Function, OnPress_Function, FuncDrawPage, HandlerKey_Function
)




//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cSound_Enable,                                                                                                                                   //--- СЕРВИС - ЗВУК - Звук ---
    "Звук",
    "Включение/выключение звука",
    DISABLE_RU,
    ENABLE_RU,
    SOUND_ENABLED, ppSound, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gSound_Volume,                                                                                                                              //--- СЕРВИС - ЗВУК - Громкость ---
    "Громкость",
    "Установка громкости звука",
    SOUND_VOLUME, 0, 100, ppSound, FuncActive, FuncChanged, FuncBeforeDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( ppSound, // -V641 // -V1027                                                                                                                               //--- СЕРВИС - ЗВУК ---
    "ЗВУК",
    "В этом меню можно настроить громкость звука",
    &cSound_Enable,             ///< СЕРВИС - ЗВУК - Звук
    &gSound_Volume,             ///< СЕРВИС - ЗВУК - Громкость
    Page::Name::Service_Sound, &pService, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME( tRTC_Time,                                                                                                                                         //--- СЕРВИС - ВРЕМЯ - Время ---
    "Время"
    ,
    "Установка текущего времени.\nПорядок работы:\n"
    "Нажать на элемент меню \"Время\". Откроется меню установки текущего времени. Короткими нажатиями кнопки на цифровой клавиатуре, соответсвующей "
    "элементу управления \"Время\", выделить часы, минуты, секунды, год, месяц, или число. Выделенный элемент обозначается мигающей областью. "
    "Вращением ручки УСТАНОВКА установить необходимое значение. Затем выделить пункт \"Сохранить\", нажать и удреживать более 0.5 сек кнопку на "
    "панели управления. Меню установки текущего временя закроется с сохранением нового текущего времени. Нажатие длительное удержание кнопки на "
    "любом другом элементе приведёт к закрытию меню установки текущего вре    мени без сохранения нового текущего времени"
    ,
    ppRTC, FuncActive, dServicetime, hours, minutes, secondes, month, day, year
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Time_Correction()
{
}

DEF_GOVERNOR( tRTC_Correction,                                                                                                                           //--- СЕРВИС - ВРЕМЯ - Коррекция ---
    "Коррекция",
    "Установка корректирующего коэффициента для компенсации хода времени",
    NRST_CORRECTION_TIME, -63, 63, ppRTC, FuncActive, OnChanged_Time_Correction, FuncBeforeDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( ppRTC, // -V641 // -V1027                                                                                                                                //--- СЕРВИС - ВРЕМЯ ---
    "ВРЕМЯ",
    "Установка и настройка времени",
    &tRTC_Time,          // СЕРВИС - ВРЕМЯ - Время
    &tRTC_Correction,    // CЕРВИС - ВРЕМЯ - Коррекция
    Page::Name::Service_RTC, &pService, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Information_Draw()
{
    Painter::BeginScene(Color::BACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, Color::FILL);
    y += dY;
    String("ИНФОРМАЦИЯ").Draw(x, y);
    y += dY;

    char buffer[100];

    std::sprintf(buffer, "%s : %s", "Модель", MODEL_RU);
    String(buffer).Draw(x, y);

    y += 2 * dY;

    String("Программное обеспечение :").Draw(x, y);
    y += dY;

    std::sprintf(buffer, "версия %s", NUM_VER);

    String(buffer).Draw(x, y);
    y += dY;

    //Painter::DrawFormText(x, y, Color::FILL, "CRC32 : %X", Hardware::CalculateCRC32());

    dY = -10;

    Text("Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ").DrawInCenterRect(0, 190 + dY, 320, 20);
    Text("Отдел маркетинга: тел./факс. 8-017-262-57-50").DrawInCenterRect(0, 205 + dY, 320, 20);
    Text("Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-262-57-51").DrawInCenterRect(0, 220 + dY, 320, 20);

    Menu::Draw();
    Painter::EndScene();
}

static void OnPress_Information(bool)
{
    Display::SetDrawMode(Display::DrawMode::Auto, Information_Draw);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Information_Exit()
{
    Display::SetDrawMode(Display::DrawMode::Auto, 0);
}

DEF_SMALL_BUTTON_EXIT( bInformation_Exit,                                                                                                               //--- СЕРВИС - ИНФОРМАЦИЯ - Выход ---
    ppInformation, FuncActive, OnPress_Information_Exit, DrawSB_Exit
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB( ppInformation, // -V641                                                                                                                            //--- СЕРВИС - ИНФОРМАЦИЯ ---
    "ИНФОРМАЦИЯ",
    "Показывает информацию о приборе",
    &bInformation_Exit, // СЕРВИС - ИНФОРМАЦИЯ - Выход
    0,
    0,
    0,
    0,
    0,
    Page::Name::SB_Service_Information, &pService, FuncActive, OnPress_Information, FuncDrawPage, FuncRegSetPage
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::pointer = &pService;

DEF_PAGE_7( pService, // -V641 // -V1027                                                                                                                                    //--- СЕРВИС ---
    "СЕРВИС",
    "Дополнительные настройки, калибровка, поиск сигнала, математические функции",
    &bResetSettings,                        ///< СЕРВИС - Сброс настроек
    //&bAutoSearch,                         ///< СЕРВИС - Поиск сигнала
    //&ppCalibrator,
    PageService::PageCalibrate::pointer,    ///< СЕРВИС - КАЛИБРОВКА
    &ppFunction,                            ///< СЕРВИС - ФУНКЦИЯ
    &ppSound,                               ///< СЕРВИС - ЗВУК
    &ppRTC,                                 ///< СЕРВИС - ВРЕМЯ
    //PageService::PageBattery::pointer,    ///< СЕРВИС - БАТАРЕЯ
    &ppInformation,                         ///< СЕРВИС - ИНФОРМАЦИЯ
    PageService::PageDebug::pointer,        ///< СЕРВИС - ОТЛАДКА
    Page::Name::Service, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
