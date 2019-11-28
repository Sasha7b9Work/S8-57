#pragma once
#include "Font.h"



struct AdvancedFont
{
    /// Установить действующий шрифт
    AdvancedFont(TypeFont::E type);
    /// Возвращает true, если в строке row есть пиксели
    static bool RowNotEmpty(uint8 symbol, int row);
    /// Возвращает ширину символа
    static uint8 GetWidth(uint8 symbol);
    /// Возвращает высоту символа
    static uint8 GetHeight(uint8 symbol);
    /// Возвращает высоту самого высокого символа
    static uint8 GetHeight();

    static bool BitIsExist(uint8 symbol, int row, int bit);

private:
    static TypeFont::E currentType;
};
