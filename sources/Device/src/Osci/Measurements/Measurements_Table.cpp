#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Osci/Measurements/Measurements_Table.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Display/Font/Font.h"
#include "Osci/Measurements/Measurements.h"


using namespace Osci;
using namespace Osci::Measurements;

using Display::Primitives::Char;
using Display::Primitives::Rectangle;
using Display::Primitives::Region;
using Display::Primitives::Text;



/// Верхняя координата y выводимой таблицы автоматических измерений
static int top = 0;



static int GetTopTable();



void Table::Draw()
{
    if (!set.meas.show)
    {
        return;
    }

    top = Grid::Bottom();

    CalculateMeasures();

    for (int str = 0; str < NumRows(); str++)
    {
        for (int elem = 0; elem < NumCols(); elem++)
        {
            int x = Grid::Left() - GetDeltaGridLeft() + DX() * elem;
            int y = GetTopTable() + str * DY();

            Cell(str, elem).Draw(x, y);
        }
    }

    if (Menu::GetNameOpenedPage() == PageName::AutoMeasures_Tune)
    {
        PageChoice::Draw();
    }
}


void Table::Cell::Draw(int x, int y)
{
    Measure measure(row, col);

    bool active = measure.IsActive() && Menu::GetNameOpenedPage() == PageName::AutoMeasures_Tune;

    Measure::Type::E type = measure.GetType();

    if (type != Measure::Type::None)
    {
        Region(DX(), DY()).Fill(x, y, Color::BACK);
        Rectangle(DX(), DY()).Draw(x, y, Color::FILL);
        top = Math::Min(top, y);
    }

    if (active)
    {
        Region(DX() - 4, DY() - 4).Fill(x + 2, y + 2, Color::FILL);
    }

    if (type != Measure::Type::None)
    {
        Color color = active ? Color::BACK : Color::FILL;

        measure.Name().Draw(x + 4, y + 2, color);

        if (type == set.meas.marked)
        {
            Region(DX() - 2, 9).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
            measure.Name().Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
        }

        DrawStringMeasure(x, y);   
    }
}


void Table::Cell::DrawStringMeasure(int x, int y)
{
    Measure measure(row, col);

    static const int SIZE_BUFFER = 20;

    char buffer[SIZE_BUFFER];

    String measureA = measure.GetStringMeasure(Chan::A, buffer, SIZE_BUFFER);
    String measureB = measure.GetStringMeasure(Chan::B, buffer, SIZE_BUFFER);

    Color colA = Color::CHAN[Chan::A];
    Color colB = Color::CHAN[Chan::B];

    if (set.meas.source == MeasuresSource::A_B)
    {
        measureA.Draw(x + 2, y + 11, colA);
        measureB.Draw(x + 2, y + (set.ch[Chan::A].enabled ? 20 : 11), colB); //-V547
    }
    else if (set.meas.source == MeasuresSource::A)
    {
        measureA.Draw(x + 2, y + 11, colA);
    }
    else if (set.meas.source == MeasuresSource::B)
    {
        measureB.Draw(x + 2, y + 11, colB);
    }
    else
    {
        // других вариантов нет
    }
}


static int GetTopTable()
{
    if ((set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2))
    {
        return Grid::Bottom() - Osci::Measurements::Table::DY() * 6;
    }

    int y = Grid::Bottom() - Table::NumRows() * Osci::Measurements::Table::DY();

    if (Menu::IsShown())
    {
        y -= 3;
    }

    return y;
}


int Osci::Measurements::Table::NumCols()
{
    const int cols[] = { 1, 2, 5, 5, 5, 1, 2 };
    return cols[set.meas.number];
}


int Osci::Measurements::Table::NumRows()
{
    int rows[] = { 1, 1, 1, 2, 3, 6, 6 };
    return rows[set.meas.number];
}


int Osci::Measurements::Table::GetDeltaGridLeft()
{
    if (set.meas.show && (set.meas.modeViewSignals == MeasuresModeViewSignals::Compress))
    {
        if (set.meas.number == MeasuresOnDisplay::_6_1)
        {
            return DX();
        }
        else if (set.meas.number == MeasuresOnDisplay::_6_2)
        {
            return DX() * 2;
        }
        else
        {
            // других вариантов нет
        }
    }
    return 0;
}


int Osci::Measurements::Table::DY()
{
    if (set.meas.source == MeasuresSource::A_B)
    {
        return 30;
    }
    return 21;
}


int Osci::Measurements::Table::DX()
{
    return Grid::Width() / 5;
}
