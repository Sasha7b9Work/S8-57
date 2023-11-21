// 2023/11/21 10:00:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"


struct ModeRedrawFM
{
    enum E
    {
        None,
        Full,
        Files,
        Folders
    };

    static void Set(E);

    static bool Is(E);

private:
    static E modeRedraw;
};


namespace FileManager
{
    // Вызывается один раз при обнаружении новой флешки
    void Init();

    // Нарисовать файловый менеджер
    void Draw();

    void Press_LevelUp();

    void Press_LevelDown();

    bool HandlerKey(const KeyEvent &event);

    bool GetNameForNewFile(char name[255]);

    void Press_Tab();
};
