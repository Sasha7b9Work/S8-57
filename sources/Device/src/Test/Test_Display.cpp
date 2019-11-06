#include "defines.h"
#include "Display/Display.h"
#include "Test/Test.h"
#include "Hardware/HAL/HAL.h"


static void Update()
{

}


void Test::Display::Init()
{
    ::Display::SetDrawMode(DrawMode::Hand, Update);
}


void Test::Display::DeInit()
{
}
