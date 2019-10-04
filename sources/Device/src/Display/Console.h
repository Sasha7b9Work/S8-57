#pragma once


struct Console
{
    static void Draw();

    static void AddString(char *string);

    static int NumberOfLines();
    /// Возвращает true, если консоль должна отображаться на экране
    static bool IsShown();
    /// Эту фнукцию надо вызвыть, когда пользователь изменяет максимальное количество строк в консоли
    static void OnChanged_MaxStringsInConsole();
};

