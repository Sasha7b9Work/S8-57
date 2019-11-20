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



static const char *FuncIDN(const char *buffer, ErrorSCPI *)
{
    if (SCPI::IsLineEnding(buffer))
    {
        const char *answer = "MNIPI, S8-57, v.1.2";
        VCP::SendDataAsynch(reinterpret_cast<const uint8 *>(answer), std::strlen(answer) + 1);

        return buffer + 1;
    }

    return nullptr;
}
