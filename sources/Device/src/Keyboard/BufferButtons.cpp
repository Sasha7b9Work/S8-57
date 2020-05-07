#include "defines.h"
#include "BufferButtons.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMultimeter.h"
#include "Menu/Pages/Include/PageService.h"


static const int SIZE = 100;

static KeyEvent buffer[SIZE];               // Здесь хранятся все события
static KeyEvent prevPushEvent;              // Предыдущее попавшее в буфер событие клавиатуры
static int end;                             // Позиция первого свободного места в буфере. Когда end == 0, буфер пустой
static int start;                           // Позиция первого значащего события в буфере.
static uint timeLastControl = 0xFFFFFFFFU;  // Возвращает время


void BufferButtons::Push(KeyEvent event)
{
    Display::Breaker::PowerOn();

    timeLastControl = TIME_MS;

    if ((event.key == prevPushEvent.key) &&             // Если отпущена кнпока, которая раньше прислала "длинное" нажатие,
        prevPushEvent.IsLong() &&
        event.IsRelease())
    {
        return;                                         // то пропустим это событие - кнопка уже отработала
    }

    PageMultimeter::DecodePassword(event);

    PageService::DecodePassword(event);

    // \todo На звуке иногда виснет при длительном удержании кнопки смещения. Вызвано ошибками при передаче данных
    Beeper::Beep(event.type);

    if(end == SIZE)
    {
//        LOG_ERROR("Буфер переполнен");
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


uint BufferButtons::TimeAfterControlMS()
{
    return TIME_MS - timeLastControl;
}
