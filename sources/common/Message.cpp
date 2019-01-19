#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "Message.h"
#include <cstdlib>
#include <cstring>
#endif
#include <cstdlib>
#include "structs.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Message::Message() : allocated(0), used(0), data(0)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 v0) : allocated(0), used(0), data(0)
{
    if (Allocate(size))
    {
        PutByte(v0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 v0, uint8 v1) : allocated(0), used(0), data(0)
{
    if (Allocate(size))
    {
        PutByte(v0);
        PutByte(v1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 v0, uint16 v1, uint8 v2) : allocated(0), used(0), data(0)
{
    if (Allocate(size))
    {
        PutByte(v0);
        PutHalfWord(v1);
        PutByte(v2);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 v0, uint8 v1, uint v2) : allocated(0), used(0), data(0)
{
    if (Allocate(size))
    {
        PutByte(v0);    // -V525
        PutByte(v1);    // -V525
        PutWord(v2);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 v0, uint16 v1, uint8 v2, uint8 v3) : allocated(0), used(0), data(0)
{
    if (Allocate(size))
    {
        PutByte(v0);
        PutHalfWord(v1);
        PutByte(v2);
        PutByte(v3);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint8 v0, uint8 v1, uint8 v2, uint8 *b0, uint16 s0, uint8 *b1, uint16 s1) : allocated(0), used(0), data(0)
{   //          v0 | v1 | v2 | b0[s0] | b1[s1]
    uint size = 1U + 1U + 1U + s0 +     s1;

    if (Allocate(size))
    {
        PutByte(v0);
        PutByte(v1);
        PutByte(v2);
        for (int i = 0; i < s0; i++)
        {
            PutByte(b0[i]);
        }
        for (int i = 0; i < s1; i++)
        {
            PutByte(b1[i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 v0, uint16 v1, uint8 v2, uint16 v3, uint8 v4) : allocated(0), used(0), data(0)
{
    if (Allocate(size))
    {
        PutByte(v0);
        PutHalfWord(v1);
        PutByte(v2);
        PutHalfWord(v3);
        PutByte(v4);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint8 v0, uint16 v1, uint8 v2, char *string) : allocated(0), used(0), data(0)
{
    //          v0  | v1  | v2  | размер_строки | string
    uint size = 1 +   2 +   1 +   1 +             std::strlen(string); //-V2513

    if (Allocate(size))
    {
        PutByte(v0);
        PutHalfWord(v1);
        PutByte(v2);
        PutByte((uint8)std::strlen(string)); //-V2513

        uint8 *pointer = Data(5);
        while (*string)
        {
            *pointer++ = (uint8)*string++;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::~Message()
{
    Free();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutByte(uint8 v0)
{
    if (used < allocated)
    {
        data[used++] = v0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutHalfWord(uint16 v)
{
    if (used + 1 < allocated)
    {
        BitSet16 bs(v);
        PutByte(bs.byte0);
        PutByte(bs.byte1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutWord(uint v)
{
    if (used + 3 < allocated)
    {
        BitSet32 bs(v);
        for (int i = 0; i < sizeof(uint); i++)
        {
            PutByte(bs.byte[i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Message::Allocate(uint size)
{
    Free();
    data = (uint8 *)std::malloc(size);  // -V106
    if (data)
    {
        allocated = size;
    }
    return data != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::Free()
{
    if (allocated)
    {
        std::free(data);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::Create::DrawBigText(Message &message, int x, int y, uint8 sizeSymbol, pString string)
{
    uint numSymbols = std::strlen(string); //-V2513
    //          command | x | y | размер_символов | размер_строки | string
    uint size = 1 +       2 + 1 + 1 +               1;

    if (message.Allocate(size))
    {
        message.PutByte(Command::Paint_DrawBigText);
        message.PutHalfWord((uint16)x);
        message.PutByte((uint8)y);
        message.PutByte(sizeSymbol);
        message.PutByte((uint8)numSymbols);

        uint8 *pointer = message.Data(6);

        uint8 *text = (uint8 *)string;

        while (*text)
        {
            *pointer++ = *text++;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::Create::DrawHLine(Message &message, int y, int x0, int x1)
{
    if (message.Allocate(6))
    {
        message.PutByte(Command::Paint_DrawHLine);
        message.PutByte((uint8)y);
        message.PutHalfWord((uint16)x0);
        message.PutHalfWord((uint16)x1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::Create::DrawRectangle(Message &message, int x, int y, int width, int height)
{
    if (message.Allocate(7))
    {
        message.PutByte(Command::Paint_DrawRectangle);
        message.PutHalfWord((uint16)x);
        message.PutByte((uint8)y);
        message.PutHalfWord((uint16)width);
        message.PutByte((uint8)height);
    }
}
