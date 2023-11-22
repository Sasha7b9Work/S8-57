// 2023/11/22 13:37:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Console
{
    void Init();

    void Draw();

    void AddString(const char *string);

    // Возвращает true, если консоль должна отображаться на экране
    bool IsShown();

    // Эту фнукцию надо вызвыть, когда пользователь изменяет максимальное количество строк в консоли
    void OnChanged_MaxStringsInConsole();
};
