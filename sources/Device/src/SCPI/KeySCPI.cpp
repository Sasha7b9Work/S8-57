#include "defines.h"
#include "Keyboard/BufferButtons.h"
#include "SCPI/KeySCPI.h"


// :KEY:PRESS:
static const char *FuncKeyPress(const char *);
static bool TestKeyPress();
static void HintKeyPress(String *);
// :KEY:LONG:
static const char *FuncKeyLong(const char *);
static bool TestKeyLong();
static void HintKeyLong(String *);


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
    SCPI_LEAF(":PRESS", FuncKeyPress, TestKeyPress, "Button press",            HintKeyPress),
    SCPI_LEAF(":LONG",  FuncKeyLong,  TestKeyLong,  "Press and hold a button", HintKeyLong),
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


static void HintKeyPress(String *message)
{
    SCPI::ProcessHint(message, keyNames);
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


static void HintKeyLong(String *message)
{
    SCPI::ProcessHint(message, keyNames);
}


static bool TestKeyPress()
{
    return true;
}


static bool TestKeyLong()
{
    return true;
}
