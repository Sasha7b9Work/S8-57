#pragma once

/*
    Функциональность взаимодействия между процессорами
*/



struct Transceiver
{
    /// Параметром передаётся указатель на функцию, которая будет вызываться при инициализации пинов на приём или передачу, чтобы внешняя программа знала, что нужно перенастроить выводы,
    /// если мы хотим использовать их в другом режиме.
    static void Init();

    static bool InInteraction();

    /// Функция циклического опроса. Принимает данные, если таковые имеются, и передаёт их через функцию, установленную в SetCallbackReceive.
    static bool Update();

    struct Transmitter
    {
        static void Send(const uint8 *data, uint size);
        static void Send(uint8 data);
        static void Send(uint8 byte0, uint8 byte1);
    };
};

