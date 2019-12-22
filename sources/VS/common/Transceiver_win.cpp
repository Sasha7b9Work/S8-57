#include "defines.h"
#include "common/Command.h"
#include "common/Decoder_d.h"
#include "common/Transceiver.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Colors.h"
#include "../../Panel/src/Display/Font/Font_p.h"
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
    if(data == nullptr)
    {
        Recorder::ReadPoint();
    }
    else if(num == 2 && *data == Command::Paint_SetColor)
    {
        uint color = COLOR(data[1]);
        uint8 b = static_cast<uint8>(color);
        uint8 g = static_cast<uint8>(color >> 8);
        uint8 r = static_cast<uint8>(color >> 16);

        colorDraw = wxColour(r, g, b);

        memDC.SetPen(wxPen(colorDraw));
    }
    else if(*data == Command::Text_Length)
    {
        uint size = static_cast<uint>(*(data + 1) + 1);

        char *text = new char[size];
        std::memcpy(text, data + 2, size - 1);
        text[size - 1] = '\0';

        WorkerLengthText::SetLength(static_cast<uint8>(PFont::GetLengthText(text)));

        delete[]text;
    }
    else if(*data == Command::Paint_SetTextSpacing)
    {
        PFont::SetSpacing(*(data + 1));
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
