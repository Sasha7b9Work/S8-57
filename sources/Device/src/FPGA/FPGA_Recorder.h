#pragma once


struct RecorderEntityFPGA;

namespace FPGA
{
    class Recorder
    {
        friend struct ::RecorderEntityFPGA;
        static void Init();
    };
}
