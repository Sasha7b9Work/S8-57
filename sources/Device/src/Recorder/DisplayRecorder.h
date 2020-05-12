#pragma once
#include "Osci/ParametersOsci.h"


struct Record;


class DisplayRecorder
{
public:

    static void Update();

    // Сместить окно просмотра влево
    static void MoveLeft();

    // Сместить окно просмотра вправо
    static void MoveRight();

    // Переместить текущий курсор влево
    static void MoveCursorLeft();

    // Переместить текущий курсор вправо
    static void MoveCursorRight();

    // 
    static bool InProcessUpdate();
};
