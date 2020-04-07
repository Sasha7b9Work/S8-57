#include "defines.h"


typedef const char *(*FuncSCPI)(const char *);


struct HeadSCPI
{
    const char * morphem;

    FuncSCPI func;
};


static const char *FuncIDN(const char *);


const HeadSCPI morphems[] =
{
    {"*IDN?", FuncIDN}
};


static const char *FuncIDN(const char *)
{

}
