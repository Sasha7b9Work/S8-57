#include "Painter.h"
#include "Display/Font/Font.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include <cstdarg>
#include <stdio.h>
#include <string.h>



extern void CalculateCurrentColor();



int Painter::DrawTextOnBackground(int x, int y, const char *text, Color colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetHeight();

    Color colorText(GetColor());
    SetColor(colorBackground);
    FillRegion(x - 1, y, width, height);
    SetColor(colorText);

    return Text::Draw(x, y, text);
}


int Painter::DrawFormatText(int x, int y, char *format, ...)
{
    char buffer[200];
    std::va_list args;
    va_start(args, format); //-V2528
    vsprintf(buffer, format, args);
    va_end(args);
    return Text::Draw(x, y, buffer);
}


int Painter::DrawFormText(int x, int y, Color color, pString text, ...)
{
    Painter::SetColor(color);

#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    std::va_list args;
    va_start(args, text); //-V2528
    vsprintf(buffer, (char *)text, args);
    va_end(args);
    return Text::Draw(x, y, buffer);
}
