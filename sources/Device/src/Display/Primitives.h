#pragma once
#include "Utils/String.h"
#include "Display/Symbols.h"
#include "common/Display/Font/Font.h"


class Region
{
public:
    Region(int width, int height);
    void Fill(int x, int y, Color color = Color::NUMBER);
    void DrawBounded(int x, int y, Color colorFill, Color colorBound);
private:
    int width;
    int height;
};


class Rectangle
{
public:
    Rectangle(int width, int height);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int width;
    int height;
};



class HLine
{
public:
    HLine(int width);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int width;
};



class VLine
{
public:
    VLine(int height);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int height;
};



class Line
{
public:
    Line(int x0, int y0, int x1, int y1);
    void Draw(Color color = Color::NUMBER);
private:
    int x0;
    int y0;
    int x1;
    int y1;
};



class Char
{
public:
    explicit Char(SymbolUGO::E ch);
    explicit Char(SymbolUGO2::E ch);
    explicit Char(char ch, TypeFont::E font = TypeFont::_8);
    int Draw(int x, int y, Color color = Color::NUMBER);
    void Draw4SymbolsInRect(int x, int y, Color color = Color::NUMBER);
    void Draw10SymbolsInRect(int x, int y, Color color = Color::NUMBER);
private:
    char ch;
    TypeFont::E font;
};



class Pixel
{
public:
    Pixel() {};
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
};



class Text
{
public:
    Text(const char *text, uint8 size = 1);
    Text(const String &string, uint8 size = 1);

    int Draw(int x, int y, Color color = Color::NUMBER);

    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawInCenterBoundedRect(int x, int y, int width, int height, Color color = Color::NUMBER);

    int DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight);
    /// ¬ыводит текст на пр€моугольнике цвета colorBackgound
    int DrawOnBackground(int x, int y, Color colorBackground);

    void DrawRelativelyRight(int xRight, int y, Color color = Color::NUMBER);
    /// ѕишет текст с переносами
    int DrawInRectWithTransfers(int x, int y, int width, int height, Color color = Color::NUMBER);
    /// ¬озвращает нижнюю координату пр€моугольника
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill);

    void DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);
    /// ѕишет строку текста в центре области(x, y, width, height)цветом ColorText на пр€моугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);
private:
    const char *text;
    ///  егль. “олько в отличие от насто€щего он показывает, во сколько раз размер пиксел€ на экране больше единичного пиксел€
    uint8 sizeOfType;
    int DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight);
    void DrawBig(int x, int y, Color color = Color::NUMBER);
    int DrawSmall(int x, int y, Color color = Color::NUMBER);
};


class DashedVLine
{
public:
    DashedVLine(int height, int deltaFill, int deltaEmpty, int deltaStart);
    void Draw(int x, int y);
private:
    int height;
    int deltaFill;
    int deltaEmpty;
    int deltaStart;
};


class DashedHLine
{
public:
    DashedHLine(int width, int deltaFill, int deltaEmpty, int deltaStart);
    void Draw(int x, int y);
private:
    int width;
    int deltaFill;
    int deltaEmpty;
    int deltaStart;
};


/// ¬ертикальна€ лини€ от y до y + height точками через каждые delta пикселей
class VPointLine
{
public:
    VPointLine(int height, float delta);
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    int height;
    float delta;
};


/// √оризонтальна€ лини€ от x до x + width точками через каждые delta пикселей
class HPointLine
{
public:
    HPointLine(int width, float delta);
    void Draw(int x, int y);
private:
    int width;
    float delta;
};


/// \brief Ќарисовать numLines горизонтальных линий, состо€щих из count точек кажда€ с рассто€нием между точками delta. ¬ертикальна€ координата
/// первой точки каждой линии соответствует очередному элементу массива y[]
class MultiHPointLine
{
public:
    MultiHPointLine(int numLines, const uint8 *y, int delta, int count);
    void Draw(int x, Color color = Color::NUMBER);
private:
    int numLines;
    const uint8 *y;
    int delta;
    int count;
};


/// ћассив вертикальных линий. Ћинии рисуютс€ одна за другой. y0y1 - массив вертикальных координат.
class VLineArray
{
public:
    VLineArray(int numLines, uint8 *y0y1);
    void Draw(int x, Color color);
private:
    int numLines;
    uint8 *y0y1;
};


/// numLines вертикальных линий, состо€щих из count точек кажда€ с рассто€нием между точками delta. √оризонтальна€ координата
/// первой точки каждой линии соответствует очередному элементу массива x[]
class MultiVPointLine
{
public:
    MultiVPointLine(int numLines, uint16 *x0, int delta, int count);
    void Draw(int y0, Color color = Color::NUMBER);
private:
    int numLines;
    uint16 *x0;
    int delta;
    int count;
};
