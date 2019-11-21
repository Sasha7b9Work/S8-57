#include "defines.h"
#include "Hardware/VCP.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/Keyboard.h"
#include "SCPI/HeadSCPI.h"
#include <cstring>


static const char *FuncIDN(const char *);
static const char *FuncKeyPress(const char *);
static const char *FuncKeyLong(const char *);


static const char *keyNames[Key::Count] =
{
    "None",
    "FUNCTION",
    "MEASURE",
    "MEMORY",
    "SERVICE",
    "A",
    "B",
    "TIME",
    "START",
    "TRIG",
    "DISPLAY",
    "RANGEA+",
    "RANGEA-",
    "RSHIFTA+",
    "RSHIFTA-",
    "RANGEB+",
    "RANGEB-",
    "RSHIFTB+",
    "RSHIFTB-",
    "TBASE+",
    "TBASE-",
    "TSHIFT+",
    "TSHIFT-",
    "TRIGLEV+",
    "TRIGLEV-",
    "LEFT",
    "RIGHT",
    "UP",
    "DOWN",
    "ENTER",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5"
};


static const StructSCPI key[] =
{
    {StructSCPI::Leaf, ":PRESS:", reinterpret_cast<void *>(FuncKeyPress)},
    {StructSCPI::Leaf, ":LONG:",  reinterpret_cast<void *>(FuncKeyLong)},
    {StructSCPI::Empty}
};


const StructSCPI head[] =
{
    {StructSCPI::Leaf, "*IDN?", reinterpret_cast<void *>(FuncIDN)},
    {StructSCPI::Node, ":KEY",  const_cast<StructSCPI *>(key)},
    {StructSCPI::Empty}
};



static const char *FuncIDN(const char *buffer)
{
    if (SCPI::IsLineEnding(&buffer))
    {
        SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

        return buffer;
    }

    return nullptr;
}


static const char *FuncKeyPress(const char *buffer)
{
    for (int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if (end)
        {
            if (SCPI::IsLineEnding(&end))
            {
                BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
                BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Release));
                return end;
            }
            else
            {
                break;
            }
        }
    }

    return nullptr;
}


static const char *FuncKeyLong(const char *buffer)
{
    for (int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if (end)
        {
            if (SCPI::IsLineEnding(&end))
            {
                BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
                BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Long));
                return end;
            }
            else
            {
                break;
            }
        }
    }

    return nullptr;
}
