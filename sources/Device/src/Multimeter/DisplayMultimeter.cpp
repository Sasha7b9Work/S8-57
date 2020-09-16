#include "defines.h"
#include "device.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Multimeter/Multimeter.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cmath>


#define SYMBOL_OMEGA '\x01'


static char outBuffer[15];      // Данные для вывода.
static bool received = false;


static void PrepareBell(pCHAR);
static void PrepareVoltageDC(pCHAR);
static void PrepareVoltageAC(pCHAR);
static void PrepareCurrentDC(pCHAR);
static void PrepareCurrentAC(pCHAR);
static void PrepareResistance(pCHAR);
static void PrepareTestDiode(pCHAR);

// Отрисовать значение измерения
static void DrawSymbols(bool inModeOsci);

// Отрисовать один символ
static void DrawChar(int numSymbol, int x, bool inModeOsci);

// Отрисовать измерение. Если inModeOsci == true, рисовать надо на экране, совмещённом с осциллографом
static void DrawMeasure(bool inModeOsci);

// Нарисовать дополнительные изображения на экране, если в этом есть необходимость
static void DrawGraphics(bool inModeOsci);

// Отобразить единицы измерения
static void DrawUnits(bool inModeOsci);

// Нарисовать линии вправо и влево отностиельно центра с длиной width
static void Draw2HLinesRelCenter(int center, int y, int width);

// Отрисовка в режиме осциллографа
static void UpdateInModeOsci();

// Отрисовка в режиме мультиметра
static void UpdateInModeMultimeter();

// Возвращает координату x для вывода измерения в совмещённом режиме
static int CalculateX();

// Возвращает координату y для вывода измерения в совмещённом режиме
static int CalculateY();

// Нарисовать символ омеги в заданных координатах
static void DrawSymbolOMEGA(int x, int y, bool inModeOsci);


static char Symbol(int i)
{
    return outBuffer[i];
}


static void DrawChar(int numSymbol, int x, bool inModeOsci)
{
    int y = inModeOsci ? (CalculateY() + 3) : 35;

    char symbols[2] = {Symbol(numSymbol), 0};

    if(symbols[0] == '-')
    {
        y -= 10;

        if (inModeOsci)
        {
            x -= 6;
            y += 7;
        }
    }
    else if(symbols[0] == '+')
    {
        y -= 9;

        if (inModeOsci)
        {
            x -= 6;
            y += 6;
        }
    }
    
    Text(symbols).Draw(x, y, Color::FILL);

    if(symbols[0] == '-' && !inModeOsci)
    {
        Pixel().Draw(x, y + 41);

        Region(3, 5).Fill(x + 31, y + 40, Color::BACK);
        Pixel().Draw(x + 30, y + 40);
        Pixel().Draw(x + 30, y + 45);
    }
    if (symbols[0] == '+' && inModeOsci)
    {
        VLine line(4);
        line.Draw(x + 4, y + 10, Color::BACK);
        line.Draw(x + 4, y + 17);
    }

    Color::FILL.SetAsCurrent();
}


// Рассчёт дополнительного смещения для точки и цифры 1 по иксу
static int CalculateOffsetX(int i, bool inModeOsci)
{
    if (Symbol(i) == '.')
    {
        return inModeOsci ? -2 : 0;
    }
    else if (Symbol(i) == '1')
    {
        return inModeOsci ? 2 : 10;
    }

    return 0;
}


static void DrawSymbols(bool inModeOsci)
{
    int x0 = 20;
    int x = 48;

    if (inModeOsci)
    {
        DFont::Set(DTypeFont::_GOST28);
        x0 = CalculateX();
        x += CalculateX(); //-V656
    }

    DrawChar(0, x0 + 10, inModeOsci);

    if (inModeOsci)
    {
        x -= 48;
    }

    for (int i = 1; i < 7; i++)
    {
        DrawChar(i, x0 + x + CalculateOffsetX(i, inModeOsci), inModeOsci);

        x += 38;

        if (Symbol(i) == '.')
        {
            x = x - (inModeOsci ? 15 : 22);
        }

        if (inModeOsci)
        {
            x -= 21;
        }
    }

    DrawUnits(inModeOsci);
}


