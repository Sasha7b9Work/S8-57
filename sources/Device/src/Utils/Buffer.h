#pragma once
#include "defines.h"


/*
     ласс единственно предназначен дл€ автоматического выделени€/освобождени€ пам€ти из кучи
*/

class Buffer
{
public:
    Buffer(uint size);
    ~Buffer();
    inline uint8 *Data() { return buffer; };
    uint   Size() const;
private:
    uint8 *buffer;
    uint size;
};
