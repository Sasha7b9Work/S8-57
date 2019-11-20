#include "defines.h"
#include "SCPI.h"
#include "Hardware/VCP.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>



void SCPI::AddNewData(const char *buffer, uint length)
{
    String string(buffer);

    SU::ToUpper(string.CString(), length);
    
    Buffer data(length + 1);
    std::memcpy(data.data, buffer, length);
    data.data[length - 1] = 0;
    
    if (SU::EqualsStrings(reinterpret_cast<char *>(data.data), const_cast<char *>("*IDN?")))
    {
        const char *answer = "MNIPI, S8-57, v.1.2";
        VCP::SendDataAsynch(reinterpret_cast<const uint8 *>(answer), std::strlen(answer) + 1);
    }
}
