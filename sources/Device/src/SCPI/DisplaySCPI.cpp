#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/DisplaySCPI.h"
#include "Settings/Settings.h"


// :DISPLAY:MAPPING
static const char *FuncDisplayMapping(const char *);


static const char *const mapping[ModeDrawSignal::Count] =
{
    " LINES",
    " DOTS"
};


const StructSCPI SCPI::display[] =
{
    DEF_LEAF(":MAPPING", FuncDisplayMapping),
    DEF_EMPTY()
};


static const char *FuncDisplayMapping(const char *buffer)
{
    const char *end = SCPI::BeginWith(buffer, "?");
    if (end)
    {
        SCPI_PROLOG(end)

        SCPI::SendAnswer(mapping[set.disp.modeDrawSignal]);

        SCPI_EPILOG(end)
    }

    for (int i = 0; i < ModeDrawSignal::Count; i++)
    {
        end = SCPI::BeginWith(buffer, mapping[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            set.disp.modeDrawSignal = static_cast<ModeDrawSignal::E>(i);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}
