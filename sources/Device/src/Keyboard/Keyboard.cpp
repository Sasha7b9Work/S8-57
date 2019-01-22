#include "defines.h"
#include "Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Keyboard::ProcessEvent(const KeyEvent & /*event*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Key::Name()
{
    static const pString names[Key::Number] =
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString TypePress::ToString()
{
    static const pString names[TypePress::Number] =
    {
        "Вниз",
        "Повтор",
        "Вверх",
        "Длинное",
        "Пусто"
    };

    return names[value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Key::IsControlSignal() const
{
    static const bool isControl[Key::Number] =
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