static void DrawUnits(bool inModeOsci)
{
    int x = 120;
    int y = 125;

    if (inModeOsci)
    {
        x = CalculateX() + 113;
        y = CalculateY() + 3;
    }

    DFont::SetSpacing(5);

    if(outBuffer[8] == SYMBOL_OMEGA)
    {
        if (inModeOsci)
        {
            x += 5;
        }

        x = Text(String(outBuffer[7])).Draw(x, y);

        DrawSymbolOMEGA(x, y, inModeOsci);
    }
    else
    {
        Text(&outBuffer[7]).Draw(x, y);

        if(outBuffer[8] == '=' || 
           outBuffer[9] == '=')         // При миллиамперах этот симовл '=' будет девятым
        {
            if(outBuffer[9] == '=')
            {
                x = x + (inModeOsci ? 22 : 50);
            }

            Pixel pixel;

            if (inModeOsci)
            {
                Region region(1, 1);
                region.Fill(x + 22, y + 18, Color::BACK);
                region.Fill(x + 27, y + 18);
            }
            else
            {
                pixel.Draw(x + 44, y + 33, Color::BACK);
                pixel.Draw(x + 44, y + 38);
                pixel.Draw(x + 72, y + 38);

                pixel.Draw(x + 44, y + 46);
                pixel.Draw(x + 44, y + 51);
                pixel.Draw(x + 72, y + 51);

                Region region(3, 5);

                region.Fill(x + 51, y + 46);
                region.Fill(x + 62, y + 46);
            }
        }
    }

    DFont::SetSpacing(1);
}


static void DrawSymbolOMEGA(int x, int y, bool inModeOsci)
{
    Color::FILL.SetAsCurrent();

    if (inModeOsci)
    {
        int radius = 10;

        for (int i = 0; i < radius; i++)
        {
            float angle = std::asinf(i * (1.0F / radius));
            float length = std::cosf(angle) * radius;
            Draw2HLinesRelCenter(x + radius, y + i + radius - 1, static_cast<int>(length + 0.5F));
            Draw2HLinesRelCenter(x + radius, y - i + radius - 1, static_cast<int>(length + 0.5F));
        }

        Color::BACK.SetAsCurrent();

        static const int lengths[8] = { 6, 6, 6, 5, 5, 4, 3, 2};

        for (int i = 0; i < 8; i++)
        {
            Draw2HLinesRelCenter(x + radius, y - 1 + radius - i, lengths[i]);
            Draw2HLinesRelCenter(x + radius, y - 1 + radius + i, lengths[i]);
        }

        Region region(8, 2);

        region.Fill(x, y + 22, Color::FILL);
        region.Fill(x + 12, y + 22);

        Pixel pixel;
        pixel.Draw(x, y + 21);
        pixel.Draw(x + 20, y + 21);
        pixel.Draw(x, y + 24, Color::BACK);
        pixel.Draw(x + 20, y + 24);

        Region(2, 2).Fill(x + 9, y + 16, Color::BACK);

        Region leg(1, 3);
        leg.Fill(x + 7, y + 18, Color::FILL);
        leg.Fill(x + 12, y + 18);
    }
    else
    {
        int radius = 27;

        for (int i = 0; i < radius; i++)
        {
            float angle = std::asinf(i * (1.0F / radius));
            float length = std::cosf(angle) * radius;
            Draw2HLinesRelCenter(x + radius, y + i + radius - 1, static_cast<int>(length + 0.5F));
            Draw2HLinesRelCenter(x + radius, y - i + radius - 1, static_cast<int>(length + 0.5F));
        }

        Color::BACK.SetAsCurrent();

        static const int lengths[23] = { 18, 18, 18, 18, 18, 18, 17, 17, 17, 17, 16, 16, 16, 15, 14, 13, 12, 11, 10, 9, 7, 5, 3 };

        for (int i = 0; i < 23; i++)
        {
            Draw2HLinesRelCenter(x + radius, y - 1 + radius - i, lengths[i]);
            Draw2HLinesRelCenter(x + radius, y - 1 + radius + i, lengths[i]);
        }

        Region region(21, 6);

        region.Fill(x, y + 59, Color::FILL);
        region.Fill(x + 33, y + 59);

        Pixel pixel;
        pixel.Draw(x, y + 58);
        pixel.Draw(x + 54, y + 58);
        pixel.Draw(x, y + 65, Color::BACK);
        pixel.Draw(x + 54, y + 65);

        Region(10, 5).Fill(x + 22, y + 47);

        Region leg(4, 6);
        leg.Fill(x + 17, y + 52, Color::FILL);
        leg.Fill(x + 33, y + 52);
    }
}


