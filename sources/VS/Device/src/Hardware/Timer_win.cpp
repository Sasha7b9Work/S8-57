#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <SDL.h>


static SDL_TimerID id[TypeTimer::Count] = { -1, -1, -1 - 1, -1, -1, -1, -1, -1, -1, -1, -1 };


#define TIMER_PERIODIC 1        // Таймер работает периодически
#define TIMER_ONCE     0        // 


static bool busy = false;

static SDL_Event events[TypeTimer::Count];


static uint CallbackFunc(uint interval, void *parameter)
{
    SDL_PushEvent(reinterpret_cast<SDL_Event *>(parameter));
    return interval;
}


void Timer::SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms)
{
    SDL_UserEvent userEvent;

    userEvent.type = SDL_USEREVENT + static_cast<uint>(type);
    userEvent.data1 = func;
    userEvent.code = TIMER_PERIODIC;

    events[type].type = userEvent.type;
    events[type].user = userEvent;

    Disable(type);

    SDL_Event *parameter = &events[type];

    id[type] = SDL_AddTimer(dTms, CallbackFunc, parameter);
}


void Timer::SetAndStartOnce(TypeTimer::E type, pFuncVV, uint)
{
    Disable(type);

//    userEvt.data1 = func;
//    userEvt.code = TIMER_ONCE;
}


void Timer::Init()
{
}


void Timer::PauseOnTime(uint timeMS)
{
    HAL::Delay(timeMS);
}


uint Timer::TimeMS()
{
    return HAL::TimeMS();
}


void Timer::Disable(TypeTimer::E type)
{
    if (id[type] != -1)
    {
        SDL_RemoveTimer(id[type]);
        id[type] = -1;
    }
}


void Timer::PauseOnTicks(uint numTicks)
{
    volatile uint startTicks = TIME_TICKS;

    while (TIME_TICKS - startTicks < numTicks) {};
}


void Timer::ElapsedCallback()
{

}


bool Timer::IsBusy()
{
    return busy;
}


uint Timer::TimeUS()
{
    return HAL_TIM2::TimeUS();
}


void Timer::StartMultiMeasurement()
{
    busy = true;
    HAL_TIM2::StartMultiMeasurement();
    busy = false;
}


uint Timer::TimeTicks()
{
    return HAL_TIM2::TimeTicks();
}
