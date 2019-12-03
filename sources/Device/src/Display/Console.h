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
private:
    static CHAR_BUF2(buffer, 33, 100);
    /// true означает, что идёт процесс вывода консоли и добавлять в неё новые строки нельзя (это происходит, когда добавление идёт из прерывания)
    static bool inProcessDrawConsole;
    /// Количество заполненных строк в консоли
    static int stringInConsole;
    /// Здесь сохраняется предыдущее значение максимального количества строк в консоли
    static int16 prevMaxStrinsInConsole;

    static void DeleteFirstString();
};

