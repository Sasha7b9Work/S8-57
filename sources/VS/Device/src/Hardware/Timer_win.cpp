#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <SDL.h>


static SDL_TimerID id[TypeTimer::Count] = { -1, -1, -1 -1, -1, -1, -1, -1, -1, -1, -1, -1};


#define TIMER_PERIODIC 1        // Таймер работает периодически
#define TIMER_ONCE     0        // 


static bool busy = false;

static SDL_Event events[TypeTimer::Count];
static SDL_UserEvent userEvents[TypeTimer::Count];


static uint CallbackFunc(uint interval, void *pointer)
{
    SDL_TimerID *typeID = reinterpret_cast<SDL_TimerID *>(pointer);

    int index = typeID - &id[0];

    events[index].user = userEvents[index];

    SDL_PushEvent(&events[index]);
    return interval;
}


void Timer::SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms)
{
    userEvents[type].type = SDL_USEREVENT + static_cast<uint>(type);
    userEvents[type].data1 = func;
    userEvents[type].code = TIMER_PERIODIC;

    events[type].type = userEvents[type].type;
    events[type].user = userEvents[type];

    Disable(type);

    SDL_TimerID *pointer = &id[type];

    id[type] = SDL_AddTimer(dTms, CallbackFunc, pointer);
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
