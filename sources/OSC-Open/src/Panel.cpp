#include "Panel.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/Keyboard.h"
#include "Hardware/FSMC.h"
#include "Utils/Decoder.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Panel::Init()
{
    Display::Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Panel::Update()
{
    /*
    uint8 data = FSMC::ReadByte();
    Decoder::AddData(data);
    FSMC::WriteBuffer();
    */

    Painter::BeginScene(Color::BLACK);

    Painter::FillRegion(0, 0, 320, 240, Color::WHITE);

    Keyboard::Draw();

    Painter::EndScene();
}