static void Draw2HLinesRelCenter(int center, int y, int width)
{
    HLine(width).Draw(center, y);
    HLine(width).Draw(center - width, y);
}


static void DrawGraphics(bool inModeOsci)
{
    int x0 = 10;
    int y0 = 10;

    if(S_MULT_MEASURE_IS_TEST_DIODE)
    {
        if (inModeOsci)
        {
            int edge = 15;
            int delta = edge / 2;
            int length = 2 * edge;

            int startX = CalculateX() + 118;
            int endX = startX + length;
            int startY = CalculateY() + 15;

            HLine(length).Draw(startX, startY, Color::FILL);
            HLine(length).Draw(startX, startY + 1, Color::FILL);

            VLine(edge).Draw(startX + delta,     startY - edge / 2);
            VLine(edge).Draw(startX + delta + 1, startY - edge / 2);

            VLine(edge + 3).Draw(endX - delta,     startY - edge / 2 - 2);
            VLine(edge + 3).Draw(endX - delta + 1, startY - edge / 2 - 2);

            Line(startX + delta, startY - edge / 2 - 1, endX - delta, startY - 1).Draw();
            Line(startX + delta, startY - edge / 2 - 2, endX - delta, startY - 2).Draw();

            Line(startX + delta, startY + edge / 2 + 1, endX - delta, startY + 1).Draw();
            Line(startX + delta, startY + edge / 2 + 2, endX - delta, startY + 2).Draw();
        }
        else
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
    }
    else if(S_MULT_MEASURE_IS_BELL)
    {
        if (inModeOsci)
        {
            int edge = 8;

            int x = CalculateX() + 125;
            int y = CalculateY() + 11;

            Rectangle(edge, edge).Draw(x, y, Color::FILL);

            Line(x + edge, y, x + edge * 2, y - edge).Draw();

            Line(x + edge, y + edge, x + edge * 2, y + edge * 2).Draw();

            VLine(3 * edge).Draw(x + edge * 2, y - edge);
        }
        else
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
    
    if(PageMultimeter::ZeroEnabled() || S_MULT_AVP_ENABLED)
    {
        if (!inModeOsci)
        {
            int x = 10;
            int y = 145;

            int dX = 6;
            int dY = 6;

            int width = 60;
            int height = 36;

            if (PageMultimeter::ZeroEnabled())
            {
                width = 69;
                height = 43;
            }

            Rectangle(width, height).Draw(x, y, Color::FILL);

            DFont::Set(DTypeFont::_GOST28);
            int spacing = DFont::GetSpacing();
            DFont::SetSpacing(3);

            char *text = PageMultimeter::ZeroEnabled() ? "Нуль" : "АВП";

            Text(text).Draw(x + dX, y + dY);

            DFont::Pop();
            DFont::SetSpacing(spacing);
        }
    }
}


static void DrawMeasure(bool inModeOsci)
{
    if (inModeOsci)
    {
        Region(DisplayMultimeter::Width(), DisplayMultimeter::HEIGHT).DrawBounded(CalculateX(), CalculateY(), Color::BACK, Color::FILL);
    }

    Color color = received ? Color::FILL : Color::GRAY_50;

    DFont::Set(DTypeFont::_GOST72bold);

    color.SetAsCurrent();
    
    DrawSymbols(inModeOsci);

    DrawGraphics(inModeOsci);

    DFont::Set(DTypeFont::_8);
}


void DisplayMultimeter::Update()
{
//    static uint prevTime = 0;
//    static bool isMinus = true;
//
//    if (TIME_MS - prevTime > 1000)
//    {
//        isMinus = !isMinus;
//        prevTime = TIME_MS;
//    }
//
//    outBuffer[0] = isMinus ? '+' : '-';
//
//    outBuffer[4] = isMinus ? '1' : '2';

    if (Device::InModeOsci())
    {
        UpdateInModeOsci();
    }
    else if (Device::InModeMultimeter())
    {
        UpdateInModeMultimeter();
    }
}


static void UpdateInModeOsci()
{
    if (!S_MULT_SHOW_ALWAYS)
    {
        return;
    }

    DrawMeasure(true);
}


static void UpdateInModeMultimeter()
{
    Painter::BeginScene(Color::BACK);

    DrawMeasure(false);

    Color::FILL.SetAsCurrent();

    Menu::Draw();
}


static int GetRange()
{
    int result = 0;
    
    switch(S_MULT_MEASURE)
    {
    case MultimeterMeasure::VoltageDC:    result = S_MULT_RANGE_VOLTAGE_DC;     break;
    case MultimeterMeasure::VoltageAC:    result = S_MULT_RANGE_VOLTAGE_AC;     break;
    case MultimeterMeasure::CurrentDC:    result = S_MULT_RANGE_CURRENT_DC;     break;
    case MultimeterMeasure::CurrentAC:    result = S_MULT_RANGE_CURRENT_AC;     break;
    case MultimeterMeasure::Resistance:   result = S_MULT_RANGE_RESISTANCE;     break;

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

    outBuffer[position[S_MULT_MEASURE][GetRange()]] = '.';
    
    std::strcpy(&outBuffer[7], suffix[S_MULT_MEASURE][GetRange()]);
    
    if(S_MULT_MEASURE_IS_RESISTANCE)
    {
        outBuffer[8] = SYMBOL_OMEGA;
    }
}


void DisplayMultimeter::SetMeasure(const uint8 buf[13])
{
    typedef void(*pFuncVCC)(pCHAR);

    static const struct Func
    {
        pFuncVCC func;
        Func(pFuncVCC f) : func(f) {};
    }
    funcs[MultimeterMeasure::Count] =
    {
        PrepareVoltageDC,
        PrepareVoltageAC,
        PrepareCurrentDC,
        PrepareCurrentAC,
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


static void PrepareTestDiode(pCHAR)
{
    std::strcpy(outBuffer + 7, "  ");
}


static void PrepareVoltageDC(pCHAR) //-V524
{
    std::strcpy(outBuffer + 7, "V=");
}


static void PrepareVoltageAC(pCHAR)
{
    std::strcpy(outBuffer + 7, "V~");
}


static void PrepareCurrentDC(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A=" : "mA=");
}


static void PrepareCurrentAC(const char *buf)
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


static void PrepareBell(pCHAR)
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


static int CalculateX()
{
    return Grid::Left();
}


static int CalculateY()
{
    return Grid::Top();
}


int DisplayMultimeter::Width()
{
    int result = 150;

    if (S_MULT_MEASURE_IS_CURRENT_DC && S_MULT_RANGE_CURRENT_DC_IS_20mA)
    {
        result = 175;
    }
    else if (S_MULT_MEASURE_IS_CURRENT_AC && S_MULT_RANGE_CURRENT_AC_IS_20mA)
    {
        result = 175;
    }
    else if (S_MULT_MEASURE_IS_RESISTANCE)
    {
        result = S_MULT_RANGE_RESISTANCE_IS_10M ? 170 : 165;
    }
    else if (S_MULT_MEASURE_IS_TEST_DIODE)
    {
        result = 155;
    }

    return result;
}
