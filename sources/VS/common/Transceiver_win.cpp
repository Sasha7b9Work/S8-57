#include "defines.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Display/Colors.h"
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


wxColour colorDraw;


extern wxMemoryDC memDC;


void Transceiver::Init()
{

}


void Transceiver::Send(const uint8 *data, uint num)
{
    if(num == 2 && *data == Command::Paint_SetColor)
    {
        uint color = COLOR(data[1]);
        uint8 b = static_cast<uint8>(color);
        uint8 g = static_cast<uint8>(color >> 8);
        uint8 r = static_cast<uint8>(color >> 16);

        colorDraw = wxColour(r, g, b);

        memDC.SetPen(wxPen(colorDraw));
    }
    else
    {
        Recorder::ReadPoint();
    }
}


void Transceiver::Send(uint8)
{

}


bool Transceiver::Receive()
{
    Osci::ReadPointP2P();

    return false;
}
