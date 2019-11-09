#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <SDL.h>


static SDL_TimerID id[TypeTimer::Count] = { -1, -1, -1 -1, -1, -1, -1, -1, -1, -1, -1, -1};


#define TIMER_PERIODIC 1        // Таймер работает периодически
#define TIMER_ONCE     0        // 


static bool busy = false;


static SDL_Event evt;
static SDL_UserEvent userEvt =
{
    SDL_USEREVENT,
    0,
    0,
    0,                      // Если 
    nullptr,                // Здесь указатель на функцию обработки
    nullptr
};


static uint CallbackFunc(uint interval, void *)
{
    SDL_Event event = evt;
    SDL_UserEvent userEvent = userEvt;

    event.type = SDL_USEREVENT;
    event.user = userEvent;

    SDL_PushEvent(&event);
    return interval;
}


void Timer::SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms)
{
    userEvt.data1 = func;
    userEvt.code = TIMER_PERIODIC;

    Disable(type);

    id[type] = SDL_AddTimer(dTms, CallbackFunc, nullptr);
}


void Timer::SetAndStartOnce(TypeTimer::E type, pFuncVV func, uint)
{
    Disable(type);

    userEvt.data1 = func;
    userEvt.code = TIMER_ONCE;
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
