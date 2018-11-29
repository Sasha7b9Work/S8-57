#pragma once
#include "Colors.h"
#include "DisplayTypes.h"
#include "Display/Font/Font.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Painter
{
public:
    static void Init();

    static void BeginScene(Color color);

    static void EndScene();
    /// Елси не передавать значение цвета, то цвет останется прежним
    static void SetColor(Color color = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    static int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);

    static void DrawBigText(int x, int y, uint8 size, const char *text, Color color = Color::NUMBER);

    static void SetColorValue(Color color, uint value);

    static void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    static int DrawFormatText(int x, int y, Color color, char *format, ...);

    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    static void DrawHLine(float y, int x0, int x1, Color color = Color::NUMBER);

    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    static void DrawVLine(int x, float y0, float y1, Color color = Color::NUMBER);

    static void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);

    static void SetFont(Font::Type::E typeFont);

    static void SetPoint(int x, int y, Color color = Color::NUMBER);

    static void DrawBoundedRegion(int x, int y, int width, int height, Color colorFill, Color colorBound);

    static void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

    static int DrawTextWithLimitation(int x, int y, const char *text, int limitX, int limitY, int limitWidth, int limitHeight);

    static void FillBoundedRegion(int x, int y, int widht, int height, Color colorFill, Color colorBound);

    static void ResetFlash();
    /// Пишет текст с переносами
    static int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    static int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);
    /// Возвращает нижнюю координату прямоугольника
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);
    /// Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    static void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder,
                                                    Color colorBackground);
    static int DrawFormatText(int x, int y, char *format, ...);
    /// Выводит текст на прямоугольнике цвета colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);
    /// Пишет строку в позиции x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);
    /// Установить яркость дисплея.
    static void SetBrightnessDisplay(int16 brightness);

    static int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    static void Draw10SymbolsInRect(int x, int y, char eChar);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);

    static void DrawTextInRect(int x, int y, int width, const char *text);
    /// Возвращает текущий цвет рисования
    static Color GetColor();
    /// Нарисовать массив вертикальных линий. Линии рисуются одна за другой. y0y1 - массив вертикальных координат.
    static void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// \brief Нарисовать numLines вертикальных линий, состоящих из count точек каждая с расстоянием между точками delta. Горизонтальная координата
    /// первой точки каждой линии соответствует очередному элементу массива x[]
    static void DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color = Color::NUMBER);
    /// \brief Нарисовать numLines горизонтальных линий, состоящих из count точек каждая с расстоянием между точками delta. Вертикальная координата
    /// первой точки каждой линии соответствует очередному элементу массива y[]
    static void DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color = Color::NUMBER);
    /// Нарисовать горизонтальную линию от x0 до x1 точками через каждые delta пикселей
    static void DrawHPointLine(int y, int x0, int x1, float delta);
    /// Нарисовать вертикальную линию от y0 до y1 точками через каждые delta пикселей
    static void DrawVPointLine(int x, int y0, int y1, float delta);

    static void DrawTesterData(uint8 mode, Color color, uint8 *x, uint8 *y);

private:

    static int DrawCharWithLimitation(int eX, int eY, char symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    static bool ByteFontNotEmpty(uint eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);

    static void OnTimerFlashDisplay();

    static int GetLenghtSubString(const char *text);

    static int DrawSubString(int x, int y, const char *text);

    static int DrawSpaces(int x, int y, const char *text, int *numSymbols);
    /// Записывает мигающй цвет в дисплей. Возвращает false, если текущий цвет немигающий
    static bool WriteFlashColor();
    /// Записывает цвет в дисплей
    static void WriteColor(Color color);
    /// Текущий цвет рисования
    static Color currentColor;
};
