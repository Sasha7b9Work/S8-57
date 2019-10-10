//
//  В этом файле содержатся функции эмулятора регистратора
//

#include "defines.h"
#include "FPGA/FPGA.h"
#include "Recorder/Recorder_win.h"


bool RecorderHAL::ReadyPoint()
{
    static bool timeLastRead = 0;   // Время предыдущего чтения точки

    return true;
}


uint8 RecorderHAL::ReadData(Chan::E)
{
    return VALUE::AVE;
}
