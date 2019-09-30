#pragma once
#include "Command.h"




typedef bool(*pFuncBU8)(uint8);

class Decoder
{
public:

    static void AddData(uint8 data);

    static void Update();
    /// Даёт декодеру буфер для сохранения строки картинки
    static void SetBufferForScreenRow(uint8 *pixels);
};
