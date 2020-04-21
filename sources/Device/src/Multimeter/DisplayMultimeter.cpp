#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Multimeter/Multimeter.h"
#include "Settings/Settings.h"
#include <cstring>


/*
    Шрифт для отрисовки в режиме мультиметра

    Имя - GOST

    Размер - 
*/


#define SYMBOL_OMEGA '\x01'


static char outBuffer[15];      // Данные для вывода.
static bool received = false;


static void PrepareBell(const char *);
static void PrepareConstantVoltage(const char *);
static void PrepareVariableVoltage(const char *);
static void PrepareConstantCurrent(const char *);
static void PrepareVariableCurrent(const char *);
static void PrepareResistance(const char *);
static void PrepareTestDiode(const char *);

// Отрисовать значение измерения
static void DrawSymbols();

// Нарисовать дополнительные изображения на экране, если в этом есть необходимость
static void DrawGraphics();


static char Symbol(uint i)
{
    return outBuffer[i];
}


static void DrawChar(uint numSymbol, int x)
{
    int y = 35;

    char symbols[2] = {Symbol(numSymbol), 0};

    if(symbols[0] == '-')
    {
        y -= 10;

        Pixel().Draw(x, y + 42);

    }
    else if(symbols[0] == '+')
    {
        y -= 9;
    }
    
    Text(symbols).Draw(x, y);

    if(symbols[0] == '-')
    {
        Region(5, 5).Fill(x + 31, y + 41, Color::BACK);
        Pixel().Draw(x + 30, y + 41);
        Pixel().Draw(x + 30, y + 46);
    }

    Color::FILL.SetAsCurrent();
}


static void DrawSymbols()
{
    int x0 = 20;

    DrawChar(0, x0 + 10);

    int x = 48;

    for (uint i = 1; i < 7; i++)
    {
        char symbol = Symbol(i);

        if (symbol == '1')
        {
            x += 10;
        }

        DrawChar(i, x0 + x);

        if (symbol == '1')
        {
            x -= 10;
        }

        x += (Symbol(i) == '.') ? 16 : 38;
    }

    DFont::SetSpacing(5);

    if(outBuffer[8] == SYMBOL_OMEGA)
    {
        x = Text(String(outBuffer[7])).Draw(120, 125);

        DFont::Set(DTypeFont::_OMEGA72);

        Text(String(SYMBOL_OMEGA)).Draw(x + 5, 130);

        DFont::Set(DTypeFont::_GOST72bold);
    }
    else
    {
        Text(&outBuffer[7]).Draw(120, 125);
    }

    DFont::SetSpacing(1);
}


static void DrawGraphics()
{
    int x0 = 10;
    int y0 = 10;

    if(set.mult.meas == MultimeterMeasure::TestDiode)
    {
        int edge = 50;
        int delta = edge / 2;
        int length = 2 * edge;

        int startX = x0 + 115;
        int endX = startX + length;
        int startY = y0 + 150;

        HLine(length).Draw(startX, startY - 1, Color::FILL);
        HLine(length).Draw(startX, startY);
        HLine(length).Draw(startX, startY + 1);

        VLine(edge + 6).Draw(startX + delta - 1, startY - edge / 2 - 3);
        VLine(edge + 2).Draw(startX + delta, startY - edge / 2 - 1);
        VLine(edge).Draw(startX + delta + 1, startY - edge / 2);

        VLine(edge + 6).Draw(endX - delta, startY - edge / 2 - 3);
        VLine(edge + 6).Draw(endX - delta + 1, startY - edge / 2 - 3);
        VLine(edge + 6).Draw(endX - delta + 2, startY - edge / 2 - 3);

        Line(startX + delta, startY - edge / 2 - 2, endX - delta + 1, startY - 1).Draw();
        Line(startX + delta, startY - edge / 2 - 1, endX - delta, startY - 1).Draw();
        Line(startX + delta, startY - edge / 2, endX - delta, startY).Draw();

        Line(startX + delta, startY + edge / 2, endX - delta, startY).Draw();
        Line(startX + delta, startY + edge / 2 + 1, endX - delta, startY + 1).Draw();
        Line(startX + delta, startY + edge / 2 + 2, endX - delta + 1, startY + 1).Draw();
    }
    else if(set.mult.meas == MultimeterMeasure::Bell)
    {
        int edge = 16;

        int x = x0 + 150;
        int y = y0 + 135;

        Rectangle(edge, edge).Draw(x, y, Color::FILL);
        Rectangle(edge + 2, edge + 2).Draw(x - 1, y - 1);
        Rectangle(edge + 4, edge + 4).Draw(x - 2, y - 2);

        Line(x + edge, y - 1, x + edge * 2, y - edge - 1).Draw();
        Line(x + edge, y, x + edge * 2, y - edge).Draw();
        Line(x + edge, y + 1, x + edge * 2, y - edge + 1).Draw();

        Line(x + edge, y + edge - 1, x + edge * 2, y + edge * 2 - 1).Draw();
        Line(x + edge, y + edge, x + edge * 2, y + edge * 2).Draw();
        Line(x + edge, y + edge + 1, x + edge * 2, y + edge * 2 + 1).Draw();

        VLine(3 * edge).Draw(x + edge * 2, y - edge);
        VLine(3 * edge + 4).Draw(x + edge * 2 + 1, y - edge - 2);
        VLine(3 * edge + 6).Draw(x + edge * 2 + 2, y - edge - 3);
    }
}


