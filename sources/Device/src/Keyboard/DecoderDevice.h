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
    /// Текущий байт выполняемой функции
    static int step;

    static int pointer;
    /// Выполняемая функция
    static pFuncBU8 curFunc;
    /// Указатель для приёма строки пикселей экрана
    static uint8 *pixels;

    static const int SIZE_BUFFER = 1024;
    static uint8 buffer[SIZE_BUFFER];

    static void RunStep(uint8 data);
    /// Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();

    static bool ButtonPress(uint8);

    static bool FuncScreen(uint8);

    static bool FuncLengthText(uint8);
    /// Добавляет текстовую строку в консоль
    static bool AddToConsole(uint8);
};
