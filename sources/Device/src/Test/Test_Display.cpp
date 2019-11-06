#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/HAL/HAL.h"
#include "Test/Test.h"
#include "Utils/Values.h"


static void Update()
{
    Painter::BeginScene(Color::BACK);

    Text("Выполняется тест флеш-памяти").Draw(30, 30, Color::FILL);

    Painter::EndScene();
}


void Test::Display::Init()
{
    ::Display::SetDrawMode(DrawMode::Hand, Update);
}


void Test::Display::DeInit()
{
    ::Display::SetDrawMode(DrawMode::Auto);
}
