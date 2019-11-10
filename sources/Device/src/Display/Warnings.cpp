#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Utils/Queue.h"
#include <cstring>


Queue<WarningStruct> warnings;


/// Удалить предупреждения с истёкшим времени жизни
static void RemoveOld();
/// Возвращает true, если последнее сообщение в очереди такое же
static bool BackMessageSame(const char *message);

static void DrawMessages();



void Warnings::AddWarning(const char *warning)
{
    if (BackMessageSame(warning))
    {
        warnings.Back();
    }

    warnings.Push(WarningStruct(warning));
}


static bool BackMessageSame(const char *message)
{
    if (warnings.IsEmpty())
    {
        return false;
    }

    return (std::strcmp(message, warnings[warnings.Size() - 1].message.CString()) == 0);
}


void Warnings::Draw()
{
    if(warnings.Size())
    {
        RemoveOld();

        if(warnings.Size())
        {
            DrawMessages();
        }
    }
}


static void RemoveOld()
{
    while (!warnings.IsEmpty() && warnings[0].IsDead())
    {
        warnings.Front();
    }
}


static void DrawMessages()
{
    Font::SetCurrent(TypeFont::_8);

    int y = Grid::BottomForWarnings();                   // Координата y нижнего левого угла прямоугольника, в котором будет отрисовано очередное сообщение

    for (int i = warnings.Size() - 1; i >= 0; i--)
    {
        y -= warnings[i].Height(Grid::Width());

        if (y < Grid::Top())
        {
            break;
        }

        warnings[i].Draw(Grid::Left(), y, Grid::Width());
    }
}


bool Warnings::IsShown()
{
    return (warnings.Size() != 0);
}


WarningStruct::WarningStruct(const char *msg)
{
    message.Set(TypeConversionString::FirstUpper, msg);
    timeStart = TIME_MS;
}


WarningStruct::WarningStruct(const WarningStruct &s)
{
    message.Set(TypeConversionString::None, s.message.CString());
    timeStart = s.timeStart;
}


WarningStruct &WarningStruct::operator=(const WarningStruct &s)
{
    timeStart = s.timeStart;
    message.Set(TypeConversionString::None, s.message.CString());
    return *this;
}


WarningStruct::~WarningStruct()
{
    message.Free();
}


bool WarningStruct::IsDead() const
{
    return (TIME_MS - timeStart) > 3500;
}

int WarningStruct::Height(int) const
{
    return 12;
}

void WarningStruct::Draw(int x, int y, int width) const
{
    Text(message).DrawInBoundedRectWithTransfers(x, y, width, Color::FLASH_10, Color::FLASH_01, Color::FILL);
}
