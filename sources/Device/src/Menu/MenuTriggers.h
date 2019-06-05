#pragma once
#include "Keyboard/Keyboard.h"


/*
    В этом файле - триггеры меню - это события-реакции на события органов управления
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Триггер - открыватель отладочной консоли
class TriggerDebugMenu
{
public:
    /// Возвращает true, если триггер сработал - консоль открылась
    static bool Triggered(const KeyEvent &event);

private:
    static const int SIZE_BUFFER_FOR_BUTTONS = 10;
    static Key::E bufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
    static const Key::E sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
};
