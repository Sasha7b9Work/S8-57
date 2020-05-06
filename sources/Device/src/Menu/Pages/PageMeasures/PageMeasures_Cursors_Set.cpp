#include "defines.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cmath>


const float PageCursorsMeasures::Set::MAX_POS_U = 200.0F;
const float PageCursorsMeasures::Set::MAX_POS_T = 280.0F;


// Рассчитывает условия отрисовки УГО малых кнопок управления выбором курсорами.
void CalculateConditions(int16 pos0, int16 pos1, CursorsControl::E cursCntrl, bool *condTopLeft, bool *condBottomRight)
{
    bool zeroLessFirst = pos0 < pos1;
    *condTopLeft = (cursCntrl == CursorsControl::Both) ||         // если управление двумя курсорами одновременно
        (cursCntrl == CursorsControl::_1 && zeroLessFirst) ||     // или управление первым курсором и позиция первого меньше, чем позиция второго
        (cursCntrl == CursorsControl::_2 && !zeroLessFirst);      // или управление вторым курсором и позиция второго курсора меньше

    *condBottomRight = (cursCntrl == CursorsControl::Both) ||
        (cursCntrl == CursorsControl::_1 && !zeroLessFirst) ||
        (cursCntrl == CursorsControl::_2 && zeroLessFirst);
}


static void Draw_ChannelA(int x, int y)
{
    String("1").Draw(x + 7, y + 5);
}

static void Draw_ChannelB(int x, int y)
{
    String("2").Draw(x + 7, y + 5);
}

void PageCursorsMeasures::Set::OnPress_Channel()
{
    Chan::E source = (set.curs.source == Chan::A) ? ChanB : ChanA;
    SetCursSource(source);
}

void PageCursorsMeasures::Set::Draw_Channel(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    static const pFuncDraw func[2] = {Draw_ChannelA, Draw_ChannelB};
    func[CursorsSource()](x, y);
}

DEF_GRAPH_BUTTON_HINTS_2( bChannel,                                                                                                        //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Канал ---
    "Канал",
    "Выбор канала для курсорных измерений",
    &PageCursorsMeasures::Set::self, Item::Active, PageCursorsMeasures::Set::OnPress_Channel, PageCursorsMeasures::Set::Draw_Channel,
    Draw_ChannelA, "канал 1",
    Draw_ChannelB, "канал 2"
)


void DrawMenuCursTime(int x, int y, bool left, bool right)
{
    x += 11;
    y += 1;

    VLine line(15);

    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        line.Draw(x - 15 + i, y + 1);
    }

    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        line.Draw(x + 15 - i, y + 1);
    }
}

static void Draw_T_disable(int x, int y)
{
    String("T").Draw(x + 7, y + 5);
}

static void Draw_T_disableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, false, false);
}

static void Draw_T_enableLeft(int x, int y)
{
    DrawMenuCursTime(x, y, true, false);
}

static void Draw_T_enableRight(int x, int y)
{
    DrawMenuCursTime(x, y, false, true);
}

static void Draw_T_enableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, true, true);
}

void PageCursorsMeasures::Set::OnPress_T()
{
    if (S_CURS_ACTIVE_IS_T || CursorsControl::IsDisabledT())
    {
        IncCursCntrlT(CursorsSource());
    }

    S_CURS_ACTIVE = CursorsActive::T;
}

static void Draw_T(int x, int y)
{
    if (CursorsControl::IsDisabledT())
    {
        Draw_T_disable(x, y);
    }
    else
    {
        if (!S_CURS_ACTIVE_IS_T)
        {
            Draw_T_disableBoth(x, y);
        }
        else
        {
            bool condLeft = false, condDown = false;
            Chan::E source = CursorsSource();

            CalculateConditions(static_cast<int16>(CursorsMeasurements::PosT(source, 0)), static_cast<int16>(CursorsMeasurements::PosT(source, 1)), 
                CursorsControl::RefT(source), &condLeft, &condDown);

            if (condLeft && condDown)
            {
                Draw_T_enableBoth(x, y);
            }
            else if (condLeft)
            {
                Draw_T_enableLeft(x, y);
            }
            else
            {
                Draw_T_enableRight(x, y);
            }
        }
    }
}

