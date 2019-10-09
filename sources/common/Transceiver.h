#pragma once

/*
    Функциональность взаимодействия между процессорами
*/



struct Transceiver
{
    /// Параметром передаётся указатель на функцию, которая будет вызываться при инициализации пинов на приём или передачу, чтобы внешняя программа знала, что нужно перенастроить выводы,
    /// если мы хотим использовать их в другом режиме.
    static void Init();

    /// Функция циклического опроса. Принимает данные, если таковые имеются.
    static bool Receive();

    static void Send(const uint8 *data, uint size);
    static void Send(uint8 data);
    static void Send(uint8 byte0, uint8 byte1);

    static bool inInteraction;
};

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
