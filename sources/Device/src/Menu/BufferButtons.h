#pragma once
#include "Keyboard/Keyboard.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BufferButtons
{
public:
    static void Push(KeyEvent str);
    static KeyEvent Extract();
    static bool IsEmpty();

private:
    enum
    {
        SIZE = 100
    };

    /// Здесь хранятся все события
    static KeyEvent buffer[SIZE];
    /// Позиция первого свободного места в буфере. Когда end == 0, буфер пустой
    static int end;
    /// Позиция первого значащего события в буфере.
    static int start;
};
