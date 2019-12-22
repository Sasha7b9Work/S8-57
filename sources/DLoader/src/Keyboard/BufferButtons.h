#pragma once
#include "Keyboard/Keyboard.h"


struct BufferButtons
{
    /// Функция вызывается при нажатии, повторе и отпускании кнопки
    static void Push(KeyEvent str);
    /// Извлекает событие из очереди
    static KeyEvent Extract();
    /// Возвращает true, если очередь событий пуста
    static bool IsEmpty();

    static void Clear() { while (!IsEmpty()) { Extract(); } }
};
