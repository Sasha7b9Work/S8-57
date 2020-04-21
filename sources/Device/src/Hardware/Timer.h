#pragma once

#define START_MULTI_MEASUREMENT() Timer::StartMultiMeasurement()
#define PAUSE_ON_TICKS(ticks)     Timer::PauseOnTicks(ticks)
#define PAUSE_ON_MS(ms)           Timer::PauseOnTime(ms)


// Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32)
 // В одной секунде 120.000.000 тиков для С8-53 и 90.000.000 тиков для С8-54.
 // Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
#undef TIME_TICKS
#undef TIME_MS
#define TIME_TICKS Timer::TimeTicks()
#define TIME_US    Timer::TimeUS()
#define TIME_MS    Timer::TimeMS()


struct TypeTimer
{
    enum E
    {
        ShowLevelTrigLev,       // Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
        ShowMessages,           // Таймер для засекания время показа информационных и предупреждающих сообщений.
        MenuAutoHide,           // Таймер для отсчёта времени скрывания меню.
        RShiftMarkersAutoHide,  // Таймер для отсчёта времени скрывания дополнительных боковых меток.
        StopSound,              // Выключить звук
        StrNaviAutoHide,        // Прятать строку навигации меню
        Display,                // Таймер на ручную отрисовку экрана
        RemoveAddFunction,      // Таймер на удаление дополнительной функции отрисовки
        USB,                    ///<
        Temp,
        Count
    };
};


struct Timer
{
    static void Init();

    static void DeInit();
    // Назначает таймеру timer функцию и время срабатывания
    static void Set(TypeTimer::E type, pFuncVV func, uint dTms);

    static void SetAndStartOnce(TypeTimer::E type, pFuncVV func, uint dTms);

    static void SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms);

    static void StartOnce(TypeTimer::E type);

    static void Enable(TypeTimer::E type);

    static void Disable(TypeTimer::E type);
    // Возвращает true, если таймер в данный момент ведёт отсчёт
    static bool IsRunning(TypeTimer::E type);

    static void PauseOnTime(uint timeMS);

    static void PauseOnTicks(uint numTicks);
    // Запускает счётчик для измерения малых отрезков времени
    static void StartMultiMeasurement();
    // Ожидать определённое число элементарных операция в цилке for
    static void PauseOnOPS(uint ops);

    static bool IsBusy();
    // Возвращает количество микросекунд, прошедших с момента последнего вызова StartMultiMeasurement()
    static uint TimeUS();
    // Возвращает количество тиков, прошедших с момента последнего вызова StartMultiMeasurement();
    static uint TimeTicks();
    // Возвращает количество миллисекунд, прошедших с момента старта программы
    static uint TimeMS();
    // Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки
    static void StartLogging();

    static uint LogPointUS(char *name);

    static uint LogPointMS(char *name);
    // Служебная функция. Вызывается строго из прерывания
    static void ElapsedCallback();
};
