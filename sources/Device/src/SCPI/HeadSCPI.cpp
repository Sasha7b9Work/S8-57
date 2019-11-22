#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "SCPI/DisplaySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/TBaseSCPI.h"


// *IDN?
static const char *FuncIDN(const char *);
// *RST
static const char *FuncReset(const char *);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",     FuncIDN),
    SCPI_LEAF("*RST",      FuncReset),
    SCPI_NODE(":KEY",      SCPI::key),
    SCPI_NODE(":TIMEBASE", SCPI::tBase),
    SCPI_NODE(":DISPLAY",  SCPI::display),
    SCPI_EMPTY()
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
