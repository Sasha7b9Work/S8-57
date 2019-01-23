#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Hardware/Sound.h"
#include <cmath>


using namespace Osci::Measurements;

using Osci::Measurements::Cursors::Cursor;
using Osci::Measurements::Measure;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSet;

const float PageMeasures::PageCursors::PageSet::MAX_POS_U = 200.0F;
const float PageMeasures::PageCursors::PageSet::MAX_POS_T = 280.0F;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_Set_ChannelA(int x, int y)
{
    String("1").Draw(x + 7, y + 5);
}

static void Draw_Set_ChannelB(int x, int y)
{
    String("2").Draw(x + 7, y + 5);
}

void PageMeasures::PageCursors::PageSet::OnPress_Set_Channel()
{
    Chan::E source = CURS_SOURCE_A ? Chan::B : Chan::A;
    SetCursSource(source);
}

void PageMeasures::PageCursors::PageSet::Draw_Set_Channel(int x, int y)
{
    static const pFuncVII func[2] = {Draw_Set_ChannelA, Draw_Set_ChannelB};
    func[CURS_SOURCE](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(bSet_Channel,                                                                                                     //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Канал ---
    "Канал", "Chan",
    "Выбор канала для курсорных измерений",
    "Chan choice for measurements",
    pageSet, FuncActive, PageMeasures::PageCursors::PageSet::OnPress_Set_Channel, PageMeasures::PageCursors::PageSet::Draw_Set_Channel,
    Draw_Set_ChannelA, {"канал 1", "channel 1"},
    Draw_Set_ChannelB, {"канал 2", "channel 2"}
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_T_disable(int x, int y)
{
    String("T").Draw(x + 7, y + 5);
}

static void Draw_Set_T_disableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, false, false);
}

static void Draw_Set_T_enableLeft(int x, int y)
{
    DrawMenuCursTime(x, y, true, false);
}

static void Draw_Set_T_enableRight(int x, int y)
{
    DrawMenuCursTime(x, y, false, true);
}

static void Draw_Set_T_enableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, true, true);
}

void PageMeasures::PageCursors::PageSet::OnPress_Set_T()
{
    if (CURS_ACTIVE_T || CURsT_DISABLED)
    {
        IncCursCntrlT(CURS_SOURCE);
    }
    CURS_ACTIVE = Cursors::Active::T;
}

static void Draw_Set_T(int x, int y)
{
    if (CURsT_DISABLED)
    {
        Draw_Set_T_disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_T)
        {
            Draw_Set_T_disableBoth(x, y);
        }
        else
        {
            bool condLeft = false, condDown = false;
            Chan::E source = CURS_SOURCE;
            CalculateConditions((int16)CURsT_POS(source, 0), (int16)CURsT_POS(source, 1), CURsT_CNTRL, &condLeft, &condDown);
            if (condLeft && condDown)
            {
                Draw_Set_T_enableBoth(x, y);
            }
            else if (condLeft)
            {
                Draw_Set_T_enableLeft(x, y);
            }
            else
            {
                Draw_Set_T_enableRight(x, y);
            }
        }
    }
}

DEF_SMALL_BUTTON_HINTS_5(bSet_T,                                                                                                       //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Курсоры Т ---
    "Курсоры T", "Cursors T",
    "Выбор курсоров времени для индикации и управления",
    "Choice of cursors of time for indication and management",
    pageSet, FuncActive, PageMeasures::PageCursors::PageSet::OnPress_Set_T, Draw_Set_T,
    Draw_Set_T_disable,     {"курсоры времени выключены",                             "cursors of time are switched off"},
    Draw_Set_T_disableBoth, {"курсоры времени включены",                              "cursors of time are switched on"},
    Draw_Set_T_enableLeft,  {"курсоры времени включены, управление левым курсором",   "cursors of time are switched on, control of the left cursor"},
    Draw_Set_T_enableRight, {"курсоры времени включены, управление правым курсором",  "cursors of time are switched on, control of the right cursor"},
    Draw_Set_T_enableBoth,  {"курсоры времени включены, управление обоими курсорами", "cursors of time are switched on, control of both cursors"}
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_U_disable(int x, int y)
{
    String('U').Draw(x + 7, y + 5);
}

static void Draw_Set_U_disableBoth(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, false);
}

static void Draw_Set_U_enableUpper(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, false);
}

static void Draw_Set_U_enableLower(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, true);
}

static void Draw_Set_U_enableBoth(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, true);
}

void PageMeasures::PageCursors::PageSet::OnPress_Set_U()
{
    if (CURS_ACTIVE_U || CURsU_DISABLED)
    {
        IncCursCntrlU(CURS_SOURCE);
    }
    CURS_ACTIVE = Cursors::Active::U;
}

