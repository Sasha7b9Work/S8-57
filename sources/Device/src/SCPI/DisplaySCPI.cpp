#include "defines.h"
#include "SCPI/DisplaySCPI.h"


// :DISPLAY:MAPPING
static const char *FuncDisplayMapping(const char *);


const StructSCPI SCPI::display[] =
{
    DEF_LEAF(":MAPPING:", FuncDisplayMapping),
    DEF_EMPTY()
};


const char *FuncDisplayMapping(const char *)
{
    return nullptr;
}
