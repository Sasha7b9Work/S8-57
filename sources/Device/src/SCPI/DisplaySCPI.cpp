#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/DisplaySCPI.h"
#include "Settings/Settings.h"


// :DISPLAY:MAPPING
static const char *FuncDisplayMapping(const char *);


static const char *const mapping[DisplayMapping::Count] =
{
    " LINES",
    " DOTS"
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":MAPPING", FuncDisplayMapping),
    SCPI_EMPTY()
};


static const char *FuncDisplayMapping(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[DisplayMapping()]));

    for (int i = 0; i < DisplayMapping::Count; i++)
    {
        end = SCPI::BeginWith(buffer, mapping[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            DisplayMapping(static_cast<DisplayMapping::E>(i));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}
