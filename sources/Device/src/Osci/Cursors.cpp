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
pString Cursors::Voltage(Chan::E source, int numCur, char buffer[20])
{
    float voltage = FPGAMath::VoltageCursor(Cursors::PosU(source, numCur), SET_RANGE(source), SET_RSHIFT(source));
    if (SET_DIVIDER_10(source))
    {
        voltage *= 10.0f;
    }
    return ::Voltage(voltage).ToString(true, buffer);
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
        bool bothCursors = CURsT_ENABLED && CURsU_ENABLED;  // Признак того, что включены и вертикальные и горизонтальные курсоры - надо нарисовать 
                                                            // квадраты в местах пересечения

        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = Grid::Left() + (int)CURsT_POS(source, 0);
            x1 = Grid::Left() + (int)CURsT_POS(source, 1);
            y0 = GRID_TOP + (int)sCursors_GetCursPosU(source, 0);
            y1 = GRID_TOP + (int)sCursors_GetCursPosU(source, 1);

            Painter::DrawRectangle(x0 - 2, y0 - 2, 4, 4);
            Painter::DrawRectangle(x1 - 2, y1 - 2, 4, 4);
        }

        if (CURsT_ENABLED)
        {
            DrawVerticalCursor((int)CURsT_POS(source, 0), y0);
            DrawVerticalCursor((int)CURsT_POS(source, 1), y1);
        }
        if (CURsU_ENABLED)
        {
            DrawHorizontalCursor((int)sCursors_GetCursPosU(source, 0), x0);
            DrawHorizontalCursor((int)sCursors_GetCursPosU(source, 1), x1);
        }

        UpdateCursorsForLook();
    }
}
