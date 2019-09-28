#pragma once
#include "Utils/String.h"


struct DataSettings;

struct Chan
{
    enum E
    {
        A,
        B,
        Math,
        Size
    } value;
    explicit Chan(E v) : value(v) { };
    bool IsA() { return value == A; };
    bool IsB() { return value == B; }
    int PointsInChannel() const;
    /// Возвращает количество памяти, требуемой для сохранения данных одного канала
    int RequestBytes(DataSettings *ds) const;
    pString Name() const;
};




