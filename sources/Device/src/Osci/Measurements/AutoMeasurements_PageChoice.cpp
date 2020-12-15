#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"


static bool isActive = false;
// Позиция курсора
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
        posCursor = static_cast<int8>(S_MEAS_INDICATED(AutoMeasurements::posActive));
    }
}


void PageChoiceMeasures::OnKeyEvent(const KeyEvent &event) //-V2506
{
    if (event.type != TypePress::Press)
    {
        return;
    }

    int8 delta = 1;
    if (event.IsArrowLeft())      { delta = -1; }
    else if (event.IsArrowUp())   { delta = -5; }
    else if (event.IsArrowDown()) { delta = 5;  } //-V2516

    if (isActive)
    {
        int prevPos = posCursor;

        posCursor += delta;
        Beeper::RegulatorSwitchRotate();

        if(delta == 1 || delta == -1)
        {
            if (posCursor < 0)
            {
                posCursor = TypeMeasure::Count - 1;
            }
            else if (posCursor >= TypeMeasure::Count) //-V2516
            {
                posCursor = 0;
            }
        }
        else
        {
            if (posCursor < 0)
            {
                if(prevPos == 0)      { posCursor = 19; }
                else if(prevPos == 4) { posCursor = 18; }
                else if(prevPos == 2) { posCursor = 21; }
                else if(prevPos == 1) { posCursor = 20; }
                else                  { posCursor = TypeMeasure::Count - 1; }
            }
            else if (posCursor >= TypeMeasure::Count) //-V2516
            {
                if (prevPos == 20)     { posCursor = 1; }
                else if(prevPos == 21) { posCursor = 2; }
                else if(prevPos == 22) { posCursor = 3; }
                else if(prevPos == 18) { posCursor = 4; }
                else                   { posCursor = 0; }
            }

        }

        S_MEAS_INDICATED(AutoMeasurements::posActive) = static_cast<TypeMeasure::E>(posCursor);
        Color::ChangeFlash(true);
    }
    else
    {
        Measure::ChangeActive(delta);

        Beeper::RegulatorSwitchRotate();
    }
}


void PageChoiceMeasures::Draw() //-V2506
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
