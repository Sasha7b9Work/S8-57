#pragma once


class DisplayRecorder
{
public:

    static void Update();
    /// Сместить окно просмотра влево
    static void MoveLeft();
    /// Сместить окно просмотра вправо
    static void MoveRight();
    /// Переместить текущий курсор влево
    static void MoveCursorLeft();
    /// Переместить текущий курсор вправо
    static void MoveCursorRight();

private:
    /// Изобразить установленные настройки
    static void DrawSettings(int x, int y);
    /// Отобразить данные
    static void DrawData();

    static void DrawMemoryWindow();
};
