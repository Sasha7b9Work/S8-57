#include "defines.h"
#include "HeadSCPI.h"


static char *FuncIDN(const char *buffer, ErrorSCPI *);


const StructSCPI head[] =
{
    {StructSCPI::Leaf, "*IDN?", reinterpret_cast<void *>(FuncIDN)},
    {StructSCPI::None}
};



static char *FuncIDN(const char *buffer, ErrorSCPI *)
{
    return const_cast<char *>(buffer);
}
