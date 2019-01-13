#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display_Primitives.h"
#include "Painter.h"
#include "Keyboard/DecoderDevice.h"
#include "Utils/Math.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Char;
using Display::HLine;
using Display::Point;
using Display::Rectangle;
using Display::Region;
using Display::Text;
using Display::VLine;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Painter::DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color)
{
    Color::SetCurrent(color);
    return DrawTextInRectWithTransfers(x, y, width, height, text);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
    };

    return isLetter[(uint8)symbol];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static char *GetWord(const char *firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (IsLetter(*firstSymbol))
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[(uint8)symbol];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool CompareArrays(const bool *array1, const bool *array2, int numElems) //-V2506
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// \brief Находит следующий перенос. C letters начинается часть слово, где нужно найти перенос, в lettersInSyllable будет записано число букв в 
/// найденном слоге. Если слово закончилось, функция возвращает false
static bool FindNextTransfer(const char *letters, int8 *lettersInSyllable) //-V2506
{

#define VOWEL       0   // Гласная
#define CONSONANT   1   // Согласная

    * lettersInSyllable = (int8)std::strlen(letters); //-V2513 //-V1029

    if (std::strlen(letters) <= 3) //-V2513
    {
        return false;
    }

    static const bool template1[3] = {false, true, true};               //     011     2   // После второго символа перенос
    static const bool template2[4] = {true, false, true, false};        //     1010    2
    static const bool template3[4] = {false, true, false, true};        //     0101    3
    static const bool template4[4] = {true, false, true, true};         //     1011    3
    static const bool template5[4] = {false, true, false, false};       //     0100    3
    static const bool template6[4] = {true, false, true, true};         //     1011    3
    static const bool template7[5] = {true, true, false, true, false};  //     11010   3
    static const bool template8[6] = {true, true, false, true, true};   //     11011   4

    bool consonant[20];

    int size = (int)std::strlen(letters); //-V2513

    for (int i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }

    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }

    if (std::strlen(letters) < 5) //-V2513
    {
        return false;
    }

    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) ||
        CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }

    if (std::strlen(letters) < 6) //-V2513
    {
        return false;
    }

    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }

    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int8 *BreakWord(char *word)
{
    int num = 0;
    static int8 lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;

    if (std::strcmp(word, "структуру") == 0) //-V2513
    {
        int8 lengths[] = {5, 2, 2, 0};
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "соответствующей") == 0) //-V2513
    {
        int8 lenghts[] = {4, 3, 4, 5, 3, 0};
        std::memcpy(lengthSyllables, lenghts, 6);
    }
    else
    {
        // здесь ничего не делаем
    }

    return lengthSyllables;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Возвращает часть слова до слога numSyllable(включительн) вместе со знаком переноса
