#pragma once
#include "Command.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef bool(*pFuncBU8)(uint8);

class Decoder
{
public:

    static void AddData(uint8 data);

    static void Update();
private:

    static void RunStep(uint8 data);

    /// Выполняемая функция
    static pFuncBU8 curFunc;
    /// Текущий байт выполняемой функции
    static int step;

    static bool EmptyFunc(uint8) { return true; }

    static bool ButtonPress(uint8);
    /// Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();
};
