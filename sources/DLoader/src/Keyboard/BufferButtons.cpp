#include "defines.h"
#include "BufferButtons.h"
#ifdef DEVICE
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"
#endif



static const int SIZE = 100;

/// Здесь хранятся все события
static KeyEvent buffer[SIZE];
/// Предыдущее попавшее в буфер событие клавиатуры
static KeyEvent prevPushEvent;
/// Позиция первого свободного места в буфере. Когда end == 0, буфер пустой
static int end;
/// Позиция первого значащего события в буфере.
static int start;




void BufferButtons::Push(KeyEvent event) //-V2506
{
    if ((event.key == prevPushEvent.key) &&             // Если отпущена кнпока, которая раньше прислала "длинное" нажатие,
        prevPushEvent.IsLong() &&
        event.IsRelease())
    {
        return;                                         // то пропустим это событие - кнопка уже отработала
    }

    if(end == SIZE)
    {
        return;
    }

    buffer[end++] = event;

    prevPushEvent = event;
}


KeyEvent BufferButtons::Extract()
{
    KeyEvent result = buffer[start];

    start++;
    if(start == end)
    {
        start = 0;
        end = 0;
    }

    return result;
}


bool BufferButtons::IsEmpty()
{
    return end == 0;
}

