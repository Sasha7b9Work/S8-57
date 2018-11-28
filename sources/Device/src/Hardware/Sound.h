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


class Sound
{
public:
    static void Init();

    /// Звук нажатия на кнопку.
    static void ButtonPress();
    /// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    static void ButtonRelease();

    static void GovernorChangedValue();
    
    static void RegulatorShiftRotate();

    static void RegulatorSwitchRotate();

    static void WarnBeepBad();

    static void WarnBeepGood();
    /// Эту функцию надо вызывать перед записью/стиранием ППЗУ. Звук конфликтует с ППЗУ.
    static void WaitForCompletion();

    static void Beep(TypePress::E type);

    static void Test();

    static DAC_HandleTypeDef handleDAC;

private:

    static void Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration);

    static void Stop();

    static void SetWave();

    static void CalculateMeandr();

    static uint16 CalculatePeriodForTIM();

    static void CalculateSine();

    static void CalculateTriangle();

    static void ConfigTIM7(uint16 prescaler, uint16 period);
};


/** @}  @}
 */
