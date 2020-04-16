#pragma once
#include "common/Command.h"



typedef bool(*pFuncBU8)(uint8);


class DDecoder
{
public:

    static void AddData(uint8 data);

    // Возвращает true, если была обработка
    static bool Update();

    // Возвращает размер буфера, ожидающего обработки
    static int BytesInBuffer();

    // Возвращает указатель на первый элемент необработанных данных
    static uint8 *Buffer();
};
