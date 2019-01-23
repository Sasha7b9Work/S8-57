#include "defines.h"
#include "Cursors.h"
#include "Display/Colors.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA_Math.h"
#include "Menu/Menu.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstring>


using namespace Display::Primitives;
using namespace FPGA::Math;
using namespace Osci::Processing;
using namespace Osci::Measurements;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Нарисовать вертикальный курсор
static void DrawVertical(int x, int yTearing);
/// Нарисовать горизонтальный курсор
static void DrawHorizontal(int y, int xTearing);

static void UpdateCursorsForLook();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Cursors::PosU(Chan::E ch, int numCur)
{
    return CURsU_POS(ch, numCur) / (Grid::Bottom() == Grid::FullBottom() ? 1.0F : 2.0F);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cursors::NecessaryDraw()
{
    return (CURsU_ENABLED || CURsT_ENABLED) && (CURS_SHOW || Menu::GetNameOpenedPage() == Page::Name::Measures_Cursors_Set);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::Voltage(Chan::E source, int numCur)
{
    float voltage = FPGA::Math::VoltageCursor(Cursors::PosU(source, numCur), SET_RANGE(source), SET_RSHIFT(source));
    if (SET_DIVIDER_10(source))
    {
        voltage *= 10.0F;
    }

    return ::Voltage(voltage).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::Time(Chan::E source, int numCur)
{
    float time = FPGA::Math::TimeCursor(CURsT_POS(source, numCur), SET_TBASE);

    return ::Time(time).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Cursors::PosT(Chan::E ch, int num)
{
    float retValue = 0.0F;
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

    Color::SetCurrent(Color::Cursors(source));

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
            y0 = Grid::Top() + (int)CURsU_POS(source, 0);
            y1 = Grid::Top() + (int)CURsU_POS(source, 1);

            Rectangle(4, 4).Draw(x0 - 2, y0 - 2);
            Rectangle(4, 4).Draw(x1 - 2, y1 - 2);
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
        //Painter::DrawDashedVLine(x, Grid::Top() + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
        DashedVLine(Grid::ChannelBottom() - Grid::Top() - 3, 1, 1, 0).Draw(x, Grid::Top() + 2);
    }
    else
    {
        //Painter::DrawDashedVLine(x, Grid::Top() + 2, yTearing - 2, 1, 1, 0);
        DashedVLine(yTearing - Grid::Top() - 4, 1, 1, 0).Draw(x, Grid::Top() + 2);
        //Painter::DrawDashedVLine(x, yTearing + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
        DashedVLine(Grid::ChannelBottom() - yTearing - 3, 1, 1, 0).Draw(x, yTearing + 2);
    }
    Rectangle(2, 2).Draw(x - 1, Grid::Top() - 1);
    Rectangle(2, 2).Draw(x - 1, Grid::ChannelBottom() - 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHorizontal(int y, int xTearing)
{
    y += Grid::Top();
    if (xTearing == -1)
    {
        //Painter::DrawDashedHLine(y, Grid::Left() + 2, Grid::Right() - 1, 1, 1, 0);
        DashedHLine(Grid::Right() - Grid::Left() - 3, 1, 1, 0).Draw(Grid::Left() + 2, y);
    }
    else
    {
        //Painter::DrawDashedHLine(y, Grid::Left() + 2, xTearing - 2, 1, 1, 0);
        DashedHLine(xTearing - Grid::Left() - 4, 1, 1, 0).Draw(Grid::Left() + 2, y);
        //Painter::DrawDashedHLine(y, xTearing + 2, Grid::Right() - 1, 1, 1, 0);
        DashedHLine(Grid::Right() - xTearing - 3, 1, 1, 0).Draw(xTearing + 2, y);
    }
    Rectangle(2, 2).Draw(Grid::Left() - 1, y - 1);
    Rectangle(2, 2).Draw(Grid::Right() - 1, y - 1);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::PercentsU(Chan::E source)
{
    /// \todo Тут дикая дичь. Эта строчка вызывает HardFault. Возможно, из-за включенного выравнивания Settings. Надо подумать
    // float dPerc = dUperc(source);     
    float dPerc = 100.0F;
    std::memcpy(&dPerc, &dUperc(source), sizeof(float));

    float dValue = std::fabsf(PosU(source, 0) - PosU(source, 1));
    return String("%s%%", Float(dValue / dPerc * 100.0F).ToString(false, 5).CString());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Cursors::PercentsT(Chan::E source)
{
    float dPerc = 100.0F;
    std::memcpy(&dPerc, &dTperc(source), sizeof(float));

    float dValue = std::fabsf(CURsT_POS(source, 0) - CURsT_POS(source, 1));
    return String("%s%%", Float(dValue / dPerc * 100.0F).ToString(false, 6).CString());
}
