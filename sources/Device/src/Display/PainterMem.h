#pragma once
#include "Colors.h"




/** @addtogroup Display
 *  @{
 *  @addtogroup Painter
 *  @{
 *  @defgroup PainterMem MemoryPainter
 *  @brief Набор функций для рисования в памяти 4-битным цветом
 *  @{
 */

class PainterMem
{
public:
    /// Эту функцию нужно вызывать перед началом рисования. Она возвращает адрес начала области памяти, в которой размещён буфер для рисования.
    static uint8 *CreateBuffer(int width, int height);
    /// \brief Эту функцию нужно вызывать по завершении рисования, чтобы освободить место, занимаемеое буфером.
    /// Можно и не вызывать, если нет надобности экономить память - в этом случае в следующий раз CreateBuffer() можно не вызывать.
    static void DeleteBuffer();
    
    static void FillRect(int x, int y, int width, int height, Color color = Color::NUMBER);
    
    static void SetPoint(int x, int y);

    static void SetPoint(int x, int y, Color color);
    
    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);
    
    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);
    
    static void DrawRectangle(int x, int y, int widht, int height, Color = Color::NUMBER);

private:
    /// Адрес начала области памяти, в которой хранится буфер для рисования. Если == 0, то буфер не создан.
    static uint8 *buffer;

    static uint8 *endBuffer;

    static int width;

    static int height;

    static Color color;
};

/** @}  @}  @}
 */
