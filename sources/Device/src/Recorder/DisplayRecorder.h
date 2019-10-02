#pragma once


struct DisplayRecorder
{
    static void Update();
    /// Сместить окно просмотра влево
    static void MoveLeft();
    /// Сместить окно просмотра вправо
    static void MoveRight();
    /// Переместить текущий курсор влево
    static void MoveCursorLeft();
    /// Переместить текущий курсор вправо
    static void MoveCursorRight();
};
