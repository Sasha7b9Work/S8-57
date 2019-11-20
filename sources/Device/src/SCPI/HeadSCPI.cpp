#include "defines.h"
#include "HeadSCPI.h"


static const char *FuncIDN(const char *buffer, ErrorSCPI *);


const StructSCPI head[] =
{
    {StructSCPI::Leaf, "*IDN?", reinterpret_cast<void *>(FuncIDN)},
    {StructSCPI::Empty}
};



static const char *FuncIDN(const char *buffer, ErrorSCPI *)
{
    return const_cast<char *>(buffer);
}