static void Draw_Set_U(int x, int y)
{
    Chan::E source = CURS_SOURCE;
    if (CURsU_DISABLED)
    {
        Draw_Set_U_disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_U)
        {
            Draw_Set_U_disableBoth(x, y);
        }
        else
        {
            bool condTop = false, condDown = false;
            CalculateConditions((int16)Cursor::PosU(source, 0), (int16)Cursor::PosU(source, 1), CURsU_CNTRL, &condTop, &condDown);
            if (condTop && condDown)
            {
                Draw_Set_U_enableBoth(x, y);
            }
            else if (condTop)
            {
                Draw_Set_U_enableUpper(x, y);
            }
            else
            {
                Draw_Set_U_enableLower(x, y);
            }
        }
    }
}

// Выбор курсора напряжения - курсор 1, курсор 2, оба курсора или отключены.
DEF_SMALL_BUTTON_HINTS_5(bSet_U,                                                                                                       //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Курсоры U ---
    "Курсоры U", "Cursors U",
    "Выбор курсоров напряжения для индикации и управления",
    "Choice of cursors of voltage for indication and management",
    pageSet, FuncActive, PageMeasures::PageCursors::PageSet::OnPress_Set_U, Draw_Set_U,
    Draw_Set_U_disable, {"курсоры напряжения выключены",
                            "cursors of tension are switched off"},
    Draw_Set_U_disableBoth, {"курсоры напряжения включены",
                            "cursors of tension are switched on"},
    Draw_Set_U_enableUpper, {"курсоры напряжения включены, управление верхним курсором",
                            "cursors of tension are switched on, control of the top cursor"},
    Draw_Set_U_enableLower, {"курсоры напряжения включены, управление нижним курсором",
                            "cursors of tension are switched on, control of the lower cursor"},
    Draw_Set_U_enableBoth, {"курсоры напряжения включены, управление обоими курсорами",
                            "cursors of tension are switched on, control of both cursors"}
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Set_100()
{
    PageMeasures::PageCursors::PageSet::SetCursPos100(CURS_SOURCE);
}

static void Draw_Set_100(int x, int y)
{
    Font::SetCurrent(Font::Type::_5);
    String("100%").Draw(x + 2, y + 3);
    Font::SetCurrent(Font::Type::_8);
}

// Установка 100 процентов в текущие места курсоров.
DEF_SMALL_BUTTON(bSet_100,                                                                                                                  //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - 100% ---
    "100%", "100%",
    "Используется для процентных измерений. Нажатие помечает расстояние между активными курсорами как 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    pageSet, FuncActive, OnPress_Set_100, Draw_Set_100
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_Movement_Percents(int x, int y)
{
    String('\x83').Draw(x + 6, y + 5);
}

static void Draw_Set_Movement_Points(int x, int y)
{
    Font::SetCurrent(Font::Type::_5);
    String("тчк").Draw(x + 4, y + 3);
    Font::SetCurrent(Font::Type::_8);
}

static void OnPress_Set_Movement()
{
    Math::CircleIncrease<int8>((int8 *)&CURS_MOVEMENT, 0, 1);
}

static void Draw_Set_Movement(int x, int y)
{
    if (CURS_MOVEMENT_IN_PERCENTS)
    {
        Draw_Set_Movement_Percents(x, y);
    }
    else
    {
        Draw_Set_Movement_Points(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2(bSet_Movement,                                                                                              //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Перемещение ---
    "Перемещение", "Movement",
    "Выбор шага перемещения курсоров - проценты или точки",
    "Choice of a step of movement of cursors - percent or points",
    pageSet, FuncActive, OnPress_Set_Movement, Draw_Set_Movement,
    Draw_Set_Movement_Percents, {"шаг перемещения курсоров кратен одному проценту", "the step of movement of cursors is multiple to one percent"},
    Draw_Set_Movement_Points, {"шаг перемещения курсора кратен одному пикселю",   "the step of movement of the cursor is multiple to one pixel"}
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PageMeasures::PageCursors::PageSet::OnKey(KeyEvent event) //-V2506
{
    TypePress::E type = event.type;

    if (type == TypePress::Release || type == TypePress::Long)
    {
        return true;
    }

    Key::E key = event.key;

    float value = event.IsAboveZero() ? 1.0F : -1.0F;

    if (CURS_ACTIVE_U && (key == Key::Up || key == Key::Down))
    {
        if (CURS_MOVEMENT_IN_PERCENTS)
        {
            value *= dUperc(CURS_SOURCE) / 100.0F;
        }

        if (CURsU_CNTRL_1 || CURsU_CNTRL_1_2)
        {
            SetShiftCursPosU(CURS_SOURCE, 0, value);
        }
        if (CURsU_CNTRL_2 || CURsU_CNTRL_1_2)
        {
            SetShiftCursPosU(CURS_SOURCE, 1, value);
        }
        UpdateCursorsForLook();
    }
    else if(CURS_ACTIVE_T && (key == Key::Left || key == Key::Right))
    {
        if (CURS_MOVEMENT_IN_PERCENTS)
        {
            value *= dTperc(CURS_SOURCE) / 100.0F;
        }

        if (CURsT_CNTRL_1 || CURsT_CNTRL_1_2)
        {
            SetShiftCursPosT(CURS_SOURCE, 0, value);
        }
        if (CURsT_CNTRL_2 || CURsT_CNTRL_1_2)
        {
            SetShiftCursPosT(CURS_SOURCE, 1, value);
        }
        UpdateCursorsForLook();
    }
    else
    {
        // ничего не делаем
    }

    Sound::RegulatorShiftRotate();

    return true;
}

DEF_PAGE_4( pageSet, // -V641 // -V1027                                                                                                            //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ ---
    "УСТАНОВИТЬ", "SET",
    "Переход в режим курсорных измерений",
    "Switch to cursor measures",
    &bSet_Channel,          ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Канал
    &bSet_U,                ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Курсоры U
    &bSet_T,                ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Курсоры Т
    &bSet_100,              ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - 100%
    //&bSet_Movement,         ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Перемещение
    Page::Name::Measures_Cursors_Set, PageMeasures::PageCursors::pointer, FuncActive, FuncPressPage, FuncDrawPage,
    PageMeasures::PageCursors::PageSet::OnKey
)



const PageBase *PageMeasures::PageCursors::PageSet::pointer = &pageSet;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursSource(Chan::E ch)
{
    CURS_SOURCE = ch;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::IncCursCntrlU(Chan::E ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURsU_CNTRL_CH(ch), 0, 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::IncCursCntrlT(Chan::E ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURsT_CNTRL_CH(ch), 0, 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursPos100(Chan::E ch)
{
    dUperc(ch) = (float)std::fabsf(CURsU_POS(ch, 0) - CURsU_POS(ch, 1));
    dTperc(ch) = (float)std::fabsf(CURsT_POS(ch, 0) - CURsT_POS(ch, 1));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetShiftCursPosU(Chan::E ch, int numCur, float delta)
{
    CURsU_POS(ch, numCur) = Math::LimitationRet(CURsU_POS(ch, numCur) - delta, 0.0F, MAX_POS_U);

    if (CURS_MOVEMENT_IN_PIXELS)                        // Если перемещение по пикселям, то нужно привести к пиксельной сетке экрана
    {
        /// \todo
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetShiftCursPosT(Chan::E ch, int numCur, float delta)
{
    /// \todo одинаковые ветки
    // CURsT_POS(ch, numCur) = LimitationFloat(CURsT_POS(ch, numCur) + delta, 0, MAX_POS_T);   
    Cursor::SetCursPosT_temp(ch, numCur, Math::LimitationRet(CURsT_POS(ch, numCur) + delta, 0.0F, MAX_POS_T));

    if (CURS_MOVEMENT_IN_PIXELS)                        // Если перемещение по пикселям, то нужно привести к пиксельной сетке экрана
    {
        /// \todo
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::UpdateCursorsForLook()
{
    Chan::E source = CURS_SOURCE;

    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        SetCursorU(source, 0, Measure::CalculateCursorU(source, CURsT_POS(source, 0)));
    }
    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        SetCursorU(source, 1, Measure::CalculateCursorU(source, CURsT_POS(source, 1)));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        SetCursorT(source, 0, Measure::CalculateCursorT(source, CURsU_POS(source, 0), 0));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        SetCursorT(source, 1, Measure::CalculateCursorT(source, CURsU_POS(source, 1), 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursorU(Chan::E ch, int numCur, float pos)
{
    CURsU_POS(ch, numCur) = Math::LimitationRet(pos, 0.0F, MAX_POS_U);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursorT(Chan::E ch, int numCur, float pos)
{
    /// \todo одинаковые ветки
    // CURsT_POS(ch, numCur) = LimitationFloat(pos, 0, MAX_POS_T);      
    Cursor::SetCursPosT_temp(ch, numCur, Math::LimitationRet(pos, 0.0F, MAX_POS_T));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PageMeasures::PageCursors::PageSet::IsRegSetActiveOnCursors()
{
    return ((Menu::GetNameOpenedPage() == Page::Name::Measures_Cursors_Set) &&
        ((CURS_ACTIVE_U && CURsU_ENABLED) || (CURS_ACTIVE_T && CURsT_ENABLED)));
}