static char *PartWordForTransfer(char *word, const int8 *lengthSyllables, int numSyllable, char buffer[30])
{
    size_t length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += (size_t)lengthSyllables[i];
    }
    std::memcpy((void *)buffer, (void *)word, length);
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
static int DrawPartWord(char *word, int x, int y, int xRight, bool draw) //-V2506
{
    int8 *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];
    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
                String(subString).Draw(x, y);
            }
            return (int)std::strlen(subString) - 1; //-V2513
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawTextInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, const char *text)
{
    int top = eY;
    int left = eX;
    int right = eX + eWidth;
    int bottom = eY + eHeight;

    char buffer[20];
    int numSymbols = (int)std::strlen(text); //-V2513

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                //x = DrawChar(x, y, symbol);
                x = Char(symbol).Draw(x, y);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    //x = DrawText(x, y, word);
                    x = Text(word).Draw(x, y);
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 5; i++)
    {
        //DrawChar(x + 8 * i, y, eChar + i);
        Char(eChar + i).Draw(x + 8 * i, y);

        //DrawChar(x + 8 * i, y + 8, eChar + i + 16);
        Char(eChar + i + 16).Draw(x + 8 * i, y + 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill)
{
    //FillBoundedRegion(x, y, width, height, colorBackground, colorFill);
    Region(width, height).DrawBounded(x, y, colorBackground, colorFill);

    Color::SetCurrent(colorFill);
    //return DrawStringInCenterRect(x, y, width, height, text);

    return Text(text).DrawInCenterRect(x, y, width, height);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Painter::FillBoundedRegion(int x, int y, int widht, int height, Color colorFill, Color colorBound)
{
    Region(widht - 2, height - 2).Draw(x + 1, y + 1, colorFill);
    Rectangle(widht, height).Draw(x, y, colorBound);
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
static bool GetHeightTextWithTransfers(int left, int top, int right, const char *text, int *height)
{
    char buffer[20];
    int numSymbols = (int)std::strlen(text); //-V2513

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x += Font::GetLengthSymbol(symbol);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, false);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::GetLengthText(word);
                }
            }
        }
        x = left;
        y += 9;
    }

    *height = y - top + 4;

    LIMITATION(*height, 0, 239); //-V2516

    return curSymbol == numSymbols;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, text, &height);

    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Draw(x + 1, y + 1, colorBackground);
    DrawTextInRectWithTransfersC(x + 3, y + 3, width - 8, height, text, colorFill);
    return y + height;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw4SymbolsInRect(int x, int y, char eChar, Color color)
{
    Color::SetCurrent(color);
 
    for (char i = 0; i < 2; i++)
    {
        //DrawChar(x + 8 * i, y, eChar + i);
        Char(eChar + i).Draw(x + 8 * i, y);

        //DrawChar(x + 8 * i, y + 8, eChar + i + 16);
        Char(eChar + i + 16).Draw(x + 8 * i, y + 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    //int eX = DrawStringInCenterRect(x, y, width, height, text, colorBackground);
    int eX = Text(text).DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Draw(eX - lenght - widthBorder, y - widthBorder + 1);
    //DrawStringInCenterRect(x, y, width, height, text, colorText);
    Text(text).DrawInCenterRect(x, y, width, height, colorText);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color)
{
    Color::SetCurrent(color);

    for(int i = 0; i < numLines; i++)
    {
        uint8 y0 = *y0y1++;
        uint8 y1 = *y0y1++;

        //DrawVLine(x, y0, y1);
        VLine(y1 - y0).Draw(x, y0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiVPointLine(int numLines, int y0, const uint16 *x0, int delta, int count, Color color)
{
    Color::SetCurrent(color);

    for(int i = 0; i < numLines; i++)
    {
        int x = x0[i];
        for(int numPoint = 0; numPoint < count; numPoint++)
        {
            int y = y0 + numPoint * delta;
            //SetPoint(x, y);
            Point().Draw(x, y);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiHPointLine(int numLines, int x0, const uint8 *y0, int delta, int count, Color color)
{
    Color::SetCurrent(color);

    for(int i = 0; i < numLines; i++)
    {
        int y = y0[i];
        for(int numPoint = 0; numPoint < count; numPoint++)
        {
            int x = x0 + numPoint * delta;
            //SetPoint(x, y);
            Point().Draw(x, y);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += (int)delta)
    {
        //SetPoint(x, y);
        Point().Draw(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVPointLine(int x, int y0, int y1, float delta)
{
    for (int y = y0; y <= y1; y += (int)delta)
    {
        //SetPoint(x, y);
        Point().Draw(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart) //-V2506
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            //DrawVLine(x, y0, y - 1);
            VLine(y - y0 - 1).Draw(x, y0);
        }
    }

    while (y < y1)
    {
        //DrawVLine(x, y, y + deltaFill - 1);
        VLine(deltaFill - 1).Draw(x, y);

        y += (deltaFill + deltaEmtpy);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart) //-V2506
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            //DrawHLine(y, x0, x - 1);
			HLine(x - 1 - x0).Draw(x0, y);
        }
    }

    while (x < x1)
    {
        //DrawHLine(y, x, x + deltaFill - 1);
		HLine(deltaFill - 1).Draw(x, y);

        x += (deltaFill + deltaEmpty);
    }
}
