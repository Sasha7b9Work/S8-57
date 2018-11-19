#pragma once
#include "Settings/SettingsChannel.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс отвечает за вывод информации в нижних двух строках экрана
class BottomPart
{
public:
    static void Draw();

private:
    static int WriteChannel(Chan ch, int x, int y);

    static void WriteTBase(int x, int y);

    static void WriteTextVoltage(Chan ch, int x, int y); ///< Написать в нижней строке параметры вертикального тракта.

    static void WriteStringAndNumber(const char *text, int16 x, int16 y, int number);

    static void DrawTime(int x, int y);
};

