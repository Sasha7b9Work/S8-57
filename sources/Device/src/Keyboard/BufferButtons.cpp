#include "defines.h"
#include "BufferButtons.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"



enum
{
    SIZE = 100
};

/// Здесь хранятся все события
static KeyEvent buffer[SIZE];
/// Предыдущее попавшее в буфер событие клавиатуры
static KeyEvent prevPushEvent;
/// Позиция первого свободного места в буфере. Когда end == 0, буфер пустой
static int end;
/// Позиция первого значащего события в буфере.
static int start;




void BufferButtons::Push(KeyEvent event)
{
    if ((event.key == prevPushEvent.key) &&             // Если отпущена кнпока, которая раньше прислала "длинное" нажатие,
        (prevPushEvent.type == TypePress::Long) &&
        (event.type == TypePress::Release))
    {
        return;                                         // то пропустим это событие - кнопка уже отработала
    }

    PageMultimeter::DecodePassword(event);

    /// \todo На звуке иногда виснет при длительном удержании кнопки смещения. Вызвано ошибками при передаче данных
    Beeper::Beep(event.type);

    if(end == SIZE)
    {
        LOG_ERROR("Буфер переполнен");
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
        start = end = 0;
    }

    return result;
}


bool BufferButtons::IsEmpty()
{
    return end == 0;
}

