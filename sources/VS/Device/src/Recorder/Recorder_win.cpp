//
//  В этом файле содержатся функции эмулятора регистратора
//

#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Recorder/Recorder_win.h"
#include "Settings/Settings.h"


bool RecorderHAL::ReadyPoint()
{
    static uint timeLastRead = 0;   // Время предыдущего чтения точки

    bool result = false;

    static const uint delta[RecorderScaleX::Count] =
    {
        100 / 20, // 
        200 / 20, // 
        500 / 20, // 
        1000 / 20, // 
        2000 / 20, // 
        5000 / 20, // 
        10000 / 20  // 
    };

    if (TIME_MS - delta[static_cast<int>(set.rec.scaleX.value)] > 100)
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