static void DrawMeasure()
{
    Color color = received ? Color::FILL : Color::GRAY_50;

    DFont::Set(DTypeFont::_GOST72bold);

    color.SetAsCurrent();
    
    DrawSymbols();

    DrawGraphics();

    DFont::Set(DTypeFont::_8);
}


void DisplayMultimeter::Update()
{
    Painter::BeginScene(Color::BACK);

    DrawMeasure();

    Color::FILL.SetAsCurrent();

    Menu::Draw();
}


static int GetRange()
{
    int result = 0;
    
    switch(set.mult.meas)
    {
    case MultimeterMeasure::VoltageDC:    result = set.mult.rangeVoltageDC;     break;
    case MultimeterMeasure::VoltageAC:    result = set.mult.rangeVoltageAC;     break;
    case MultimeterMeasure::CurrentDC:    result = set.mult.rangeCurrentDC;     break;
    case MultimeterMeasure::CurrentAC:    result = set.mult.rangeCurrentAC;     break;
    case MultimeterMeasure::Resistance:   result = set.mult.rangeResist;        break;

    case MultimeterMeasure::Bell:
    case MultimeterMeasure::TestDiode:
    case MultimeterMeasure::Count:
        break;
    }


    return result;
}


void DisplayMultimeter::ChangedMode()
{
    received = false;

    std::memset(outBuffer, '-', 7); //-V512

    static const int position[MultimeterMeasure::Count][4] =
    {
        {2, 3, 4},      // VoltageDC
        {2, 3, 4},      // VoltageAC
        {3, 2},         // CurrentDC
        {3, 2},         // CurrentAC
        {2, 3, 4, 3},   // Resistance
        {2},            // TestDiode
        (2),            // Bell
    };
    
    static const pString suffix[MultimeterMeasure::Count][4] =
    {
        {"V=", "V=", "V="},                     // U=
        {"V\x7e", "V\x7e", "V\x7e"},            // U~
        {"mA=", "A="},                          // I=
        {"mA\x7e", "A\x7e"},                    // I~
        {"k\x01", "k\x01", "k\x01", "M\x01"},   // R
        {"  ", "  ", "  ", "  "},               // VD
        {"  ", "  ", "  ", "  "}                // Прозвонка
    };

    outBuffer[position[set.mult.meas][GetRange()]] = '.';
    
    std::strcpy(&outBuffer[7], suffix[set.mult.meas][GetRange()]);
    
    if(set.mult.meas == MultimeterMeasure::Resistance)
    {
        outBuffer[8] = SYMBOL_OMEGA;
    }
}


void DisplayMultimeter::SetMeasure(const uint8 buf[13])
{
    typedef void(*pFuncVCC)(const char *);

    static const struct Func
    {
        pFuncVCC func;
        Func(pFuncVCC f) : func(f) {};
    }
    funcs[MultimeterMeasure::Count] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareBell
    };

    MultimeterMeasure::E meas = MultimeterMeasure::GetCode(reinterpret_cast<const char *>(buf));

    if (meas >= MultimeterMeasure::Count)
    {
        return;
    }

    std::memcpy(outBuffer, buf + 1, 7); //-V512

    funcs[meas].func(reinterpret_cast<const char *>(buf));

    received = true;
}


static void PrepareTestDiode(const char *)
{
    std::strcpy(outBuffer + 7, "  ");
}


static void PrepareConstantVoltage(const char *) //-V524
{
    std::strcpy(outBuffer + 7, "V=");
}


static void PrepareVariableVoltage(const char *)
{
    std::strcpy(outBuffer + 7, "V~");
}


static void PrepareConstantCurrent(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A=" : "mA=");
}


static void PrepareVariableCurrent(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A~" : "mA~");
}


void PrepareResistance(const char *buf)
{
    outBuffer[7] = buf[8];
    outBuffer[8] = SYMBOL_OMEGA;
}


static bool ResistanceLess100()
{
    return ((outBuffer[1] == '0') && (outBuffer[3] == '0'));
}


static void PrepareBell(const char *)
{
    std::strcpy(outBuffer + 7, "  ");

    if (ResistanceLess100())
    {
        Beeper::Bell::On();
    }
    else
    {
        Beeper::Bell::Off();
    }
}
