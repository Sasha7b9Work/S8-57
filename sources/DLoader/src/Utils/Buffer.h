#pragma once


/*
     ласс единственно предназначен дл€ автоматического выделени€/освобождени€ пам€ти из кучи
*/

class Buffer
{
public:
    Buffer(int size = 0U);
    ~Buffer();
    void Realloc(int size);
    void Fill(uint8 value);
    uint8 *data;
    int Size() const { return size; }

    void Log();
    static void LogU8(uint8 *buffer);
private:
    int size;
    void Free();
    void Malloc(int size);
};
