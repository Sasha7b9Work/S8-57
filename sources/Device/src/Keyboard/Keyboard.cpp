#include "defines.h"
#include "Keyboard.h"


/// Указатель на массив кнопок, которые разрешены для обработки. Если == 0, то разрешены все кнопки
static const Key::E *allowedKeys = 0;


pString Key::Name()
{
    static const pString names[Key::Count] =
    {
        "None",
        "Функция",
        "Измер",
        "Память",
        "Сервис",
        "Канал 1",
        "Канал 2",
        "Развертка",
        "Пуск/стоп",
        "Синхр",
        "Дисплей",
        "Range 1 больше",
        "Range 1 меньше",
        "RShift 1 больше",
        "RShift 1 меньше",
        "Range 2 больше",
        "Range 2 меньше",
        "RShift 2 больше",
        "RShift 2 меньше",
        "TBase больше",
        "TBase меньше",
        "TShift больше",
        "TShift меньше",
        "Синхр больше",
        "Синхр меньше",
        "Влево",
        "Вправо",
        "Вверх",
        "Вниз",
        "Ввод",
        "1",
        "2",
        "3",
        "4",
        "5"
    };

    return names[value];
}


pString TypePress::ToString()
{
    static const pString names[TypePress::Count] =
    {
        "Вниз",
        "Повтор",
        "Вверх",
        "Длинное",
        "Пусто"
    };

    return names[value];
}


bool Key::IsControlSignal() const
{
    static const bool isControl[Key::Count] =
    {
        false, // None
        false, // Function
        false, // Measure
        false, // Memory
        false, // Service
        false, // ChannelA
        false, // ChannelB
        false, // Time
        false, // Start
        false, // Trig
        false, // Display
        true,  // RangeMoreA
        true,  // RangeLessA
        true,  // RShiftMoreA
        true,  // RShiftLessA
        true,  // RangeMoreB
        true,  // RangeLessB
        true,  // RShiftMoreB
        true,  // RShiftLessB
        false, // TBaseMore
        false, // TBaseLess
        false, // TShiftMore
        false, // TShiftLess
        false, // TrigLevMore
        false, // TrigLevLess
        false, // Left
        false, // Right
        false, // Up
        false, // Down
        false, // Enter
        false, // F1
        false, // F2
        false, // F3
        false, // F4
        false  // F5
    };


    return  isControl[value];
}


bool KeyEvent::IsArrow() const
{
    return (key == Key::Up) || (key == Key::Down) || (key == Key::Left) || (key == Key::Right);
}



void Keyboard::Lock(const Key::E *const keys)
{
    allowedKeys = keys;
}


void Keyboard::Unlock()
{
    allowedKeys = 0;
}


bool Keyboard::KeyIsActive(Key::E key)
{
    if (allowedKeys == 0)
    {
        return true;
    }

    const Key::E *nextKey = allowedKeys;

    while (nextKey)
    {
        if (*nextKey == 0)
        {
            break;
        }

        if (*nextKey == key)
        {
            return true;
        }
        nextKey++;
    }

    return false;
}


void Keyboard::Wait(Key::E key)
{
    while (1) {};
}
