#include "defines.h"
#include "Keyboard/BufferButtons.h"
#include "SCPI/KeySCPI.h"


// :KEY:PRESS:
static const char *FuncKeyPress(const char *);
// :KEY:LONG:
static const char *FuncKeyLong(const char *);


static bool TestKeyPress();
static bool TestKeyLong();


static const char *const keyNames[Key::Count] =
{
    " NONE",
    " FUNCTION",
    " MEASURE",
    " MEMORY",
    " SERVICE",
    " A",
    " B",
    " TIME",
    " START",
    " TRIG",
    " DISPLAY",
    " RANGEA+",
    " RANGEA-",
    " RSHIFTA+",
    " RSHIFTA-",
    " RANGEB+",
    " RANGEB-",
    " RSHIFTB+",
    " RSHIFTB-",
    " TBASE+",
    " TBASE-",
    " TSHIFT+",
    " TSHIFT-",
    " TRIGLEV+",
    " TRIGLEV-",
    " LEFT",
    " RIGHT",
    " UP",
    " DOWN",
    " ENTER",
    " F1",
    " F2",
    " F3",
    " F4",
    " F5"
};


const StructSCPI SCPI::key[] =
{
    SCPI_LEAF(":PRESS", FuncKeyPress, TestKeyPress),
    SCPI_LEAF(":LONG",  FuncKeyLong, TestKeyLong),
    SCPI_EMPTY()
};


static const char *FuncKeyPress(const char *buffer)
{
    for(int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Release));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static const char *FuncKeyLong(const char *buffer)
{
    for(int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Press));
            BufferButtons::Push(KeyEvent(static_cast<Key::E>(i), TypePress::Long));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static bool TestKeyPress()
{
    return false;
}


static bool TestKeyLong()
{
    return false;
}
