#pragma once


class ExtRAM
{
public:
    /// Первый байт внешней памяти
    static uint8 *Begin();
    /// За последним байтом внешней памяти
    static uint8 *End();
    /// Записывает во внешнюю память по адресу address данные buffer, size кратен 4
    static void Write(uint8 *buffer, uint size, uint8 *address);
    /// Читает из внешней памяти address в буфер buffer данные, size кратен 4
    static void Read(uint8 *buffer, uint size, uint8 *address);

    /// Возвращает сколько процентов данных считанных неправильно.
    static float Test1();
    static float Test2();
    /// Возвращает время в секундах, которое занимает запись/чтение sizekB килобайт во внешнюю память. Возвращает -1, если произошла ошибка
    static float TestTime(uint sizekB);
    /// Тест скорости записи/чтение 1кБ внешней RAM
    static float TestTime1kB(uint8 *address);
};
