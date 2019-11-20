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
    if(*buffer == '\0')              // Подстраховка от того, что символ окончания команды не принят
    {
        return nullptr;
    }

    if (SCPI::IsLineEnding(buffer))
    {
        SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

        return buffer + 1;
    }

    error->Set(ErrorSCPI::InvalidSequence, buffer, buffer + 1);

    return buffer + 1;
}
