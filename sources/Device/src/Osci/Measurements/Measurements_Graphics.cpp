#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Osci/Measurements/Measurements_Graphics.h"
#include "Osci/Measurements/Processing.h"
#include "Settings/Settings.h"

#include "Utils/Math.h"
#include "Display/Font/Font.h"


using namespace Osci::Measurements;
using namespace Osci::Measurements::Settings;

using Display::Primitives::Char;
using Display::Primitives::Rectangle;
using Display::Primitives::Region;
using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Верхняя координата y выводимой таблицы автоматических измерений
static int top = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Нарисовать страницу выбора измерений
static void DrawPageChoice();

static int GetTopTable();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Graphics::Draw()
{
    if (!SHOW_MEASURES)
    {
        return;
    }

    top = Grid::Bottom();

    Osci::Processing::CalculateMeasures();

    int x0 = Grid::Left() - GetDeltaGridLeft();
    int dX = DX();
    int dY = DY();
    int y0 = GetTopTable();

    int numRows = NumRows();
    int numCols = NumCols();

    for (int str = 0; str < numRows; str++)
    {
        for (int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            Measure measure = Measure::Get(str, elem);

            bool active = measure.IsActive() && Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune;

            Measure::Type::E type = measure.GetType();
            if (type != Measure::Type::None)
            {
                Region(dX, dY).Fill(x, y, Color::BACK);
                Rectangle(dX, dY).Draw(x, y, Color::FILL);
                top = Math::Min(top, y);
            }
            if (active)
            {
                Region(dX - 4, dY - 4).Fill(x + 2, y + 2, Color::FILL);
            }
            if (type != Measure::Type::None)
            {
                Color color = active ? Color::BACK : Color::FILL;

#define SIZE_BUFFER 20
                char buffer[SIZE_BUFFER];

                measure.Name().Draw(x + 4, y + 2, color);

                if (type == MEAS_MARKED)
                {
                    Region(dX - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    measure.Name().Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }
                if (VIEW_MEASURES_BOTH)
                {
                    Processing::GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::A));
                    Processing::GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER).Draw(x + 2, y + (SET_ENABLED_A ? 20 : 11), Color::Channel(Chan::B));
                }
                else if (VIEW_MEASURES_A)
                {
                    Processing::GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::A));
                }
                else if (VIEW_MEASURES_B)
                {
                    Processing::GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER).Draw(x + 2, y + 11, Color::Channel(Chan::B));
                }
                else
                {
                    // других вариантов нет
                }
            }
        }
    }

    if (Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune)
    {
        DrawPageChoice();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetTopTable()
{
    if (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2)
    {
        return Grid::Bottom() - Osci::Measurements::Graphics::DY() * 6;
    }

    int y = Grid::Bottom() - Graphics::NumRows() * Osci::Measurements::Graphics::DY();

    if (Menu::IsShown())
    {
        y -= 3;
    }

    return y;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPageChoice()
{
    if (!Measure::pageChoiceIsActive)
    {
        return;
    }
    int x = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? (Grid::Right() - 3 * Grid::Width() / 5) : Grid::Left();
    int y = Grid::Top();
    int dX = Grid::Width() / 5;
    int dY = 22;
    int maxRow = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 8 : 5;
    int maxCol = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 3 : 5;
    Measure::Type::E meas = Measure::Type::None;
    Font::SetCurrent(Font::Type::_UGO);
    for (int row = 0; row < maxRow; row++)
    {
        for (int col = 0; col < maxCol; col++)
        {
            if (meas >= Measure::Type::Number)
            {
                break;
            }
            int x0 = x + col * dX;
            int y0 = y + row * dY;
            bool active = (meas == Measure::posOnPageChoice);
            Rectangle(dX, dY).Draw(x0, y0, Color::WHITE);
            Region(dX - 2, dY - 2).Fill(x0 + 1, y0 + 1, (active ? Color::FLASH_10 : Color::BACK));
            Color::SetCurrent(active ? Color::FLASH_01 : Color::FILL);
            Char(Measure::GetChar(meas)).Draw10SymbolsInRect(x0 + 2, y0 + 1);
            Font::SetCurrent(Font::Type::_5);
            Text(Measure::Get(row, col).Name()).DrawRelativelyRight(x0 + dX, y0 + 12, active ? Color::FLASH_01 : Color::FILL);
            Font::SetCurrent(Font::Type::_UGO);
            meas = (Measure::Type::E)((int)meas + 1);    // meas++;
        }
    }
    Font::SetCurrent(Font::Type::_8);
}
