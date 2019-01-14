#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "Display_Primitives.h"
#include "Painter.h"
#include "Hardware/FSMC.h"
#include "Utils/Math.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Display::Primitive::Primitive(int _x, int _y) : x(_x), y(_y), color(Color::NUMBER)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Region::Region(int _width, int _height) : width(_width), height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Region::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Region::DrawBounded(int x, int y, Color colorFill, Color colorBound)
{
    Color color = Color::GetCurent();
    Rectangle(width, height).Draw(x, y, colorBound);
    Region(width - 2, height - 2).Draw(x + 1, y + 1, colorFill);
    /// \todo Почему-то цвет не восстанавливается
    Color::SetCurrent(color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{
}
    
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Rectangle::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::HLine::HLine(int _width) : width(_width)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::HLine::Draw(int x, int y, Color color)
{
	Color::SetCurrent(color);
	int x0 = x;
	int x1 = x0 + width;
	uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
	FSMC::WriteToPanel(buffer, 6);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::VLine::VLine(int _height) : height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::VLine::Draw(int x, int y, Color color)
{
	Color::SetCurrent(color);
	int y0 = y;
	int y1 = y0 + height;
	uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
	FSMC::WriteToPanel(buffer, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Point::Draw(int x, int y, Color color)
{
	Color::SetCurrent(color);
	uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
	FSMC::WriteToPanel(buffer, 4);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Line::Line(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Line::Draw(Color color)
{
	Color::SetCurrent(color);
	uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
	FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Char::Char(char _ch) : ch(_ch)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Char::Draw(int x, int y, Color color)
{
	String("%c", ch).Draw(x, y, color);

	return x + Font::GetLengthSymbol(ch) + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Char::Draw4SymbolsInRect(int x, int y, Color color)
{
    Color::SetCurrent(color);

    for (char i = 0; i < 2; i++)
    {
        Char(ch + i).Draw(x + 8 * i, y);
        Char(ch + i + 16).Draw(x + 8 * i, y + 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Char::Draw10SymbolsInRect(int x, int y)
{
    for (char i = 0; i < 5; i++)
    {
        Char(ch + i).Draw(x + 8 * i, y);
        Char(ch + i + 16).Draw(x + 8 * i, y + 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Text::Text(const char *_text, uint8 _size) : text(_text), sizeOfType(_size)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Text::Text(const String &string, uint8 _size) : sizeOfType(_size)
{
    text = string.CString();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::Draw(int x, int y, Color color)
{
    if (sizeOfType != 1)
    {
        DrawBig(x, y, color);
    }
    else
    {
        return DrawSmall(x, y, color);
    }

    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Text::DrawBig(int x, int y, Color color)
{
#define MAX_SIZE_BUFFER 100

    Color::SetCurrent(color);

    uint numSymbols = std::strlen(text); //-V2513
    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawBigText, (uint8)x, (uint8)(x >> 8), (uint8)y, sizeOfType, (uint8)(numSymbols) };

    uint8 *pointer = &buffer[6];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, 1 + 2 + 1 + 1 + numSymbols + 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int retValue = x;

    while (*text)
    {
        x = DrawCharWithLimitation(x, y, *text, limitX, limitY, limitWidth, limitHeight);
        retValue += Font::GetLengthSymbol(*text);
        text++;
    }

    return retValue + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    uint8 symbol = (uint8)_symbol;

    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Point().Draw(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::Text::ByteFontNotEmpty(uint eChar, int byte)
{
    static const uint8 *bytes = 0;
    static uint prevChar = 0xffffffffU;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::Text::BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      /// \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color color)
{
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2 + 1;
    return Draw(x, y, color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawSmall(int x, int y, Color color)
{
    /// \todo Такую проверку нужно сделать и на приёмной стороне и тогда здесь убрать

    if (*text == 0)
    {
        return x;
    }

    int result = x + Font::GetLengthText(text) + 1;

#define MAX_SIZE_BUFFER 100

    if (std::strlen(text) + 1 > MAX_SIZE_BUFFER) //-V2513
    {
        return x + 10;
    }

    Color::SetCurrent(color);

    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawText, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)std::strlen(text) };

    for (int i = 0; text[i] != 0; i++)
    {
        buffer[5 + i] = (uint8)text[i];
    }

    FSMC::WriteToPanel(buffer, 1 + 2 + 1 + 1 + std::strlen(text));

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawOnBackground(int x, int y, Color colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetSize();

    Color colorText(Color::GetCurent());
    Region(width, height).Draw(x - 1, y, colorBackground);

    Color::SetCurrent(colorText);

    //return DrawText(x, y, text);
    return Text(text).Draw(x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Text::DrawRelativelyRight(int xRight, int y, Color color)
{
    int lenght = Font::GetLengthText(text);
    //DrawText(xRight - lenght, y, text, color);
    Text(text).Draw(xRight - lenght, y, color);
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

    static const bool template1[3] = { false, true, true };               //     011     2   // После второго символа перенос
    static const bool template2[4] = { true, false, true, false };        //     1010    2
    static const bool template3[4] = { false, true, false, true };        //     0101    3
    static const bool template4[4] = { true, false, true, true };         //     1011    3
    static const bool template5[4] = { false, true, false, false };       //     0100    3
    static const bool template6[4] = { true, false, true, true };         //     1011    3
    static const bool template7[5] = { true, true, false, true, false };  //     11010   3
    static const bool template8[6] = { true, true, false, true, true };   //     11011   4

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
        int8 lengths[] = { 5, 2, 2, 0 };
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "соответствующей") == 0) //-V2513
    {
        int8 lenghts[] = { 4, 3, 4, 5, 3, 0 };
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
    uint length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += (uint)lengthSyllables[i];
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
int Display::Text::DrawInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, Color color)
{
    Color::SetCurrent(color);

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
int Display::Text::DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, text, &height);

    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Draw(x + 1, y + 1, colorBackground);
    DrawInRectWithTransfers(x + 3, y + 3, width - 8, height, colorFill);
    return y + height;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill)
{
    //FillBoundedRegion(x, y, width, height, colorBackground, colorFill);
    Region(width, height).DrawBounded(x, y, colorBackground, colorFill);

    Color::SetCurrent(colorFill);
    //return DrawStringInCenterRect(x, y, width, height, text);

    return Text(text).DrawInCenterRect(x, y, width, height);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Text::DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground)
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
Display::DashedVLine::DashedVLine(int _height, int _deltaFill, int _deltaEmpty, int _deltaStart) : height(_height), deltaFill(_deltaFill), deltaEmpty(_deltaEmpty), deltaStart(_deltaStart)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DashedVLine::Draw(int x, int y0)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            //DrawVLine(x, y0, y - 1);
            VLine(y - y0 - 1).Draw(x, y0);
        }
    }

    int y1 = y0 + height;

    while (y < y1)
    {
        //DrawVLine(x, y, y + deltaFill - 1);
        VLine(deltaFill - 1).Draw(x, y);

        y += (deltaFill + deltaEmpty);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::DashedHLine::DashedHLine(int _width, int _deltaFill, int _deltaEmpty, int _deltaStart) : width(_width), deltaFill(_deltaFill), deltaEmpty(_deltaEmpty), deltaStart(_deltaStart)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DashedHLine::Draw(int x0, int y)
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

    int x1 = x0 + width;

    while (x < x1)
    {
        //DrawHLine(y, x, x + deltaFill - 1);
        HLine(deltaFill - 1).Draw(x, y);

        x += (deltaFill + deltaEmpty);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::VPointLine::VPointLine(int _height, float _delta) : height(_height), delta(_delta)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::VPointLine::Draw(int _x, int _y)
{
    int y0 = _y;
    int y1 = y0 + height;

    for (int y = y0; y <= y1; y += (int)delta)
    {
        //SetPoint(x, y);
        Point().Draw(_x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::HPointLine::HPointLine(int _width, float _delta) : width(_width), delta(_delta)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::HPointLine::Draw(int _x, int _y)
{
    int x0 = _x;
    int x1 = _x + width;

    for (int x = x0; x <= x1; x += (int)delta)
    {
        //SetPoint(x, y);
        Point().Draw(x, _y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::MultiHPointLine::MultiHPointLine(int _numLines, const uint8 *_y, int _delta, int _count) : numLines(_numLines), y(_y), delta(_delta), count(_count)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::MultiHPointLine::Draw(int x, Color color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        for (int numPoint = 0; numPoint < count; numPoint++)
        {
            Point().Draw(x + numPoint * delta, y[i]);
        }
    }
}
