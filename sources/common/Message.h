#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    explicit Message(uint size, uint8);
    explicit Message(uint size, uint8, uint8);
    explicit Message(uint size, uint8, uint16, uint8);
    ~Message();

    uint Size() const { return allocated; }

    uint8 *Data() { return data; }

private:
    /// Положить один байт
    void PutByte(uint8);
    /// Положить послова
    void PutHalfWord(uint16);
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
