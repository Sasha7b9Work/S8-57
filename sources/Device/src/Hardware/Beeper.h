#pragma once
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Sound
 *  @brief Работа со звуковым динамиком
 *  @{
 */


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    } value;
};


namespace Beeper
{
    void Init();

    /// Звук нажатия на кнопку.
    void ButtonPress();
    /// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    void ButtonRelease();

    void GovernorChangedValue();
    
    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();
    /// Эту функцию надо вызывать перед записью/стиранием ППЗУ. Звук конфликтует с ППЗУ.
    void WaitForCompletion();

    void Beep(TypePress::E type);

    void Test();
};


/** @}  @}
 */
