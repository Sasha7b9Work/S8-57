#include "defines.h"
#include "log.h"
#include "Decoder_d.h"
#ifdef DEVICE
#include "Hardware/Memory/RAM.h"
#include "Menu/Menu.h"
#endif
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Keyboard/BufferButtons.h"
#include "Utils/Debug.h"
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
}


void DDecoder::Update()
{
    if(pointer == 0)
    {

    }
    else if(pointer == 1)
    {
        //LOG_WRITE("     run step %d", buffer[0]);
    }
    else if(pointer == 2)
    {
        //LOG_WRITE("     run steps %d %d", buffer[0], buffer[1]);
    }
    else if(pointer == 3)
    {
        //LOG_WRITE("     run steps %d %d %d", buffer[0], buffer[1], buffer[2]);
    }
    else
    {
        //LOG_WRITE("     выполнить нужно больше трех шагов");
    }

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
    DEBUG_TRACE;
    return true;
}

static bool finishedCommand = false;


void DDecoder::RunStep(uint8 data)
{
    static uint8 com[10];
    static int size = 0;

    if(finishedCommand)
    {
        if(size)
        {
            if(size == 1)
            {
                LOG_WRITE("Предыдущая команда %d", com[0]);
            }
            else if(size == 2)
            {
                if(com[0] != 22)
                {
                    LOG_WRITE("Предыдущая команда %d %d", com[0], com[1]);
                }
            }
            else if(size == 3)
            {
                LOG_WRITE("Команда %d %d %d", com[0], com[1], com[2]);
            }
            else
            {
                LOG_WRITE("Больше трёх байт в команде");
            }

            finishedCommand = false;
            size = 0;
        }
        else
        {
            LOG_WRITE("Нет комманды");
        }
    }

    com[size++] = data;

    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Count] =
    {
        EmptyFunc,      // None,                    // 00
        ButtonPress,    // ButtonPress,             // 01
        EmptyFunc,      // Paint_BeginScene,        // 02
        EmptyFunc,      // Paint_EndScene,          // 03
        EmptyFunc,      // Paint_SetColor,          // 04
        EmptyFunc,      // Paint_FillRegion,        // 05
        EmptyFunc,      // Paint_DrawText,          // 06
        EmptyFunc,      // Paint_SetPalette,        // 07
        EmptyFunc,      // Paint_DrawRectangle,     // 08
        EmptyFunc,      // Paint_DrawVLine,         // 09
        EmptyFunc,      // Paint_DrawHLine,         // 10
        EmptyFunc,      // Paint_SetFont,           // 11
        EmptyFunc,      // Paint_SetPoint,          // 12
        EmptyFunc,      // Paint_DrawLine,          // 13
        EmptyFunc,      // Paint_TesterLines,       // 14
        EmptyFunc,      // Paint_DrawBigText,       // 15
        FuncScreen,     // Screen                   // 16
        EmptyFunc,      // Paint_VPointLine         // 17
        EmptyFunc,      // Paint_HPointLine         // 18
        EmptyFunc,      // Paint_SetMonoSpaceFont   // 19
        EmptyFunc,      // Paint_SetTextSpacing     // 20
        AddToConsole,   // AddToConsole             // 21
        FuncLengthText  // Text_Length              // 22
    };

    if (step == 0)
    {
        if (data < Command::Count)
        {
//            DEBUG_TRACE;
            curFunc = commands[data].func;
//            DEBUG_TRACE;
        }
        else
        {
            DEBUG_MESSAGE("Нет такой функции");
            LOG_WRITE("Нет такой функции");
            while(true)
            {

            };
            DEBUG_TRACE;
            FinishCommand(__FUNCTION__, __LINE__);
            DEBUG_TRACE;
            return;
        }
    }

    if (curFunc)
    {
        if (curFunc(data))
        {
            VALIDATE_RAM;
            FinishCommand(__FUNCTION__, __LINE__);
            VALIDATE_RAM;
        }
        else
        {
//            DEBUG_TRACE;
            ++step;
        }
    }
    else
    {
        DEBUG_TRACE;
        FinishCommand(__FUNCTION__, __LINE__);
        DEBUG_TRACE;
    }
}


bool DDecoder::ButtonPress(uint8 data)
{
//    DEBUG_TRACE;
    static Key::E button;
    if (step == 0)
    {
//        DEBUG_TRACE;
        return false;
    }
    else if (step == 1)
    {
//        DEBUG_TRACE;
        button = static_cast<Key::E>(data);
        if(button >= Key::Count)
        {
            LOG_WRITE("Неправильная кнопка %d", button);
        }
        return false;
    }
    else
    {

        TypePress::E type = static_cast<TypePress::E>(data);
        if(type >= TypePress::Count)
        {
            LOG_WRITE("Неправильный тип %d", type);
        }
//        DEBUG_TRACE;
        BufferButtons::Push(KeyEvent(button, type));
//        DEBUG_TRACE;
    }

//    DEBUG_TRACE;
    return true;
}


void DDecoder::SetBufferForScreenRow(uint8 *_pixels)
{
    pixels = _pixels;
}


bool DDecoder::FuncScreen(uint8 data)
{
    DEBUG_TRACE;
    static int numString = 0;

    if (step == 0)
    {
        DEBUG_TRACE;
        return false;
    }

    DEBUG_TRACE;
    if (step == 1)
    {
        DEBUG_TRACE;
        numString = data;
        return false;
    }

    DEBUG_TRACE;
    if (step < 321)
    {
        DEBUG_TRACE;
        pixels[step - 2] = data;
        DEBUG_TRACE;
        return false;
    }

    DEBUG_TRACE;
    if (step == 321)
    {
        DEBUG_TRACE;
        Display::SaveRow(numString);
    }

    DEBUG_TRACE;

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
        //LOG_WRITE("%s step = %d, data = %d", __FUNCTION__,   step, data);
        WorkerLengthText::SetLength(data);
    }

    return true;
}


bool DDecoder::AddToConsole(uint8 data)
{
    DEBUG_TRACE;
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
        std::free(text);
    }

    DEBUG_TRACE;
    return (recvSymbols == allSymbols);
}


void DDecoder::FinishCommand(const char *, int)
{
//    LOG_WRITE("FinishCommand from %s : %d", func, line);
//    LOG_WRITE("%s step = %d, curFunc = %d", __FUNCTION__, step, curFunc);
    step = 0;
    curFunc = 0;
    finishedCommand = true;
}
