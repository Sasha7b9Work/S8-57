#include "defines.h"
#include "log.h"
#include "DecoderDevice.h"
#include "Menu/Menu.h"
#include "Utils/Debug.h"
#include "Display/Painter.h"
#include "Menu/BufferButtons.h"
#include <cstdlib>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder decoder;

#define SIZE_BUFFER 1024
static uint8 buffer[SIZE_BUFFER];
static int pointer = 0;

/// Выполняемая функция
static pFuncBU8 curFunc;
/// Текущий байт выполняемой функции
static int step;

static uint8 *pixels = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void RunStep(uint8 data);

static bool ButtonPress(uint8);

static bool FuncScreen(uint8);
/// Эту функцию надо вызывать после выполнения последнего шага
static void FinishCommand();
/// Добавляет текстовую строку в консоль
static bool AddToConsole(uint8);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Decoder::AddData(uint8 data)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::Update()
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RunStep(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Size] =
    {
        EmptyFuncBtU8,      // None,
        ButtonPress,        // ButtonPress,
        EmptyFuncBtU8,      // Paint_BeginScene,
        EmptyFuncBtU8,      // Paint_EndScene,
        EmptyFuncBtU8,      // Paint_SetColor,
        EmptyFuncBtU8,      // Paint_FillRegion,
        EmptyFuncBtU8,      // Paint_DrawText,
        EmptyFuncBtU8,      // Paint_SetPalette,
        EmptyFuncBtU8,      // Paint_DrawRectangle,
        EmptyFuncBtU8,      // Paint_DrawVLine,
        EmptyFuncBtU8,      // Paint_DrawHLine,
        EmptyFuncBtU8,      // Paint_SetFont,
        EmptyFuncBtU8,      // Paint_SetPoint,
        EmptyFuncBtU8,      // Paint_DrawLine,
        EmptyFuncBtU8,      // Paint_TesterLines,
        EmptyFuncBtU8,      // Paint_DrawBigText,
        FuncScreen,         // Screen
        EmptyFuncBtU8,      // Paint_VPointLine
        EmptyFuncBtU8,      // Paint_HPointLine
        EmptyFuncBtU8,      // Paint_SetMonoSpaceFont
        EmptyFuncBtU8,      // Paint_SetTextSpacing
        AddToConsole        // AddToConsole
    };

    if (step == 0)
    {
        if (data < Command::Size)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ButtonPress(uint8 data)
{
    static Key::E button;
    if (step == 0)
    {
        return false;
    }
    else if (step == 1)
    {
        button = (Key::E)data;
        return false;
    }
    else
    {
        BufferButtons::Push(KeyEvent(button, (TypePress::E)data));
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::SetBufferForScreenRow(uint8 *_pixels)
{
    pixels = _pixels;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncScreen(uint8 data)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool AddToConsole(uint8 data)
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
        text = (char *)std::malloc(allSymbols + 1U);
        recvSymbols = 0;
        return false;
    }

    text[recvSymbols++] = (char)data;

    if (recvSymbols == allSymbols)
    {
        LOG_WRITE(text);
        std::free(text);
    }

    return (recvSymbols == allSymbols);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}

