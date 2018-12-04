#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Cursors.h"
#include "Settings/Settings.h"
#include "Display/Colors.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"
#include "FPGA/FPGAMath.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Ќарисовать вертикальный курсор
static void DrawVertical(int x, int yTearing);
/// Ќарисовать горизонтальный курсор
static void DrawHorizontal(int y, int xTearing);

static void UpdateCursorsForLook();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Cursors::PosU(Chan::E ch, int numCur)
{
    return CURsU_POS(ch, numCur) / (Grid::Bottom() == Grid::FullBottom() ? 1.0f : 2.0f);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cursors::NecessaryDraw()
{
    return (CURsU_ENABLED || CURsT_ENABLED) && (CURS_SHOW || Menu::GetNameOpenedPage() == Page::Name::Measures_Cursors_Set);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::Voltage(Chan::E source, int numCur)
{
    float voltage = FPGAMath::VoltageCursor(Cursors::PosU(source, numCur), SET_RANGE(source), SET_RSHIFT(source));
    if (SET_DIVIDER_10(source))
    {
        voltage *= 10.0f;
    }

    return ::Voltage(voltage).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::Time(Chan::E source, int numCur)
{
    char buffer[20];

    return String((char *)Time(source, numCur, buffer));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Cursors::Time(Chan::E source, int numCur, char buffer[20])
{
    float time = FPGAMath::TimeCursor(CURsT_POS(source, numCur), SET_TBASE);

    return ::Time(time).ToString(true, buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Cursors::PosT(Chan::E ch, int num)
{
    float retValue = 0.0f;
    std::memcpy(&retValue, &set.curs_posCurT[ch][num], sizeof(float));
    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cursors::SetCursPosT_temp(Chan::E ch, int num, float value)
{
    std::memcpy(&set.curs_posCurT[ch][num], &value, sizeof(float));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cursors::Draw()
{
    Chan::E source = CURS_SOURCE;

    Painter::SetColor(Color::Cursors(source));

    if (NecessaryDraw())
    {
        bool bothCursors = CURsT_ENABLED && CURsU_ENABLED;  // ѕризнак того, что включены и вертикальные и горизонтальные курсоры - надо нарисовать 
                                                            // квадраты в местах пересечени€

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = Grid::Left() + (int)CURsT_POS(source, 0);
            x1 = Grid::Left() + (int)CURsT_POS(source, 1);
            y0 = Grid::Top() + (int)CURsU_POS(source, 0);
            y1 = Grid::Top() + (int)CURsU_POS(source, 1);

            Painter::DrawRectangle(x0 - 2, y0 - 2, 4, 4);
            Painter::DrawRectangle(x1 - 2, y1 - 2, 4, 4);
        }

        if (CURsT_ENABLED)
        {
            DrawVertical((int)CURsT_POS(source, 0), y0);
            DrawVertical((int)CURsT_POS(source, 1), y1);
        }
        if (CURsU_ENABLED)
        {
            DrawHorizontal((int)CURsU_POS(source, 0), x0);
            DrawHorizontal((int)CURsU_POS(source, 1), x1);
        }

        UpdateCursorsForLook();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawVertical(int x, int yTearing)
{
    x += Grid::Left();
    if (yTearing == -1)
    {
        Painter::DrawDashedVLine(x, Grid::Top() + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedVLine(x, Grid::Top() + 2, yTearing - 2, 1, 1, 0);
        Painter::DrawDashedVLine(x, yTearing + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    Painter::DrawRectangle(x - 1, Grid::Top() - 1, 2, 2);
    Painter::DrawRectangle(x - 1, Grid::ChannelBottom() - 1, 2, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHorizontal(int y, int xTearing)
{
    y += Grid::Top();
    if (xTearing == -1)
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, Grid::Right() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, xTearing - 2, 1, 1, 0);
        Painter::DrawDashedHLine(y, xTearing + 2, Grid::Right() - 1, 1, 1, 0);
    }
    Painter::DrawRectangle(Grid::Left() - 1, y - 1, 2, 2);
    Painter::DrawRectangle(Grid::Right() - 1, y - 1, 2, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void UpdateCursorsForLook()
{
//    Chan::E source = CURS_SOURCE;

    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        //SetCursorU(source, 0, Processing::CalculateCursorU(source, CURsT_POS(source, 0)));
    }
    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        //SetCursorU(source, 1, Processing::CalculateCursorU(source, CURsT_POS(source, 1)));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        //SetCursorT(source, 0, Processing::CalculateCursorT(source, CURsU_POS(source, 0), 0));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        //SetCursorT(source, 1, Processing::CalculateCursorT(source, CURsU_POS(source, 1), 1));
    }
}
