#include "defines.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Utils/Queue.h"
#include <cstring>


Queue<WarningStruct> warnings;


/// ”далить предупреждени€ с истЄкшим времени жизни
static void RemoveOld();
/// ¬озвращает true, если врем€ жизни первого сообщени€ в очереди истекло
static bool FrontMessageIsDead();
/// ¬озвращает true, если последнее сообщение в очереди такое же
static bool BackMessageSame(const char *message);

static void DrawMessages(int left, int down, int width, int height);
/// ¬озвращает высоту отрисованного в области шириной weidth сообщени€
static int HeightMessage(const char *message, int width);



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
    while (!warnings.IsEmpty() && FrontMessageIsDead())
    {
        warnings.Front();
    }
}


static bool FrontMessageIsDead()
{
    return (TIME_MS - warnings[0].timeStart) > 3000;
}


static void DrawMessages(int left, int down, int width, int height)
{
    int y = down;                   //  оордината y нижнего левого угла пр€моугольника, в котором будет отрисовано очередное сообщение

    int size = warnings.Size();

    for (int i = size - 1; i >= 0; i--)
    {
        const char *warning = warnings[i].message;

    }
}


WarningStruct::WarningStruct(const char *msg) : message(msg)
{
    timeStart = TIME_MS;
}
