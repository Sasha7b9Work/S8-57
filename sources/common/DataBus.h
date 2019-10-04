#pragma once


/// Режим, в котором сейчас находится шина
struct DataBusMode
{
    enum E
    {
        DeviceTransmit, ///< Передача в панель
        DeviceReceive,  ///< Чтение из панели
        FPGA            ///< Обмен с FPGA
    };

    static DataBusMode::E state;
};