DEF_GRAPH_BUTTON_HINTS_5( bT,                                                                                                          //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Курсоры Т ---
    "Курсоры T",
    "Выбор курсоров времени для индикации и управления",
    &PageCursorsMeasures::Set::self, Item::Active, PageCursorsMeasures::Set::OnPress_T, Draw_T,
    Draw_T_disable,     "курсоры времени выключены",
    Draw_T_disableBoth, "курсоры времени включены",
    Draw_T_enableLeft,  "курсоры времени включены, управление левым курсором",
    Draw_T_enableRight, "курсоры времени включены, управление правым курсором",
    Draw_T_enableBoth,  "курсоры времени включены, управление обоими курсорами"
)


void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 10;
    y += 6;

    HLine line(40);

    for (int i = 0; i < (top ? 3 : 1); i++)
    {
        line.Draw(x, y + i);
    }

    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
        line.Draw(x, y + 8 - i);
    }
}

static void Draw_U_disable(int x, int y)
{
    String('U').Draw(x + 7, y + 5);
}

static void Draw_U_disableBoth(int x, int y)
{
    DrawMenuCursVoltage(x, y, false, false);
}

static void Draw_U_enableUpper(int x, int y)
{
    DrawMenuCursVoltage(x, y, true, false);
}

static void Draw_U_enableLower(int x, int y)
{
    DrawMenuCursVoltage(x, y, false, true);
}

static void Draw_U_enableBoth(int x, int y)
{
    DrawMenuCursVoltage(x, y, true, true);
}

void PageCursorsMeasures::Set::OnPress_U()
{
    if (S_CURS_ACTIVE_IS_U || CursorsControl::IsDisabledU())
    {
        IncCursCntrlU(CursorsSource());
    }

    S_CURS_ACTIVE = CursorsActive::U;
}

static void Draw_U(int x, int y)
{
    Chan::E source = CursorsSource();
    if (CursorsControl::IsDisabledU())
    {
        Draw_U_disable(x, y);
    }
    else
    {
        if (!S_CURS_ACTIVE_IS_U)
        {
            Draw_U_disableBoth(x, y);
        }
        else
        {
            bool condTop = false, condDown = false;

            CalculateConditions(static_cast<int16>(CursorsMeasurements::PosU(source, 0)), static_cast<int16>(CursorsMeasurements::PosU(source, 1)),
                CursorsControl::RefU(source), &condTop, &condDown);

            if (condTop && condDown)
            {
                Draw_U_enableBoth(x, y);
            }
            else if (condTop)
            {
                Draw_U_enableUpper(x, y);
            }
            else
            {
                Draw_U_enableLower(x, y);
            }
        }
    }
}

// Выбор курсора напряжения - курсор 1, курсор 2, оба курсора или отключены.
DEF_GRAPH_BUTTON_HINTS_5( bU,                                                                                                          //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Курсоры U ---
    "Курсоры U",
    "Выбор курсоров напряжения для индикации и управления",
    &PageCursorsMeasures::Set::self, Item::Active, PageCursorsMeasures::Set::OnPress_U, Draw_U,
    Draw_U_disable,     "курсоры напряжения выключены",
    Draw_U_disableBoth, "курсоры напряжения включены",
    Draw_U_enableUpper, "курсоры напряжения включены, управление верхним курсором",
    Draw_U_enableLower, "курсоры напряжения включены, управление нижним курсором",
    Draw_U_enableBoth,  "курсоры напряжения включены, управление обоими курсорами"
)


static void OnPress_100()
{
    PageCursorsMeasures::Set::SetCursPos100(CursorsSource());
}

static void Draw_100(int x, int y)
{
    String("100%%").Draw(x + 1, y + 5);
}

// Установка 100 процентов в текущие места курсоров.
DEF_GRAPH_BUTTON( b100,                                                                                                                     //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - 100% ---
    "100\x83",
    "Используется для процентных измерений. Нажатие помечает расстояние между активными курсорами как 100%",
    &PageCursorsMeasures::Set::self, Item::Active, OnPress_100, Draw_100
)


