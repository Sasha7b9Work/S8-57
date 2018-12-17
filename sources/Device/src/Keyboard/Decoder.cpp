#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Decoder.h"
#include "Menu/Menu.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder decoder;

#define SIZE_BUFFER 1024
static uint8 buffer[SIZE_BUFFER];
static int pointer = 0;

/// Выполняемая функция
static pFuncBU8 curFunc;
/// Текущий байт выполняемой функции
static int step;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void RunStep(uint8 data);

static bool ButtonPress(uint8);

static bool FuncScreen(uint8);
/// Эту функцию надо вызывать после выполнения последнего шага
static void FinishCommand();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Decoder::AddData(uint8 data)
{
    if (pointer < SIZE_BUFFER)
    {
        buffer[pointer++] = data;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::Update()
{
    if (pointer)
    {
        if(buffer[0] == 1)
        {
            buffer[0] = 1;
        }
        
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
    static const struct StructFunc { pFuncBU8 func; } commands[Command::Number] =
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
        FuncScreen          // Screen
    };

    if (step == 0)
    {

        if (data < Command::Number)
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
        Menu::ButtonEvent(KeyEvent(button, (TypePress::E)data));
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncScreen(uint8)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FinishCommand()
{
    step = 0;
    curFunc = 0;
}
