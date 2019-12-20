#include "defines.h"
#include "BufferButtons.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


//                         RL    SL
static const Key::E controls[6][8] =
{ //          SL0/SL6                SL1/SL7                  SL2                     SL3                     SL4                     SL5              
    {Key::None,       Key::F3,           Key::Down,        Key::None,        Key::TrigLevLess, Key::TrigLevMore,
     Key::RangeLessB, Key::RShiftMoreB},                                                                                            // RL0

    {Key::F1,         Key::F4,              Key::Right,       Key::Enter,       Key::Start,       Key::Trig,
     Key::RangeMoreB, Key::RShiftLessB},                                                                                            // RL1

    {Key::F2,         Key::F5,              Key::Up,          Key::Left,        Key::None,        Key::None,
     Key::None,       Key::ChannelB},                                                                                               // RL2

    {Key::Function,   Key::Service,      Key::RangeLessA,  Key::RangeMoreA,  Key::None,        Key::TShiftLess,
     Key::None,       Key::None},                                                                                                   // RL3

    {Key::Measure,   Key::None,         Key::ChannelA,    Key::None,        Key::TBaseMore,   Key::TShiftMore,
     Key::None,       Key::None},                                                                                                   // RL4

    {Key::Memory,     Key::Display,      Key::RShiftMoreA, Key::RShiftLessA, Key::Time,        Key::TBaseLess,
     Key::None,       Key::None}                                                                                                    // RL5
};


static const int SIZE = 100;

/// Здесь хранятся все события
static KeyEvent buffer[SIZE];
/// Предыдущее попавшее в буфер событие клавиатуры
static KeyEvent prevPushEvent;
/// Позиция первого свободного места в буфере. Когда end == 0, буфер пустой
static int end;
/// Позиция первого значащего события в буфере.
static int start;


static void FuncRLSL(Key::E key, int *r, int *s)
{
    for(int rl = 0; rl < 6; rl++)
    {
        for(int sl = 0; sl < 8; sl++)
        {
            if(key == controls[rl][sl])
            {
                *r = rl;
                *s = sl;
            }
        }
    }
}



void BufferButtons::Push(KeyEvent event)
{
    if ((event.key == prevPushEvent.key) &&             // Если отпущена кнпока, которая раньше прислала "длинное" нажатие,
        prevPushEvent.IsLong() &&
        event.IsRelease())
    {
        return;                                         // то пропустим это событие - кнопка уже отработала
    }

    int rl = -1;
    int sl = -1;

    FuncRLSL(event.key, &rl, &sl);

    LOG_WRITE("%s %s       %02d %02d", Key(event.key).Name(), TypePress(event.type).ToChar(), rl, sl);

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
        start = 0;
        end = 0;
    }

    return result;
}


bool BufferButtons::IsEmpty()
{
    return end == 0;
}