static void Draw_Movement_Percents(int x, int y)
{
    String('\x83').Draw(x + 6, y + 5);
}

static void Draw_Movement_Points(int x, int y)
{
    String("ТЧК").Draw(x + 2, y + 5);
}

static void OnPress_Movement()
{
    Math::CircleIncrease<int8>(reinterpret_cast<int8 *>(&set.curs.movement), 0, 1);
}

static void Draw_Movement(int x, int y)
{
    if (set.curs.movement == CursorsMovement::Percents)
    {
        Draw_Movement_Percents(x, y);
    }
    else
    {
        Draw_Movement_Points(x, y);
    }
}

DEF_GRAPH_BUTTON_HINTS_2( bMovement,                                                                                                 //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ - Перемещение ---
    "Перемещение",
    "Выбор шага перемещения курсоров - проценты или точки",
    &PageCursorsMeasures::Set::self, Item::Active, OnPress_Movement, Draw_Movement,
    Draw_Movement_Percents, "шаг перемещения курсоров кратен одному проценту",
    Draw_Movement_Points,   "шаг перемещения курсора кратен одному пикселю"
)


bool PageCursorsMeasures::Set::HandlerKey(const KeyEvent &event) //-V2506
{
    TypePress::E type = event.type;

    if (type == TypePress::Release || type == TypePress::Long)
    {
        return false;
    }

    float value = event.IsIncrease() ? 1.0F : -1.0F;

    if (S_CURS_ACTIVE_IS_U && (event.IsArrowUp() || event.IsArrowDown()))
    {
        if (set.curs.movement == CursorsMovement::Percents)
        {
            value *= set.curs.deltaU100percents[CursorsSource()] / 100.0F;
        }

        if (CursorsControl::IsEnabled1U())
        {
            SetShiftCursPosU(CursorsSource(), 0, value);
        }
        if (CursorsControl::IsEnabled2U())
        {
            SetShiftCursPosU(CursorsSource(), 1, value);
        }
        UpdateCursorsForLook();
        
        return true;
    }
    else if(S_CURS_ACTIVE_IS_T && (event.IsArrowLeft() || event.IsArrowRight()))
    {
        if (set.curs.movement == CursorsMovement::Percents)
        {
            value *= set.curs.deltaT100percents[CursorsSource()] / 100.0F;
        }

        if (CursorsControl::IsEnabled1T())
        {
            SetShiftCursPosT(CursorsSource(), 0, value);
        }
        if (CursorsControl::IsEnabled2T())
        {
            SetShiftCursPosT(CursorsSource(), 1, value);
        }
        UpdateCursorsForLook();
        
        return true;
    }
    else
    {
        // ничего не делаем
    }

    Beeper::RegulatorShiftRotate();

    return false;
}

static bool IsActive_Set()
{
    return S_CURS_SHOW;
}

DEF_PAGE_5( pSet,                                                                                                                                  //--- ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ ---
    "УСТАНОВИТЬ",
    "Переход в режим курсорных измерений",
    &bChannel,
    &bU,
    &bT,
    &b100,
    &bMovement,
    PageName::CursorsMeasures_Set, &PageCursorsMeasures::self, IsActive_Set, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, PageCursorsMeasures::Set::HandlerKey
)

const Page * const PageCursorsMeasures::Set::self = static_cast<const Page *>(&pSet);



void PageCursorsMeasures::Set::SetCursSource(Chan::E ch)
{
    set.curs.source = ch;
}


void PageCursorsMeasures::Set::IncCursCntrlU(Chan::E ch)
{
    Math::CircleIncrease<int8>(reinterpret_cast<int8 *>(&CursorsControl::RefU(ch)), 0, 3);
}


void PageCursorsMeasures::Set::IncCursCntrlT(Chan::E ch)
{
    Math::CircleIncrease<int8>(reinterpret_cast<int8 *>(&CursorsControl::RefT(ch)), 0, 3);
}


