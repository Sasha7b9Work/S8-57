#pragma once


namespace DataBus
{
    /// Режим, в котором сейчас находится шина
    struct Mode
    {
        enum E
        {
            DeviceTransmit, ///< Передача в панель
            DeviceReceive,  ///< Чтение из панели
            FPGA            ///< Обмен с FPGA
        };
    };

    extern Mode::E mode;

    void SetModeTransmit();

    void SetModeReceive();

    void SetModeFPGA();
}
