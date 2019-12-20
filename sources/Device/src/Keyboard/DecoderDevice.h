#pragma once
#include "common/Command.h"




typedef bool(*pFuncBU8)(uint8);

class Decoder
{
public:

    static void AddData(uint8 data);

    static void Update();
    /// Даёт декодеру буфер для сохранения строки картинки
    static void SetBufferForScreenRow(uint8 *pixels);
private:
    static int pointer;
    /// Выполняемая функция
    static pFuncBU8 curFunc;

    static void RunStep(uint8 data);
    /// Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();
    /// Добавляет текстовую строку в консоль
    static bool AddToConsole(uint8);
};
