#include "defines.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Display/Colors.h"
#include "Recorder/Recorder.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


extern wxPaintDC *paintDC;
wxColour colorDraw;


void Transceiver::Init()
{

}


void Transceiver::Send(const uint8 *data, uint num)
{
    if(num == 2 && *data == Command::Paint_SetColor)
    {
        colorDraw = COLOR(data[1]);
        wxBrush brush(colorDraw, wxSOLID);
        if(paintDC)
        {
            paintDC->SetBrush(brush);
        }
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
    return false;
}
