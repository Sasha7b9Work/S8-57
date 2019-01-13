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

    int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);
    /// ѕишет строку текста в центре области(x, y, width, height)цветом ColorText на пр€моугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder,
                                                    Color colorBackground);
    /// ”становить €ркость диспле€.
    void SetBrightnessDisplay(int16 brightness);

    void Draw10SymbolsInRect(int x, int y, char eChar);
    /// Ќарисовать массив вертикальных линий. Ћинии рисуютс€ одна за другой. y0y1 - массив вертикальных координат.
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// \brief Ќарисовать numLines вертикальных линий, состо€щих из count точек кажда€ с рассто€нием между точками delta. √оризонтальна€ координата
    /// первой точки каждой линии соответствует очередному элементу массива x[]
    void DrawMultiVPointLine(int numLines, int y, const uint16 *x, int delta, int count, Color color = Color::NUMBER);
    /// \brief Ќарисовать numLines горизонтальных линий, состо€щих из count точек кажда€ с рассто€нием между точками delta. ¬ертикальна€ координата
    /// первой точки каждой линии соответствует очередному элементу массива y[]
    void DrawMultiHPointLine(int numLines, int x, const uint8 *y, int delta, int count, Color color = Color::NUMBER);
    /// Ќарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    void DrawHPointLine(int y, int x0, int x1, float delta);
    /// Ќарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    void DrawVPointLine(int x, int y0, int y1, float delta);

    void DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y);

    void DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart);

    void DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart);

    void SaveScreenToDrive();
};
