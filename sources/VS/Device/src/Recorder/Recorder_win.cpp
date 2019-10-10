//
//  В этом файле содержатся функции эмулятора регистратора
//

#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Recorder/Recorder_win.h"


bool RecorderHAL::ReadyPoint()
{
    static uint timeLastRead = 0;   // Время предыдущего чтения точки

    bool result = false;

    if (TIME_MS - timeLastRead > 100)
    {
        timeLastRead = TIME_MS;
        result = true;
    }

    return result;
}


uint8 RecorderHAL::ReadData(Chan::E)
{
    return VALUE::AVE;
}
