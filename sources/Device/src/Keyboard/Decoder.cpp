#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Decoder.h"
#include "Menu/Menu.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Decoder decoder;

#define SIZE_BUFFER 1024
static uint8 buffer[SIZE_BUFFER];
static int pointer = 0;

pFuncBU8 Decoder::curFunc = 0;
int      Decoder::step = 0;


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
void Decoder::RunStep(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    commands[Command::Number] =
    {
        &Decoder::EmptyFunc,
        &Decoder::ButtonPress,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc,
        &Decoder::EmptyFunc
    };

    if (step == 0)
    {

        if (data < Command::Number)
        {
            curFunc = commands[data].func;
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
bool Decoder::ButtonPress(uint8 data)
{
    static Key button;
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
        Menu::ButtonEvent(KeyEvent(button.value, (TypePress::E)data));
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}
