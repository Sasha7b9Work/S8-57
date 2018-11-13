#pragma once


class  MemoryWindow
{
public:

    static void Draw();

    static int X();

    static int Y();

    static int Width();

    static int Height();

private:

    static void DrawDataInRect(int x, int y, int width, int height, uint8 *data, int length);
    /// Нарисовать область экрана
    static void DrawScreenArea();
};
