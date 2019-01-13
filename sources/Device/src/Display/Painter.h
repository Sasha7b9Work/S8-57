#pragma once
#include "Colors.h"
#include "Display_Types.h"
#include "Display/Font/Font.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Painter
{
    void Init();

    void BeginScene(Color color);

    void EndScene();

    void SetFont(Font::Type::E typeFont);
    /// Установить яркость дисплея.
    void SetBrightnessDisplay(int16 brightness);
    /// Нарисовать массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат.
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// \brief Нарисовать numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    /// первой точки каждой линии соответствует очередному элементу массива x[]
    void DrawMultiVPointLine(int numLines, int y, const uint16 *x, int delta, int count, Color color = Color::NUMBER);

    void DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y);

    void SaveScreenToDrive();
};
