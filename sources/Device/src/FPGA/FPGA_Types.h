#pragma once


namespace FPGA
{
    namespace VALUE
    {
        const uint8 AVE = 127;
        const uint8 MIN = AVE - 125;
        const uint8 MAX = AVE + 125;
        /// Если значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала
        const uint8 NONE = 0;
    }
}

