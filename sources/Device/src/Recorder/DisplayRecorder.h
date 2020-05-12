#pragma once
#include "Osci/ParametersOsci.h"


struct Record;


class DisplayRecorder
{
public:

    static void Update();

    // —местить окно просмотра влево
    static void MoveWindowLeft();

    // —местить окно просмотра вправо
    static void MoveWindowRight();

    // ѕереместить текущий курсор влево
    static void MoveCursorLeft();

    // ѕереместить текущий курсор вправо
    static void MoveCursorRight();

    // ¬озвращает true, если как раз происходит отображение данных
    static bool InProcessUpdate();

    // ”становить отображаемую запись
    static void SetDisplayerRecord(Record *record);
};
