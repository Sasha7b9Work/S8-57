#pragma once
#include "Settings/Settings.h"



class Tables
{
public:
    static ENumSignalsInSec::E ENumSignalsInSecToENUM(int numSignalsInSec);
    
    static pString Get(int index);
    // Выводит строку из таблицы symbolsAlphaBet
    static void DrawStr(int index, int x, int y);

    static int Size();

private:
    static pString symbolsAlphaBet[0x48];
};

