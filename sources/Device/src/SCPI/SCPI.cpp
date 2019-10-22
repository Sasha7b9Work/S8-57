#include "defines.h"
#include "SCPI.h"
#include "Hardware/VCP.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>



void SCPI::AddNewData(uint8 *buffer, uint length)
{
    SU::ToUpper(buffer, length);
    
    Buffer data(length + 1);
    std::memcpy(data.data, buffer, length);
    data.data[length] = 0;
    
    if (SU::EqualsStrings(buffer, "*IDN?", 5))
    {
        const char *answer = "MNIPI, S8-57, v.1.2";
        VCP::SendDataAsynch(reinterpret_cast<const uint8 *>(answer), std::strlen(answer) + 1);
    }
}
