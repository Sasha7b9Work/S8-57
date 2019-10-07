#include "defines.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Utils/Queue.h"
#include <cstring>


Queue<WarningStruct> warnings;


/// Удалить предупреждения с истёкшим времени жизни
static void RemoveOld();
/// Возвращает true, если последнее сообщение в очереди такое же
static bool BackMessageSame(const char *message);

static void DrawMessages(int left, int down, int width, int height);



void Warnings::AddWarning(const char *warning)
{
    if (BackMessageSame(warning))
    {
        warnings[warnings.Size() - 1].timeStart = TIME_MS;
    }
    else
    {
        warnings.Push(WarningStruct(warning));
    }
}


static bool BackMessageSame(const char *message)
{
    if (warnings.IsEmpty())
    {
        return false;
    }

    return std::strcmp(message, warnings[warnings.Size() - 1].message) != 0;
}


void Warnings::Show(int left, int down, int width, int height)
{
    RemoveOld();

    if (!warnings.IsEmpty())
    {
        DrawMessages(left, down, width, height);
    }
}


static void RemoveOld()
{
    while (!warnings.IsEmpty() && warnings[0].IsDead())
    {
        warnings.Front();
    }
}


static void DrawMessages(int left, int down, int width, int height)
{
    int y = down;                   // Координата y нижнего левого угла прямоугольника, в котором будет отрисовано очередное сообщение

    int size = warnings.Size();

    for (int i = size - 1; i >= 0; i--)
    {
        int height = warnings[i].Height(width);
    }
}


WarningStruct::WarningStruct(const char *msg) : message(msg)
{
    timeStart = TIME_MS;
}


bool WarningStruct::IsDead()
{
    return (TIME_MS - timeStart) > 3000;
}

int WarningStruct::Height(int width) const
{
    return 10;
}
