#include "defines.h"
#include "Hardware/VCP.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/Include/PageService.h"
#include "SCPI/HeadSCPI.h"
#include <cstring>


static const char *FuncIDN(const char *);
static const char *FuncReset(const char *);
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


static const StructSCPI keys[] =
{
    DEF_LEAF(":PRESS:", FuncKeyPress),
    DEF_LEAF(":LONG:",  FuncKeyLong),
    DEF_EMPTY()
};


const StructSCPI head[] =
{
    DEF_LEAF("*IDN?", FuncIDN),
    DEF_LEAF("*RST",  FuncReset),
    DEF_NODE(":KEY",  keys),
    DEF_EMPTY()
};



static const char *FuncIDN(const char *buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

    return buffer;

    SCPI_EPILOG()
}


static const char *FuncReset(const char *buffer)
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    return buffer;

    SCPI_EPILOG()
}


static const char *FuncKeyPress(const char *buffer)
{
    for (int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Release));
            return end;
            
            SCPI_EPILOG()
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
            SCPI_PROLOG(end)

            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Long));
            return end;

            SCPI_EPILOG()
        }
    }

    return nullptr;
}
