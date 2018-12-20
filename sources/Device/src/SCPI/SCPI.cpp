#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "SCPI.h"
#include "Hardware/VCP.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::AddNewData(uint8 *buffer, uint length)
{
    SU::ToUpper(buffer, length);
    
    Buffer data(length + 1);
    std::memcpy(data.Data(), buffer, length);
    data.Data()[length] = 0;
    
    if (SU::EqualsStrings(buffer, "*IDN?", 5))
    {
        const char *answer = "S8-56, MNIPI";
        VCP::SendDataAsynch((uint8 *)answer, std::strlen(answer) + 1);
    }
}
