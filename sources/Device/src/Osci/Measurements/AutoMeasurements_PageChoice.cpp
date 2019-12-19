#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"


static bool isActive = false;
/// ѕозици€ курсора
static int8 posCursor = 0;


void PageChoiceMeasures::ChangeState()
{
    isActive = !isActive;
}


void PageChoiceMeasures::OnOpenCloseEvent()
{
    ChangeState();
    if (isActive)
    {
        posCursor = static_cast<int8>(set.meas.measures[AutoMeasurements::posActive]);
    }
}


void PageChoiceMeasures::OnKeyEvent(const KeyEvent &event)
{
    if (event.type != TypePress::Press)
    {
        return;
    }

    int8 delta = (event.IsUp() || event.IsRight()) ? 1 : -1;

    if (isActive)
    {
        posCursor += delta;
        Beeper::RegulatorSwitchRotate();

        if (posCursor < 0)
        {
            posCursor = TypeMeasure::Count - 1;
        }
        else if (posCursor == TypeMeasure::Count)
        {
            posCursor = 0;
        }
        else
        {
            // здесь ничего делать не нужно
        }

        set.meas.measures[AutoMeasurements::posActive] = static_cast<TypeMeasure::E>(posCursor);
        Color::ChangeFlash(true);
    }
    else
    {
        Measure::ChangeActive(delta);

        Beeper::RegulatorSwitchRotate();
    }
}


void PageChoiceMeasures::Draw()
{
    if (!isActive)
    {
        return;
    }
    int x = MeasuresOnDisplay::IsVertical() ? (Grid::Right() - 3 * Grid::Width() / 5) : Grid::Left();
    int y = Grid::Top();
    int dX = Grid::Width() / 5;
    int dY = 22;
    int maxRow = MeasuresOnDisplay::IsVertical() ? 8 : 5;
    int maxCol = MeasuresOnDisplay::IsVertical() ? 3 : 5;

    TypeMeasure::E meas = TypeMeasure::None;

    for (int row = 0; row < maxRow; row++)
    {
        for (int col = 0; col < maxCol; col++)
        {
            if (meas < TypeMeasure::Count)
            {
                int x0 = x + col * dX;
                int y0 = y + row * dY;
                bool active = (meas == posCursor);
                Rectangle(dX, dY).Draw(x0, y0, Color::WHITE);
                Region(dX - 2, dY - 2).Fill(x0 + 1, y0 + 1, (active ? Color::FLASH_10 : Color::BACK));
                Char(static_cast<SymbolUGO::E>(Measure::GetChar(meas))).Draw10SymbolsInRect(x0 + 2, y0 + 1, active ? Color::FLASH_01 : Color::FILL);
                DFont::Set(DTypeFont::_5);
                Text(TypeMeasure::GetName(meas)).DrawRelativelyRight(x0 + dX, y0 + 12);
                meas = static_cast<TypeMeasure::E>(static_cast<int>(meas) + 1);    // meas++;
            }
        }
    }

    DFont::Set(DTypeFont::_8);
}
