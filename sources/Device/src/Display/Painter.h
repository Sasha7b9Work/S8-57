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

    void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);
    /// Установить яркость дисплея.
    void SetBrightnessDisplay(int16 brightness);

    void Draw10SymbolsInRect(int x, int y, char eChar);
    /// Нарисовать массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат.
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// \brief Нарисовать numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    /// первой точки каждой линии соответствует очередному элементу массива x[]
    void DrawMultiVPointLine(int numLines, int y, const uint16 *x, int delta, int count, Color color = Color::NUMBER);
    /// \brief Нарисовать numLines горизонтальных линий, состоящих из count точек каждая с расстоянием между точками delta. Вертикальная координата
    /// первой точки каждой линии соответствует очередному элементу массива y[]
    void DrawMultiHPointLine(int numLines, int x, const uint8 *y, int delta, int count, Color color = Color::NUMBER);
    /// Нарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    void DrawHPointLine(int y, int x0, int x1, float delta);
    /// Нарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    void DrawVPointLine(int x, int y0, int y1, float delta);

    void DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y);

    void DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart);

    void DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart);

    void SaveScreenToDrive();
};
