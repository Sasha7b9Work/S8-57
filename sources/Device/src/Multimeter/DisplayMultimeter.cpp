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

    if(Multimeter::Measure().IsTestDiode())
    {
        int edge = 50;
        int delta = edge / 2;
        int length = 2 * edge;

        int startX = x0 + 115;
        int endX = startX + length;
        int startY = y0 + 150;

        HLine(length).Draw(startX, startY, Color::FILL);
        VLine(edge).Draw(startX + delta, startY - edge / 2);
        VLine(edge).Draw(endX - delta, startY - edge / 2);
        Line(startX + delta, startY - edge / 2, endX - delta, startY).Draw();
        Line(startX + delta, startY + edge / 2, endX - delta, startY).Draw();
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
    if (Multimeter::Measure::IsVoltageDC())
    {
        return Multimeter::RangeDC();
    }
    else if (Multimeter::Measure::IsVoltageAC())
    {
        return Multimeter::RangeAC();
    }
    else if (Multimeter::Measure::IsCurrentDC())
    {
        return Multimeter::RangeCurrentDC();
    }
    else if (Multimeter::Measure::IsCurrentAC())
    {
        return Multimeter::RangeCurrentAC();
    }
    else if (Multimeter::Measure::IsResistance())
    {
        return Multimeter::RangeResistance();
    }

    return 0;
}


void DisplayMultimeter::ChangedMode()
{
    received = false;

    std::memset(outBuffer, '-', 7); //-V512

    static const int position[Multimeter::Measure::Count][4] =
    {
        {2, 3, 4},      // VoltageDC
        {2, 3, 4},      // VoltageAC
        {3, 2},         // CurrentDC
        {3, 2},         // CurrentAC
        {2, 3, 4, 3},   // Resistance
        {2},            // TestDiode
        (2),            // Bell
    };
    
    static const pString suffix[Multimeter::Measure::Count][4] =
    {
        {"V=", "V=", "V="},                     // U=
        {"V\x7e", "V\x7e", "V\x7e"},            // U~
        {"mA=", "A="},                          // I=
        {"mA\x7e", "A\x7e"},                    // I~
        {"k\x01", "k\x01", "k\x01", "M\x01"},   // R
        {"  ", "  ", "  ", "  "},               // VD
        {"k\x01"}                               // Прозвонка
    };

    outBuffer[position[Multimeter::Measure()][GetRange()]] = '.';
    
    std::strcpy(&outBuffer[7], suffix[Multimeter::Measure()][GetRange()]);
    
    if(Multimeter::Measure().IsResistance())
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
    funcs[Multimeter::Measure::Count] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareBell
    };

    Multimeter::Measure::E meas = Multimeter::Measure::GetCode(reinterpret_cast<const char *>(buf));

    if (meas >= Multimeter::Measure::Count)
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
    outBuffer[7] = 'k';
    outBuffer[8] = SYMBOL_OMEGA;

    if (ResistanceLess100())
    {
        Beeper::Bell::On();
    }
    else
    {
        Beeper::Bell::Off();
    }
}