void PageCursorsMeasures::Set::SetCursPos100(Chan::E ch)
{
    set.curs.deltaU100percents[ch] = static_cast<float>(std::fabsf(set.curs.posCurU[ch][0] - set.curs.posCurU[ch][1]));
    set.curs.deltaT100percents[ch] = static_cast<float>(std::fabsf(CursorsMeasurements::PosT(ch, 0) - CursorsMeasurements::PosT(ch, 1)));
}


void PageCursorsMeasures::Set::SetShiftCursPosU(Chan::E ch, int numCur, float delta)
{
    set.curs.posCurU[ch][numCur] = Math::LimitationRet(set.curs.posCurU[ch][numCur] - delta, 0.0F, MAX_POS_U);

    if (set.curs.movement == CursorsMovement::Pixels)                        // Если перемещение по пикселям, то нужно привести к пиксельной сетке экрана
    {
        // \todo
    }
}


void PageCursorsMeasures::Set::SetShiftCursPosT(Chan::E ch, int numCur, float delta)
{
    // \todo одинаковые ветки
    // CURsT_POS(ch, numCur) = LimitationFloat(CURsT_POS(ch, numCur) + delta, 0, MAX_POS_T);   
    CursorsMeasurements::SetCursPosT_temp(ch, numCur, Math::LimitationRet(CursorsMeasurements::PosT(ch, numCur) + delta, 0.0F, MAX_POS_T));

    if (set.curs.movement == CursorsMovement::Pixels)         // Если перемещение по пикселям, то нужно привести к пиксельной сетке экрана
    {
        // \todo
    }
}


void PageCursorsMeasures::Set::UpdateCursorsForLook()
{
    Chan::E source = CursorsSource();

    if (S_CURS_ACTIVE_IS_T && (S_CURS_LOOK_MODE_IS_VOLTAGE(ChanA) || S_CURS_LOOK_MODE_IS_BOTH(ChanA)))
    {
        SetCursorU(source, 0, Measure::CalculateCursorU(source, CursorsMeasurements::PosT(source, 0)));
    }
    if (S_CURS_ACTIVE_IS_T && (S_CURS_LOOK_MODE_IS_VOLTAGE(ChanB) || S_CURS_LOOK_MODE_IS_BOTH(ChanB)))
    {
        SetCursorU(source, 1, Measure::CalculateCursorU(source, CursorsMeasurements::PosT(source, 1)));
    }
    if (S_CURS_ACTIVE_IS_U && (S_CURS_LOOK_MODE_IS_TIME(ChanA) || S_CURS_LOOK_MODE_IS_BOTH(ChanA)))
    {
        SetCursorT(source, 0, Measure::CalculateCursorT(source, set.curs.posCurU[source][0], 0));
    }
    if (S_CURS_ACTIVE_IS_U && (S_CURS_LOOK_MODE_IS_TIME(ChanB) || S_CURS_LOOK_MODE_IS_BOTH(ChanB)))
    {
        SetCursorT(source, 1, Measure::CalculateCursorT(source, set.curs.posCurU[source][1], 1));
    }
}


void PageCursorsMeasures::Set::SetCursorU(Chan::E ch, int numCur, float pos)
{
    set.curs.posCurU[ch][numCur] = Math::LimitationRet(pos, 0.0F, MAX_POS_U);
}


void PageCursorsMeasures::Set::SetCursorT(Chan::E ch, int numCur, float pos)
{
    // \todo одинаковые ветки
    // CURsT_POS(ch, numCur) = LimitationFloat(pos, 0, MAX_POS_T);      
    CursorsMeasurements::SetCursPosT_temp(ch, numCur, Math::LimitationRet(pos, 0.0F, MAX_POS_T));
}


bool PageCursorsMeasures::Set::IsRegSetActiveOnCursors()
{
    return ((Menu::OpenedItem() == PageCursorsMeasures::Set::self) &&
        ((S_CURS_ACTIVE_IS_U && CursorsControl::IsDisabledU()) ||
        (S_CURS_ACTIVE_IS_T && CursorsControl::IsDisabledT())));
}
