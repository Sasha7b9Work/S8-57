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
    static void SetDisplayedRecord(Record *record, bool forListening);

    struct Cursor
    {
        enum E
        {
            None,
            _1,
            _2
        };
    };


    // —корость перемещени€ окна по пам€ти в режиме просмотра
    struct SpeedWindow
    {
        enum E
        {
            Cell,           // 1 клетка
            _1Window,       // 1 окно
            _10Windows      // 10 окон
        };
    };

    static SpeedWindow::E speed;
};
