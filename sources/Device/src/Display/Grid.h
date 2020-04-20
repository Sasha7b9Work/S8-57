#pragma once


struct Grid
{
    //  онстанты используютс€ дл€ расчЄта размеров, которые остаютс€ одинаковыми при любых размерах и пропорци€х сетки - например, элементов меню
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;

    static void Draw();

    // Ёти значени€ могут мен€тьс€ в зависимости от выставленного режима
    static int Left();
    static int Top();
    static int Width();
    static int Height();
    static int Bottom();
    static int Right();
    static int ChannelCenterHeight();
    static int ChannelHeight();
    static int FullBottom();
    static int MathBottom();
    static int MathHeight();
    static int ChannelBottom();
    // ¬озвращает нижнюю y-координату окна, в котором нужно выводить предупреждени€
    static int BottomForWarnings();
};
