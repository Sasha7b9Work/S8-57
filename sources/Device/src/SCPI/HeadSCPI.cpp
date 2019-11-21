#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "SCPI/DisplaySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/TimeSCPI.h"


// *IDN?
static const char *FuncIDN(const char *);
// *RST
static const char *FuncReset(const char *);


const StructSCPI SCPI::head[] =
{
    DEF_LEAF("*IDN?",    FuncIDN),
    DEF_LEAF("*RST",     FuncReset),
    DEF_NODE(":KEY",     SCPI::key),
    DEF_NODE(":TIME",    SCPI::time),
    DEF_NODE(":DISPLAY", SCPI::display),
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
