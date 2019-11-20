#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include <cstring>


static const char *FuncIDN(const char *buffer, ErrorSCPI *);


const StructSCPI head[] =
{
    {StructSCPI::Leaf, "*IDN?", reinterpret_cast<void *>(FuncIDN)},
    {StructSCPI::Empty}
};



static const char *FuncIDN(const char *buffer, ErrorSCPI *error)
{
    if (SCPI::IsLineEnding(buffer))
    {
        VCP::SendString("MNIPI, S8-57, v.1.2");

        return buffer + 1;
    }

    return nullptr;
}
