#include "defines.h"
#include "BottomPart.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Grid.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"
#include "Utils/Values.h"
#include <cmath>


using namespace Display::Primitives;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Cursors;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::BottomPart::Draw()
{
    //WriteCursors();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Display::BottomPart::WriteCursors()
{
    const int x0 = 43 + 29;
    const int y0 = 0;

    int x = x0;
    int y1 = y0;
    int y2 = y0 + 9;
    if(Cursor::NecessaryDraw())
    {
        //Painter::DrawVLine(x, 1, Grid::Top() - 2, Color::FILL);
        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 3;
        Chan::E source = CURS_SOURCE;
        Color colorText = Color::Channel(source);
        if (CURsU_ENABLED)
        {
            String("1:").Draw(x, y1, colorText);
            String("2:").Draw(x, y2);
            x += 7;
            Cursor::Voltage(source, 0).Draw(x, y1);
            Cursor::Voltage(source, 1).Draw(x, y2);
            x = x0 + 49;
            float pos0 = FPGA::Math::VoltageCursor(Cursor::PosU(source, 0), SET_RANGE(source), SET_RSHIFT(source));
            float pos1 = FPGA::Math::VoltageCursor(Cursor::PosU(source, 1), SET_RANGE(source), SET_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0);
            if(SET_DIVIDER_10(source))
            {
                delta *= 10;
            }
            String(":dU=").Draw(x, y1);
            Voltage(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            Cursor::PercentsU(source).Draw(x + 10, y2);
        }

        x = x0 + 101;

        //Painter::DrawVLine(x, 1, Grid::Top() - 2, Color::FILL);
        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 3;
        if(CURsT_ENABLED)
        {
            Color::SetCurrent(colorText);
            String("1:").Draw(x, y1);
            String("2:").Draw(x, y2);
            x += 7;
            Cursor::Time(source, 0).Draw(x, y1);
            Cursor::Time(source, 1).Draw(x, y2);
            x = x0 + 153;          

            float pos0 = FPGA::Math::TimeCursor(CURsT_POS(source, 0), SET_TBASE);
            float pos1 = FPGA::Math::TimeCursor(CURsT_POS(source, 1), SET_TBASE);
            float delta = std::fabsf(pos1 - pos0);
            String(":dT=").Draw(x, y1);
            Time(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            Cursor::PercentsT(source).Draw(x + 8, y2);

            if(CURSORS_SHOW_FREQ)
            {
                int width = 65;
                x = Grid::Right() - width;
                Rectangle(width, 12).Draw(x, Grid::Top(), Color::FILL);
                Region(width - 2, 10).Fill(x + 1, Grid::Top() + 1, Color::BACK);
                String("1/dT=").Draw(x + 1, Grid::Top() + 2, colorText);
                if (delta != 0.0F) //-V550
                {
                    Frequency(1.0F / delta).ToString().Draw(x + 25, Grid::Top() + 2);
                }
            }
        }
    }
}
