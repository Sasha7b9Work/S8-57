#pragma once
#include "Display_Types.h"
#include "Display_Warnings.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogEntity
{
    friend class Log;
};

#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    void Init();
    /// Здесь происходит вся отрисовка
    void Update();
    /// Возвращаемое значение true означает, что дисплей находится в состоянии отрисовки
    bool InProcess();
    /// Устанавливает функцию, которая выполится после отрисовки кадра однократно
    void SetFuncAfterUpadteOnce(pFuncVV func);

    void ShowWarning(Warning::E warning);
    /// @brief Установить функцию и режим отрисовки экрана.
    /// @details Возможны три варианта.
    /// 1. DrawMode_Hand - в этом случае будет вызываться функция func(), определяемая пользователем, с частотой 25 раз в секунду.
    /// Используется в случаях, когда не выполняется главный цикл.
    /// 2. DrawMode::Auto и func == 0 - в этом случае будет выполняться функция Update() в главном цикле.
    /// 3. DrawMode::Auto и func != 0 - в этом случае будет выполняться функция func(), определяемая пользователем, но в теле
    /// главного цикла, будучи вызываемой из Update() вместо Update().
    void SetDrawMode(DrawMode::E mode, pFuncVV func);

    void FuncOnWaitStart(const char *text, bool eraseBackground);

    void FuncOnWaitStop();
    /// Устанавливает дополнительную функцию, которая будет отрисовываться каждый раз после обновления экрана
    /// timeRemove - время, по истечении которого дополнительная функция отрисовки будет удалена. Если его не указывать, фукнция удаляться самостоятельно не будет
    void SetAddDrawFunction(pFuncVV func, uint timeRemove = 0);
    /// Удаляет дополнительую функцию отрисовки, установленную вызовом функции SetAddDrawFunction()
    void RemoveAddDrawFunction();

    void SetOrientation(Orientation::E orientation);
    /// Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий
    int TimeMenuAutoHide();
    /// Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    bool IsSeparate();

    void SaveScreenToDrive();

    void SaveRow(int row);
};
