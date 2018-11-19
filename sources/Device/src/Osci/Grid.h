#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TYPE_GRID       (set.disp_typeGrid)
#define TYPE_GRID_1     (TYPE_GRID == Grid::Type::_1)
#define TYPE_GRID_2     (TYPE_GRID == Grid::Type::_2)
#define TYPE_GRID_3     (TYPE_GRID == Grid::Type::_3)
#define TYPE_GRID_4     (TYPE_GRID == Grid::Type::_4)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Grid
{
public:

    /// Константы используются для расчёта размеров, которые остаются одинаковыми при любых размерах и пропорциях сетки - например, элементов меню
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;
    static const int LEFT = 20;

    /// Эти значения могут меняться в зависимости от выставленного режима
    static int Left();
    static int Top();
    static int Width();
    static int Height();
    static int Bottom();
    static int Right();
    static int NumRows();
    static int NumCols();
    static int ChannelCenterHeight();
    static int Delta();
    static int FullBottom();
    static int MathBottom();
    static int MathHeight();
    static int ChannelBottom();
    static int MathTop();

    /// Нарисовать сетку
    static void Draw();

private:

    static void DrawGridSignal(int left, int top, int width, int height);

    static void DrawGridSpectrum();

    static float DeltaY();

    static float DeltaX();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);

    static int  DeltaVforLineGrid();
    /// Возвращает расстояние между 
    static int  DeltaHforLineGrid();

public:

    /// Тип сетки на экране.
    struct Type
    {
        enum E
        {
            _1,
            _2,
            _3,
            _4,
            Size
        } value;
        operator uint8() const { return (uint8)value; };
    };
};
