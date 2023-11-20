#pragma once
#include "common/Command.h"



typedef bool(*pFuncBU8)(uint8);


namespace DDecoder
{
    void AddData(uint8 data);

    // Возвращает true, если была обработка
    bool Update();

    // Возвращает размер буфера, ожидающего обработки
    int BytesInBuffer();

    // Возвращает указатель на первый элемент необработанных данных
    uint8 *Buffer();
};
