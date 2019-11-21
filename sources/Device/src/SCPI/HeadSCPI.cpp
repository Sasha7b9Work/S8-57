#include "defines.h"
#include "Hardware/VCP.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/Keyboard.h"
#include "SCPI/HeadSCPI.h"
#include <cstring>


static const char *FuncIDN(const char *, ErrorSCPI *);
static const char *FuncKeyPress(const char *, ErrorSCPI *);
static const char *FuncKeyLong(const char *, ErrorSCPI *);


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


static const StructSCPI keys[] =
{
    {StructSCPI::Leaf, ":PRESS:", FuncKeyPress},
    {StructSCPI::Leaf, ":LONG:",  FuncKeyLong},
    {StructSCPI::Empty}
};


const StructSCPI head[] =
{
    {StructSCPI::Leaf, "*IDN?", reinterpret_cast<void *>(FuncIDN)},
    {StructSCPI::Node, ":KEY",  const_cast<StructSCPI *>(keys)},
    {StructSCPI::Empty}
};



static const char *FuncIDN(const char *buffer, ErrorSCPI *error)
{
    if(*buffer == '\0')              // Подстраховка от того, что символ окончания команды не принят
    {
        return nullptr;
    }

    if (SCPI::IsLineEnding(buffer))
    {
        SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

        return buffer + 1;
    }

    error->Set(ErrorSCPI::InvalidSequence, buffer, buffer + 1);

    return buffer + 1;
}


static const char *FuncKeyPress(const char *buffer, ErrorSCPI *error)
{
    if (*buffer == '\0')
    {
        return nullptr;
    }

    for (int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if (end)
        {
            if (SCPI::IsLineEnding(end))
            {
                BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
                BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Release));
                return buffer + std::strlen(keyNames[i]) + 1;
            }
            else
            {
                break;
            }
        }
    }

    error->Set(ErrorSCPI::InvalidSequence, buffer, buffer + 1);

    return buffer + 1;
}


static const char *FuncKeyLong(const char *, ErrorSCPI *)
{
    return "";
}
