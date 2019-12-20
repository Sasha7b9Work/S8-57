#include "defines.h"
#include "log.h"
#include "Decoder_d.h"
#ifdef DEVICE
#include "Menu/Menu.h"
#include "Utils/Debug.h"
#endif
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Keyboard/BufferButtons.h"
#include <cstdlib>


int      DDecoder::pointer = 0;
pFuncBU8 DDecoder::curFunc = nullptr;
uint8   *DDecoder::pixels = nullptr;
int      DDecoder::step = 0;
uint8    DDecoder::buffer[DDecoder::SIZE_BUFFER];


void DDecoder::AddData(uint8 data)
{
    if (pointer < SIZE_BUFFER)
    {
        buffer[pointer++] = data;
    }
    else
    {
        LOG_WRITE("Нет места в приёмном буфере");
    }
}


void DDecoder::Update()
{
    if (pointer)
    {
        for (int i = 0; i < pointer; i++)
        {
            RunStep(buffer[i]);
        }
        pointer = 0;
    }
}


static bool EmptyFunc(uint8)
{
    return true;
}


void DDecoder::RunStep(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Count] =
    {
        EmptyFunc,      // None,
        ButtonPress,    // ButtonPress,
        EmptyFunc,      // Paint_BeginScene,
        EmptyFunc,      // Paint_EndScene,
        EmptyFunc,      // Paint_SetColor,
        EmptyFunc,      // Paint_FillRegion,
        EmptyFunc,      // Paint_DrawText,
        EmptyFunc,      // Paint_SetPalette,
        EmptyFunc,      // Paint_DrawRectangle,
        EmptyFunc,      // Paint_DrawVLine,
        EmptyFunc,      // Paint_DrawHLine,
        EmptyFunc,      // Paint_SetFont,
        EmptyFunc,      // Paint_SetPoint,
        EmptyFunc,      // Paint_DrawLine,
        EmptyFunc,      // Paint_TesterLines,
        EmptyFunc,      // Paint_DrawBigText,
        FuncScreen,     // Screen
        EmptyFunc,      // Paint_VPointLine
        EmptyFunc,      // Paint_HPointLine
        EmptyFunc,      // Paint_SetMonoSpaceFont
        EmptyFunc,      // Paint_SetTextSpacing
        AddToConsole,   // AddToConsole
        FuncLengthText  // Text_Length
    };

    if (step == 0)
    {
        if (data < Command::Count)
        {
            curFunc = commands[data].func;
            if (curFunc == 0)
            {
                LOG_ERROR("Нет обработчика");
            }
        }
        else
        {
            FinishCommand();
            return;
        }
    }

    if (curFunc)
    {
        if (curFunc(data))
        {
            FinishCommand();
        }
        else
        {
            ++step;
        }
    }
    else
    {
        FinishCommand();
    }
}


bool DDecoder::ButtonPress(uint8 data)
{
    static Key::E button;
    if (step == 0)
    {
        return false;
    }
    else if (step == 1)
    {
        button = static_cast<Key::E>(data);
        return false;
    }
    else
    {
        BufferButtons::Push(KeyEvent(button, static_cast<TypePress::E>(data)));
    }

    return true;
}


void DDecoder::SetBufferForScreenRow(uint8 *_pixels)
{
    pixels = _pixels;
}


bool DDecoder::FuncScreen(uint8 data)
{
    static int numString = 0;

    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        numString = data;
        return false;
    }

    if (step < 321)
    {
        pixels[step - 2] = data;
        return false;
    }

    if (step == 321)
    {
        Display::SaveRow(numString);
    }


    return true;
}


bool DDecoder::FuncLengthText(uint8 data)
{
    if(step == 0)
    {
        return false;
    }

    if(step == 1)
    {
        WorkerLengthText::SetLength(data);
    }

    return true;
}


bool DDecoder::AddToConsole(uint8 data)
{
    static char *text = nullptr;        // Здесь будет храниться принятая строка

    static uint8 allSymbols = 0;        // Количество символов в строке без учёта завершающего нуля

    static uint8 recvSymbols = 0;       // Столько символов уже принято

    if (step == 0)
    {
        return false;
    }

    if (step == 1)
    {
        allSymbols = data;
        text = static_cast<char *>(std::malloc(allSymbols + 1U));
        recvSymbols = 0;
        return false;
    }

    text[recvSymbols++] = static_cast<char>(data);

    if (recvSymbols == allSymbols)
    {
        LOG_WRITE(text);
        std::free(text);
    }

    return (recvSymbols == allSymbols);
}


void DDecoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}
