// 2023/11/22 13:46:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


struct Record;


namespace DisplayRecorder
{
    void Update();

    // —местить окно просмотра влево
    void MoveWindowLeft();

    // —местить окно просмотра вправо
    void MoveWindowRight();

    // ѕереместить текущий курсор влево
    void MoveCursorLeft();

    // ѕереместить текущий курсор вправо
    void MoveCursorRight();

    // ¬озвращает true, если как раз происходит отображение данных
    bool InProcessUpdate();

    // ”становить отображаемую запись
    void SetDisplayedRecord(Record *record);

    struct Cursor
    {
        enum E
        {
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
        } value;

        SpeedWindow(E v) : value(v)  { }

        int NumPoints() const;      // ¬озвращает число точек, на которое нужно сдвинуть экран
    };

    extern SpeedWindow speed;
};
