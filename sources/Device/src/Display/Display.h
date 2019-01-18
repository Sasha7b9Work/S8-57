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

    void Update();

    void RemoveAddDrawFunction();

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

    void SetAddDrawFunction(pFuncVV func, uint time = MAX_UINT);

    void ChangedRShiftMarkers(bool active);

    void SetOrientation(Orientation orientation);
    /// Возвращает адрес первой и последней точки на экране в координатах экрана
    BitSet64 PointsOnDisplay();
    /// \brief Возращает адрес первой и последней точки в координатах экрана
    BitSet64 BytesOnDisplay();
    /// Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий
    int TimeMenuAutoHide();
    /// Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    bool IsSeparate();
};
