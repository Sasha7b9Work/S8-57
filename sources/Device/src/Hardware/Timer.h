#pragma once


#define START_MULTI_MEASUREMENT() Timer::StartMultiMeasurement()
#define PAUSE_ON_TICKS(ticks)     Timer::PauseOnTicks(ticks)
#define PAUSE_ON_MS(ms)           Timer::PauseOnTime(ms)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Timer
 *  @brief Разные таймеры
 *  @{
 */

 /// Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32)
 /// В одной секунде 120.000.000 тиков для С8-53 и 90.000.000 тиков для С8-54.
 /// Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
#define TIME_TICKS (TIM2->CNT)
#define TIME_US    (TIM2->CNT / 90)
#define TIME_MS    HAL_GetTick()


class Timer
{
public:

    struct Type
    {
        enum E
        {
            ShowLevelTrigLev,          ///< Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
            FlashDisplay,              ///< Таймер для мерцающих участков экрана чёрно-белого дисплея.
            ShowMessages,              ///< Таймер для засекания время показа информационных и предупреждающих сообщений.
            MenuAutoHide,              ///< Таймер для отсчёта времени скрывания меню.
            RShiftMarkersAutoHide,     ///< Таймер для отсчёта времени скрывания дополнительных боковых меток.
            StopSound,                 ///< Выключить звук
            StrNaviAutoHide,           ///< Прятать строку навигации меню
            TimerDisplay,              ///< Таймер на ручную отрисовку экрана
            Temp,
            Number
        };
    };


    static void Init();

    static void DeInit();
    /// Назначает таймеру timer функцию и время срабатывания
    static void Set(Timer::Type::E type, pFuncVV func, uint dTms);

    static void SetAndStartOnce(Timer::Type::E type, pFuncVV func, uint dTms);

    static void SetAndEnable(Timer::Type::E type, pFuncVV func, uint dTms);

    static void StartOnce(Timer::Type::E type);

    static void Enable(Timer::Type::E type);

    static void Disable(Timer::Type::E type);

    static bool IsRun(Timer::Type::E type);

    static void PauseOnTime(uint timeMS);

    static void PauseOnTicks(uint numTicks);
    /// Запускает счётчик для измерения малых отрезков времени
    static void StartMultiMeasurement();
    /// Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки
    static void StartLogging();

    static uint LogPointUS(char *name);

    static uint LogPointMS(char *name);
    /// Служебная функция. Вызывается строго из прерывания
    static void ElapsedCallback();
};


/** @}  @}
 */
