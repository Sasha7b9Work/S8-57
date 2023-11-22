// 2023/11/22 13:39:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ExtRAM
{
    // Первый байт внешней памяти
    uint8 *Begin();

    // За последним байтом внешней памяти
    uint8 *End();

    // Записывает во внешнюю память по адресу address данные buffer, size кратен 4
    void Write(const uint8 *buffer, int size, uint8 *address);

    // Читает из внешней памяти address в буфер buffer данные, size кратен 4
    void Read(uint8 *buffer, int size, const uint8 *address);

    void Fill(uint8 *begin = 0, uint8 value = 0, int size = 0);

    // Возвращает сколько процентов данных считанных неправильно.
    float Test1();
    float Test2();

    // Возвращает время в секундах, которое занимает запись/чтение sizekB килобайт во внешнюю память. Возвращает -1, если произошла ошибка
    float TestTime(int sizekB);

    // Тест скорости записи/чтение 1кБ внешней RAM
    float TestTime1kB(uint8 *address);
};
