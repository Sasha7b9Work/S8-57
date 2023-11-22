// 2023/11/22 13:37:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"


namespace PainterMem
{
    // Эту функцию нужно вызывать перед началом рисования. Она возвращает адрес начала области памяти, в которой размещён буфер для рисования.
    uint8 *CreateBuffer(int width, int height);

    // \brief Эту функцию нужно вызывать по завершении рисования, чтобы освободить место, занимаемеое буфером.
    // Можно и не вызывать, если нет надобности экономить память - в этом случае в следующий раз CreateBuffer() можно не вызывать.
    void DeleteBuffer();

    void FillRect(int x, int y, int width, int height, Color color = Color::NUMBER);

    void SetPoint(int x, int y);

    void SetPoint(int x, int y, Color color);

    void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    void DrawRectangle(int x, int y, int widht, int height, Color = Color::NUMBER);
};
