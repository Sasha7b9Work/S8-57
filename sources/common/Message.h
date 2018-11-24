#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    Message(uint size, uint8);
    Message(uint size, uint8, uint8);
    ~Message();

    uint Size() const { return allocated; }

    uint8 *Data() { return data; }

private:
    /// Положить один байт
    void PutByte(uint8);
    /// Выделить память
    bool Allocate(uint size);
    /// Освободить занятую память
    void Free();
    /// Сколько выделено памяти
    uint allocated;
    /// Сколько заполнено (функциями PutX())
    uint used;
    /// Указатель на выделенный участок памяти
    uint8 *data;
};
