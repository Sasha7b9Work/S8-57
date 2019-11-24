#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/DisplaySCPI.h"
#include "Settings/Settings.h"


// :DISPLAY:MAPPING
static const char *FuncMapping(const char *);


static bool TestMapping();


static const char *const mapping[] =
{
    " LINES",
    " DOTS",
    ""
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":MAPPING", FuncMapping, TestMapping),
    SCPI_EMPTY()
};


static const char *FuncMapping(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[DisplayMapping()]));

    SCPI_PROCESS_ARRAY(mapping, DisplayMapping(static_cast<DisplayMapping::E>(i)));
}


bool TestMapping()
{
    return false;
}
