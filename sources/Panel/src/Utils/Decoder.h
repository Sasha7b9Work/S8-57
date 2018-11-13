#pragma once
#include "defines.h"
#include "Command.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Decoder
{
public:
    static void AddData(uint8 data);

private:
    /// Текущий байт выполняемой функции
    static int step;

    static bool EmptyFunc(uint8) { return true; }

    static bool InButtonPress(uint8);

    static bool BeginScene(uint8);

    static bool EndScene(uint8);

    static bool SetColor(uint8);

    static bool FillRegion(uint8);

    static bool DrawText(uint8);

    static bool SetPalette(uint8);

    static bool DrawRectangle(uint8);

    static bool DrawVLine(uint8);

    static bool SetFont(uint8);

    static bool SetPoint(uint8);

    static bool DrawLine(uint8);

    static bool DrawHLine(uint8);

    static bool DrawTesterPoints(uint8);
    /// Эту функцию надо вызывать после выполнения последнего шага
    static void FinishCommand();
};
