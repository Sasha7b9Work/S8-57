#pragma once


namespace Grid
{
    //  онстанты используютс€ дл€ расчЄта размеров, которые остаютс€ одинаковыми при любых размерах и пропорци€х сетки - например, элементов меню
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;

    void Draw();

    // Ёти значени€ могут мен€тьс€ в зависимости от выставленного режима
    int Left();
    int Top();
    int Width();
    int Height();
    int Bottom();
    int Right();
    int ChannelCenterHeight();
    int ChannelHeight();
    int FullBottom();
    int MathBottom();
    int MathHeight();
    int ChannelBottom();
    // ¬озвращает нижнюю y-координату окна, в котором нужно выводить предупреждени€
    int BottomForWarnings();
};
