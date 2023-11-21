#pragma once
#include "Keyboard/Keyboard.h"


namespace BufferButtons
{
    // Функция вызывается при нажатии, повторе и отпускании кнопки
    void Push(const KeyEvent &str);

    // Извлекает событие из очереди
    KeyEvent Extract();

    // Возвращает true, если очередь событий пуста
    bool IsEmpty();

    inline void Clear() { while (!IsEmpty()) { Extract(); } }

    // Возвращает количество миллисекунд, прошедщих после последнего события клавиатуры
    uint TimeAfterControlMS();
};
