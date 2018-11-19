#pragma once
#include "Keyboard/Keyboard.h"


/*
    В этом файле - триггеры меню - это события-реакции на события органов управления
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Триггер - открыватель отладочной консоли
class TriggerDebugConsole
{
public:
    /// Возвращает true, если триггер сработал - консоль открылась
    static bool Update(Key key);

private:
    static const int SIZE_BUFFER_FOR_BUTTONS = 10;
    static Key bufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
    static const Key sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
};
