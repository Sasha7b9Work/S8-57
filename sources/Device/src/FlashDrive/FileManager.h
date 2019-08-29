#pragma once
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileManager
{
public:
    /// Вызывается один раз при обнаружении новой флешки
    static void Init();
    /// Нарисовать файловый менеджер
    static void Draw();
    
    static void Press_LevelUp();
    
    static void Press_LevelDown();
    
    static bool OnArrows(KeyEvent event);
    
    static bool GetNameForNewFile(char name[255]);

    static void Press_Tab();
};
