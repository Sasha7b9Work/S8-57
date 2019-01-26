#pragma once
#include "defines.h"
#include "Data/DataSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)

typedef void(*pFuncTester)();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Hardware
{
    class Clock;
    class Battery;
    class ADC1_;
}

namespace Multimeter
{
    class USART3_;
}

namespace FPGA
{
    class ADC3_;
}

namespace Beeper
{
    class DAC_;
}

namespace Timer
{
    class TIM2_;
    class TIM3_;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HAL
{
    void ErrorHandler(const char *, int);

    void Delay(uint timeMS);

    uint TimeMS();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class RTC_
    {
    friend class Hardware::Clock;
    private:
        static void Init();
        static PackedTime GetPackedTime();
        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ADC1_
    {
    friend class Hardware::Battery;
    friend class Hardware::ADC1_;
    private:
        static void Init();

        static void SetActiveChannel2();

        static void SetActiveChannel9();

        static uint ReadValue();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ADC3_
    {
    friend class FPGA::ADC3_;
    private:
        static void Init();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class USART3_
    {
    friend class Multimeter::USART3_;
    private:
        static void Init(pFuncVV recvCallback);

        static void Transmit(void *buffer, uint size, uint timeout);

        static void StartReceiveIT(void *buffer, uint size);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DAC_
    {
    friend class Beeper::DAC_;
    private:
        static void Init();

        static void StartDMA(void *points, uint numPoints);

        static void StopDMA();

        static void ConfigTIM7(uint16 prescaler, uint16 period);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TIM2_
    {
    public:
        static uint TimeUS();

        static uint TimeTicks();

    friend class Timer::TIM2_;
    private:
        static void Init(uint prescaler, uint period);

        static void DeInit();

        static void Start();

        static void Stop();

        static void StartMultiMeasurement();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TIM3_
    {
    friend class Timer::TIM3_;
    private:
        static void Init(uint prescaler, uint period);

        static void DeInit();

        static void Start();

        static void Stop();

        static void EnableIRQ(uint mainPriority, uint subPriority);

        static void DisableIRQ();

        static void StartIT(uint period);

        static void StopIT();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class FSMC
    {
    public:
        static void Init();

        static void WriteToPanel(const uint8 *data, uint length);         /// \todo Эта функция должна быть приватной
        /// Записать в панель команду, состоящую из двух байт
        static void WriteToPanel2bytes(uint8 byte0, uint8 byte1);

        static void WriteToFPGA8(uint8 *address, uint8 value);

        static void WriteToFPGA16(uint8 *address, uint16 value);

        static uint8 ReadFromFPGA(const uint8 *address);
        /// Возвращает true, если идёт обмен с панелью
        static bool InterchangeWithPanel();
        /// Установить функцию, которую нужно выполнить единоразово после завершения обмена с панелью
        static void RunFunctionAfterInteractionWitchPanel(pFuncTester func);
        /// Читать байт сразу же
        static uint8 ReadByteNow();
    };
}
