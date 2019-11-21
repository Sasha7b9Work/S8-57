#include "defines.h"
#include "Hardware/VCP.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/SettingsOsci.h"
#include "SCPI/HeadSCPI.h"
#include <cstring>


// *IDN?
static const char *FuncIDN(const char *);
// *RST
static const char *FuncReset(const char *);
// :KEY:PRESS:
static const char *FuncKeyPress(const char *);
// :KEY:LONG:
static const char *FuncKeyLong(const char *);
// :TIME:SCALE:
static const char *FuncTBaseScale(const char *);


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

static const char *tBaseNames[TBase::Count] =
{
    "2NS",
    "5NS",
    "10NS",
    "20NS",
    "50NS",
    "100NS",
    "200NS",
    "500NS",
    "1US",
    "2US",
    "5US",
    "10US",
    "20US",
    "50US",
    "100US",
    "200US",
    "500US",
    "1MS",
    "2MS",
    "5MS",
    "10MS",
    "20MS",
    "50MS",
    "100MS",
    "200MS",
    "500MS",
    "1S",
    "2S",
    "5S",
    "10S"
};


static const StructSCPI keys[] =
{
    DEF_LEAF(":PRESS:", FuncKeyPress),
    DEF_LEAF(":LONG:",  FuncKeyLong),
    DEF_EMPTY()
};

static const StructSCPI tBases[] =
{
    DEF_LEAF(":SCALE:", FuncTBaseScale),
    DEF_EMPTY()
};


const StructSCPI head[] =
{
    DEF_LEAF("*IDN?", FuncIDN),
    DEF_LEAF("*RST",  FuncReset),
    DEF_NODE(":KEY",  keys),
    DEF_NODE(":TIME", tBases),
    DEF_EMPTY()
};



static const char *FuncIDN(const char *buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

    SCPI_EPILOG(buffer)
}


static const char *FuncReset(const char *buffer)
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    SCPI_EPILOG(buffer)
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
            
            SCPI_EPILOG(end)
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

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static const char *FuncTBaseScale(const char *buffer)
{
    for(int i = 0; i < TBase::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, tBaseNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            TBase(static_cast<TBase::E>(i));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}
