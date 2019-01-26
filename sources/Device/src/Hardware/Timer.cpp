#include "defines.h"
#include "log.h"
#include "Timer.h"
#include <climits>
#include "Hardware/HAL/HAL.h"


using namespace Hardware;

using Timer::Type;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TimerStruct
{
    pFuncVV func;           // Функция таймера
    uint    dTms;           // Период срабатывания, мс
    uint    timeNextMS;     // Время следующего срабатывания. Если == 0xffffffff, то таймер неактивен
    bool    repeat;         // Если true, будет срабатывать, пока не будет вызвана функция Timer_Disable()
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TimerStruct timers[Timer::Type::Number];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Завести таймр, который остановится в timeStop мс
static void StartTIM(uint timeStop);

static void StopTIM();
/// Возвращает время срабатывания ближайщего таймера, либо 0, если таймеров нет
static uint NearestTime();
/// Настроить систему на таймер
static void TuneTIM(::Timer::Type::E type);


namespace Timer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TIM2_
    {
    public:
        static void Init(uint prescaler, uint period)
        {
            HAL::TIM2_::Init(prescaler, period);
        }
        static void DeInit()
        {
            HAL::TIM2_::DeInit();
        }
        static void Start()
        {
            HAL::TIM2_::Start();
        }
        static void Stop()
        {
            HAL::TIM2_::Stop();
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TIM3_
    {
    public:
        static void Init(uint prescaler, uint period)
        {
            HAL::TIM3_::Init(prescaler, period);
        }

        static void DeInit()
        {
            HAL::TIM3_::DeInit();
        }
        
        //static void Start();
        //
        //static void Stop();
        //
        static void EnableIRQ(uint mainPriority, uint subPriority)
        {
            HAL::TIM3_::EnableIRQ(mainPriority, subPriority);
        }
        
        static void DisableIRQ()
        {
            HAL::TIM3_::DisableIRQ();
        }
        
        static void StartIT(uint period)
        {
            HAL::TIM3_::StartIT(period);
        }
        
        static void StopIT()
        {
            HAL::TIM3_::StopIT();
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ::Timer::IsRun(::Timer::Type::E type)
{
    return TIME_NEXT(type) != UINT_MAX; //-V2523
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Init()
{
    for(uint i = 0; i < Timer::Type::Number; i++)
    {
        timers[i].timeNextMS = UINT_MAX; //-V2523
    }

    TIM3_::Init(54000 - 1, 1);
    TIM3_::EnableIRQ(1, 1);

    TIM2_::Init(0, (uint)-1);
    TIM2_::Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::DeInit()
{
    TIM2_::Stop();
    TIM2_::DeInit();

    TIM3_::DisableIRQ();
    TIM3_::StopIT();
    TIM3_::DeInit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::ElapsedCallback()
{
    uint time = TIME_MS;

    if (NearestTime() > time)
    {
        return;
    }

    StopTIM();

    for (uint type = 0; type < Timer::Type::Number; type++)
    {
        if (TIME_NEXT(type) <= time)            // Если пришло время срабатывания
        {
            TimerStruct *timer = &timers[type];
            if(timer->func)
            {
                timer->func();
            }
            if (timer->repeat)
            {
                do      // Цикл нужен потому, что системный таймер SysTick, который отсчитываем миллисекунды, имеет наивысший приоритет,
                {       // и если функция выполняется дольше, чем timer->dTm мс, то оно тут зависнет
                    timer->timeNextMS += timer->dTms;
                } while (timer->timeNextMS < TIME_MS);

            }
            else
            {
                timer->timeNextMS = UINT_MAX; //-V2523
            }
        }
    }

    StartTIM(NearestTime());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Set(Timer::Type::E type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndStartOnce(Timer::Type::E type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndEnable(Timer::Type::E type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartOnce(Timer::Type::E type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Enable(Timer::Type::E type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TuneTIM(Timer::Type::E type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = TIME_MS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // Если таймер должен сработать раньше текущего
    {
        StartTIM(timeNext);         // то заводим таймер на наше время
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Disable(Timer::Type::E type)
{
    timers[type].timeNextMS = UINT_MAX; //-V2523
    timers[type].repeat = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint NearestTime()
{
    uint time = UINT_MAX; //-V2523

    for(uint type = 0; type < Timer::Type::Number; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void StartTIM(uint timeStopMS)
{
    StopTIM();

    if(timeStopMS == UINT_MAX) //-V2523
    {
        return;
    }

    uint dT = timeStopMS - TIME_MS;

    Timer::TIM3_::StartIT((dT * 2) - 1);             // 10 соответствует 0.1мс. Т.е. если нам нужна 1мс, нужно засылать (100 - 1)
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void StopTIM()
{
    Timer::TIM3_::StopIT();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTime(uint timeMS)
{
    HAL::Delay(timeMS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTicks(uint numTicks)
{
    volatile uint startTicks = TIME_TICKS;

    while (TIME_TICKS - startTicks < numTicks)    {};
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartLogging()
{
    timeStartLogging = TIME_TICKS;
    timePrevPoint = timeStartLogging;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointUS(char * name)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    LOG_WRITE("%s %.2f us", name, interval / 120.0);
    return interval;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointMS(char * name)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::TimeUS()
{
    return HAL::TIM2_::TimeUS();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::TimeTicks()
{
    return HAL::TIM2_::TimeTicks();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::TimeMS()
{
    return HAL::TimeMS();
}
