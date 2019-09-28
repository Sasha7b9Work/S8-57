#include "defines.h"
#include "log.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"


using namespace Display::Primitives;
using namespace Osci::Measurements::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Grid
{
    /// Нарисовать сетку для режима осциллографа
    static void DrawOsci();
    /// Нарисовать сетку для режима тестер-компонента
    static void DrawTester();

    static void DrawRecorder();

    static void DrawGridSignal(int left, int top, int width, int height);

    static void DrawGridSpectrum();
    /// Возвращает расстояние между 
    static int  DeltaHforLineGrid();

    static int  DeltaVforLineGrid();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);

    static float DeltaY();

    static float DeltaX();

    static int MathTop();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Grid::Left()
{
    static const int left[Device::Mode::Size] = { 20, 0, 0, 0 };

    return left[Device::State::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Top()
{
    static const int top[Device::Mode::Size] = { 19, 0, 0, 0 };

    return top[Device::State::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Width()
{
    static const int width[Device::Mode::Size] = { 280, Display::WIDTH - 1, 0, 0 };

    return width[Device::State::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Height()
{
    static const int height[Device::Mode::Size] = { 200, Display::HEIGHT - 1, 0, 0 };

    return height[Device::State::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Bottom()
{
    return Top() + Height();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::FullBottom()
{
    return Bottom();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Right()
{
    return Left() + Width();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Grid::MathTop()
{
    return Top() + Height() / 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathBottom()
{
    return FullBottom();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathHeight()
{
    return Height() / 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::ChannelBottom()
{
    return (Display::IsSeparate()) ? (Top() + Height() / 2) : FullBottom();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::ChannelCenterHeight()
{
    return (Top() + Bottom()) / 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void EmptyFunc()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::Draw()
{
    static const pFuncVV funcs[4] =
    {
        DrawOsci,
        DrawTester,
        EmptyFunc,
        DrawRecorder
    };

    funcs[Device::State::CurrentMode()]();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawOsci()
{
    if (Display::IsSeparate())
    {
        DrawGridSignal(Left(), Top(), Width(), Height() / 2);

        if (FFT_ENABLED)
        {
            DrawGridSpectrum();
        }

        //if (FUNC_MODE_DRAW_IS_ENABLED)
        //{
        //    DrawGridSignal(Left(), Top() + Height() / 2, Width(), Height() / 2);
        //}
        
		HLine(Width()).Draw(Left(), Top() + Height() / 2, Color::FILL);
    }
    else
    {
        DrawGridSignal(Left(), Top(), Width(), Height());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridSignal(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Color::FILL.SetAsCurrent();

    if (top == Top())
    {
		HLine(left - 1).Draw(1, top);
		HLine(Display::WIDTH - right - 4).Draw(right + 2, top);

        VLine line(bottom - top - 4);

        line.Draw(0, top + 2);
        line.Draw(319, top + 2);
    }

    float deltaX = DeltaX() * static_cast<float>(width) / width;
    float deltaY = DeltaY() * static_cast<float>(height) / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    float centerX = static_cast<float>(left + width / 2);
    float centerY = static_cast<float>(top + height / 2);

    Color::GRID.SetAsCurrent();

    if (TYPE_GRID_1)
    {
        DrawGridType1(left, top, right, bottom, centerX, centerY, deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_2)
    {
        DrawGridType2(left, top, right, bottom, static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX), static_cast<int>(stepY));
    }
    else if (TYPE_GRID_3)
    {
        DrawGridType3(left, top, right, bottom, static_cast<int>(centerX), static_cast<int>(centerY), static_cast<int>(deltaX), static_cast<int>(deltaY), static_cast<int>(stepX));
    }
    else
    {
        // для других типов сетки ничего делать не нужно
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static pString strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[MAX_DB_FFT];
        float scale = static_cast<float>(MathHeight()) / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = MathTop() + static_cast<int>(i * scale);

			HLine(256).Draw(Left(), y, Color::GRID);

            Color::FILL.SetAsCurrent();
            String(const_cast<char *>(strs[i])).Draw(3, y - 4);
        }

        Color::FILL.SetAsCurrent();
        String("дБ").Draw(5, MathTop() + 1);
    }
    else // SCALE_FFT_IS_LINEAR
    {
        static pString strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = static_cast<float>(MathHeight()) / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = MathTop() + static_cast<int>(i * scale);

			HLine(256).Draw(Left(), y, Color::GRID);

            String(const_cast<char *>(strs[i])).Draw(5, y - 4, Color::FILL);
        }
    }

    VLine(MathBottom() - MathTop()).Draw(Left() + 256, MathTop(), Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float Grid::DeltaY()
{
    float delta = (FullBottom() - Top()) / 10.0F;
    return Display::IsSeparate() ? (delta / 2.0F) : delta;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float Grid::DeltaX()
{
    float delta = (Right() - Left()) / 14.0F;
    return delta;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = static_cast<uint16>(left + 1);
    for (int i = 1; i < 7; i++)
    {
        masX[i] = static_cast<uint16>(left + static_cast<int>(deltaX * i));
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = static_cast<uint16>(static_cast<int>(centerX) - 8 + i);
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = static_cast<uint16>(centerX + deltaX * (i - 9)); //-V2004
    }
    masX[16] = static_cast<uint16>(right - 1);

    MultiVPointLine(17, masX, static_cast<int>(stepY), DeltaVforLineGrid()).Draw(top + static_cast<int>(stepY), Color::GRID);

    uint8 mas[13];
    mas[0] = static_cast<uint8>(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = static_cast<uint8>(top + static_cast<int>(deltaY * i));
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = static_cast<uint8>(static_cast<int>(centerY) - 6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = static_cast<uint8>(static_cast<int>(centerY) + static_cast<int>(deltaY * (i - 7)));
    }
    mas[12] = static_cast<uint8>(bottom - 1);

    MultiHPointLine(13, mas, static_cast<int>(stepX), DeltaHforLineGrid()).Draw(left + static_cast<int>(stepX), Color::GRID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{
    uint16 masX[15];
    masX[0] = static_cast<uint16>(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = static_cast<uint16>(left + static_cast<int>(deltaX * i));
    }
    masX[14] = static_cast<uint16>(right - 1);
    MultiVPointLine(15, masX, stepY, DeltaVforLineGrid()).Draw(top + stepY, Color::GRID);

    uint8 mas[11];
    mas[0] = static_cast<uint8>(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = static_cast<uint8>(top + static_cast<int>(deltaY * i));
    }
    mas[10] = static_cast<uint8>(bottom - 1);

    MultiHPointLine(11, mas, stepX, DeltaHforLineGrid()).Draw(left + stepX, Color::GRID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX)
{
    HPointLine(right - left - stepX, static_cast<float>(stepX)).Draw(left + stepX, centerY);

    uint8 masY[6] = {
        static_cast<uint8>(top + 1),
        static_cast<uint8>(top + 2),
        static_cast<uint8>(centerY - 1),
        static_cast<uint8>(centerY + 1),
        static_cast<uint8>(bottom - 2),
        static_cast<uint8>(bottom - 1)
    };

    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX, Color::GRID);

    VPointLine(bottom - top - 2 * stepX, static_cast<float>(stepX)).Draw(centerX, top + stepX, Color::GRID);

    uint16 masX[6] =
    {
        static_cast<uint16>(left + 1),
        static_cast<uint16>(left + 2),
        static_cast<uint16>(centerX - 1),
        static_cast<uint16>(centerX + 1),
        static_cast<uint16>(right - 2),
        static_cast<uint16>(right - 1)
    };

    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY, Color::GRID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Grid::DeltaVforLineGrid()
{
    int result = 49;

    if (SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_1_5)
        {
            result = VIEW_MEASURES_BOTH ? 55 : 59;
        }
        else if (NUM_MEASURES_IS_2_5)
        {
            result = VIEW_MEASURES_BOTH ? 69 : 51;
        }
        else if (NUM_MEASURES_IS_3_5)
        {
            result = VIEW_MEASURES_BOTH ? 54 : 68;
        }
        else
        {
            // здесь ничего
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Grid::DeltaHforLineGrid()
{
    int result = 69;

    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_6_1)
        {
            result = 73;
        }
        else if (NUM_MEASURES_IS_6_2)
        {
            result = 83;
        }
        else
        {
            // здесь ничего
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawTester()
{
    Color::FILL.SetAsCurrent();

    Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0);

    float x0 = 0;

    float y0 = 0;

    Color::GRID.SetAsCurrent();

    int x = static_cast<int>(x0 + Display::WIDTH / 2);
    int y = static_cast<int>(y0 + Display::HEIGHT / 2);

    VLine(Display::HEIGHT).Draw(x, 0);

	HLine(Display::WIDTH).Draw(0, y);

    Color::GRID.SetAsCurrent();

    int deltaX = 32;
    int deltaY = 24;

    x += deltaX;

    float deltaPoint = 5.0F;

    VPointLine vLine(Display::HEIGHT, deltaPoint);

    while (x < Display::WIDTH)
    {
        vLine.Draw(x, 0);
        x += deltaX;
    }

    x = static_cast<int>(x0 + Display::WIDTH / 2 - deltaX);

    while (x > 0)
    {
        vLine.Draw(x, 0);
        x -= deltaX;
    }

    y += deltaY;

    HPointLine hLine(Display::WIDTH, deltaPoint);

    while (y < Display::HEIGHT)
    {
        hLine.Draw(0, y);
        y += deltaY;
    }

    y = static_cast<int>(y0 + Display::HEIGHT / 2 - deltaY);

    while (y > 0)
    {
        hLine.Draw(0, y);
        y -= deltaY;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawRecorder()
{
    Color::GRAY_10.SetAsCurrent();

    VLine vLine(Display::HEIGHT - 1);

    int step = 20;

    for (int x = 0; x < Display::WIDTH; x += step)
    {
        vLine.Draw(x, 0);
    }

    HLine hLine(Display::WIDTH - 1);

    for (int y = 0; y < Display::HEIGHT; y += 24)
    {
        hLine.Draw(0, y);
    }

    HLine(Display::WIDTH - 1).Draw(0, Display::HEIGHT / 2, Color::GRAY_20);

    Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0, Color::FILL);
}
