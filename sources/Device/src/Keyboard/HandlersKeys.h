#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


typedef void (*pFuncVChI16)(Chan::E, int16);
typedef void (*pFuncVCh)(Chan::E);
typedef void (*pFuncVI)(int);


struct Handlers
{
    static void Process(KeyEvent);

private:
    /// Обрабатываемое событие
    static KeyEvent event;
    /// Канал, параметры которого нужно временно выводить
    static Chan drawingChan;
    /// Обрабатываемая клавиша
    static Key::E key;
    /// Обрабатываемый тип события
    static TypePress::E type;
    /// Общий обработчик изменения параметра канала - масштаба или смещения
    static void OnChangeParameterChannel(pFuncVChI16, Chan::E, int16);
    /// Пустой обработчик
    static void Empty();
    /// Обработчик нажатия стрелки

    static void HandlerArrow();

    static void EnterRelease();
    static void EnterLong();

    static void Time_Long();
    static void Time_Release();

    static void Start();

    static void Function_Release();

    static void Service_Release();

    static void Measures_Release();

    static void Display_Release();

    static void Memory_Release();

    static void ChannelA_Release();
    static void ChannelB_Release();
    static void ChannelA_Long();
    static void ChannelB_Long();


    static void Trig_Release();
    static void Trig_Long();
    static void TrigLevLess_Press();
    static void TrigLevMore_Press();

    static void RangeLessA();
    static void RangeMoreA();
    static void RangeLessB();
    static void RangeMoreB();

    static void RShiftLessA();
    static void RShiftMoreA();
    static void RShiftLessB();
    static void RShiftMoreB();

    static void TBaseLess();
    static void TBaseMore();
    static void TShiftLess();
    static void TShiftMore();
    /// Общий обработчик раскрытой страницы. Возвращает true, если отработал и дальнейшая обработка события клавиатуры не требуется.
    static bool CommonHandlerPage();
    /// Обработчик нажатия функциональной кнопки
    static void FX_Press();
    /// Обработчик отпускания функциональной кнопки
    static void FX_Release();
    /// Обработчик длительного нажатия функциональной кнопки
    static void FX_Long();
    /// Общий обработчик изменения временных параметров
    static void OnChangeParameterTime(pFuncVI, int);
    /// Открывает страницу или закрывает меню в зависимости от того, какая страница сейчас раскрыта
    static void ShowHidePage(const Page *page);

    static void ChangeRange(Chan::E ch, int16 delta);

    static void ChangeTShift(int delta);

    static void ChangeTBase(int delta);

    static void ChangeRShift(Chan::E ch, int16 delta);
};


