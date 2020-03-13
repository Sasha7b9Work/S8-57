#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "common/Display/Font/Font_d.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include <cstring>


static void EmptyFunc() { }


// Дополнительная функция рисования. Выполняется после стандартной отрисовки, но перед вызовом EndScene;
volatile static pFuncVV funcAdditionDraw = EmptyFunc;

// true означает, что происходит процесс отрисовки
static bool inStateDraw = false;


void Display::Init()
{
    for(uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        Color(i).LoadValueRGB();
    }

    LoadBrightness();
}


void Display::Update()
{
    static int x = 0;
    static int y = 0;

    Painter::BeginScene(Color::WHITE);

    Region(20, 20).Fill(x++, y++, Color::BLACK);

    if(x > 100)
    {
        x = 0;
        y = 0;
    }

    Painter::EndScene();
}


bool Display::InProcess()
{
    return inStateDraw;
}


void Display::LoadBrightness()
{
    HAL_BUS::Panel::Send(Command::Display_Brightness, 110);
}


bool Display::IsRun()
{
    return true;
}
