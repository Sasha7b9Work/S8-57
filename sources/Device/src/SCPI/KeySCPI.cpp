#include "defines.h"
#include "Keyboard/BufferButtons.h"
#include "SCPI/SCPI.h"
#include "Utils/String.h"


// :KEY:PRESS:
static const char *FuncKeyPress(pCHAR);
static bool TestKeyPress();
// :KEY:LONG:
static const char *FuncKeyLong(pCHAR);
static bool TestKeyLong();

static void HintKey(String *);


static const char *const keyNames[Key::Count] =
{
    " NONE",
    " FUNCTION",
    " MEASURE",
    " MEMORY",
    " SERVICE",
    " 1",
    " 2",
    " TIME",
    " START",
    " TRIG",
    " DISPLAY",
    " RANGE1+",
    " RANGE1-",
    " RSHIFT1+",
    " RSHIFT1-",
    " RANGE2+",
    " RANGE2-",
    " RSHIFT2+",
    " RSHIFT2-",
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
    SCPI_LEAF(":PRESS", FuncKeyPress, TestKeyPress, "Button press",            HintKey),
    SCPI_LEAF(":LONG",  FuncKeyLong,  TestKeyLong,  "Press and hold a button", HintKey),
    SCPI_EMPTY()
};


#define PROCESS_KEY(end, name, key)                                                     \
    end = SCPI::BeginWith(buffer, name);                                                \
    if (end)                                                                            \
    {                                                                                   \
        SCPI_PROLOG(end)                                                                \
                                                                                        \
        BufferButtons::Push(KeyEvent(static_cast<Key::E>(key), TypePress::Press));      \
        BufferButtons::Push(KeyEvent(static_cast<Key::E>(key), TypePress::Release));    \
                                                                                        \
        SCPI_EPILOG(end)                                                                \
    }                                                                                   \


static const char *FuncKeyPress(const char *buffer) //-V2506
{
    const char *end = nullptr;

    PROCESS_KEY(end, " TRIGLEV+", Key::TrigLevMore);
    PROCESS_KEY(end, " TRIGLEV-", Key::TrigLevLess);

    for(int i = 0; i < Key::Count; i++)
    {
        PROCESS_KEY(end, keyNames[i], i);
    }

    return nullptr;
}


static void HintKey(String *message)
{
    SCPI::ProcessHint(message, keyNames);
}


static const char *FuncKeyLong(const char *buffer) //-V2506
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
    return true;
}


static bool TestKeyLong()
{
    return true;
}
