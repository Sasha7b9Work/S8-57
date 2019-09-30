#pragma once
#include "Keyboard/Keyboard.h"



namespace BufferButtons
{
    /// Функция вызывается при нажатии, повторе и отпускании кнопки
    void Push(KeyEvent str);
    /// Извлекает событие из очереди
    KeyEvent Extract();
    /// Возвращает true, если очередь событий пуста
    bool IsEmpty();
};
