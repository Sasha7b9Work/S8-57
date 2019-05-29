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
        } value;
        Mode(E v) : value(v) {};

        bool IsFPGA() const { return value == Mode::FPGA; }

        bool IsDeviceTransmit() const { return value == Mode::DeviceTransmit; }
    };

    void SetModeTransmit();

    void SetModeReceive();

    void SetModeFPGA();

    Mode GetMode();
}
