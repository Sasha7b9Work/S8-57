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
    /// Елси не передавать значение цвета, то цвет останется прежним
    void SetColor(Color color = Color::NUMBER);

    void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawBigText(int x, int y, uint8 size, const char *text, Color color = Color::NUMBER);

    void SetColorValue(Color color, uint value);

    void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);

    void SetFont(Font::Type::E typeFont);

    void SetPoint(int x, int y, Color color = Color::NUMBER);

    void DrawBoundedRegion(int x, int y, int width, int height, Color colorFill, Color colorBound);

    void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

    int DrawTextWithLimitation(int x, int y, const char *text, int limitX, int limitY, int limitWidth, int limitHeight);

    void FillBoundedRegion(int x, int y, int widht, int height, Color colorFill, Color colorBound);

    void ResetFlash();
    /// Пишет текст с переносами
    int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);
    /// Возвращает нижнюю координату прямоугольника
    int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);
    /// Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder,
                                                    Color colorBackground);
    /// Выводит текст на прямоугольнике цвета colorBackgound
    int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);
    /// Установить яркость дисплея.
    void SetBrightnessDisplay(int16 brightness);

    static int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    void Draw10SymbolsInRect(int x, int y, char eChar);

    void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);
    /// Возвращает текущий цвет рисования
    static Color GetColor();
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

    int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);

    static int DrawCharWithLimitation(int eX, int eY, char symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    static bool ByteFontNotEmpty(uint eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);

    void OnTimerFlashDisplay();
    /// Записывает мигающй цвет в дисплей. Возвращает false, если текущий цвет немигающий
    bool WriteFlashColor();
    /// Записывает цвет в дисплей
    void WriteColor(Color color);

    extern Color currentColor;
};
