#pragma once
#include "defines.h"
#include "Data/DataSettings.h"
#include "Hardware/Clock.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PRIORITY_SOUND_DMA1_STREAM5 5, 0


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
    class DAC1_;
}

namespace Timer
{
    class TIM2_;
    class TIM3_;
}

namespace Tester
{
    class DAC2_;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HAL
{
    void Init();

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
    class DAC1_
    {
    friend class Beeper::DAC1_;
    private:
        static void Init();

        static void StartDMA(void *points, uint numPoints);

        static void StopDMA();

        static void ConfigTIM7(uint16 prescaler, uint16 period);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DAC2_
    {
    friend class Tester::DAC2_;
    private:
        static void Init();
        static void SetValue(uint value);
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
    class EEPROM_
    {
    public:
        static void EraseSector(uint startAddress);
        static void WriteBytes(uint address, const uint8 *data, int size);
        static void WriteBufferBytes(uint address, void *buffer, int size);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class OTP_
    {
    public:
        /// Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
        static int GetSerialNumber(char buffer[17]);
        static bool SaveSerialNumber(char *servialNumber);

    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class CRC32_
    {
    public:
        static void Init();
        static uint Calculate(uint address, uint size);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class NVIC_
    {
    public:
        static uint irqEXTI1;
        static uint irqEXTI9_5;
        static void EnableIRQ(uint irq);
        static void DisableIRQ(uint irq);
        static void SetPriorityIRQ(uint irq, uint main, uint sub);
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
        static void RunFunctionAfterInteractionWitchPanel(pFuncVV func);
        /// Читать байт сразу же
        static uint8 ReadByteNow();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    namespace ADDRESSES
    {
        namespace FPGA
        {
            namespace WR
            {
                extern uint8 *START;        ///< Старт
                extern uint8 *TBASE;        ///< Развёртка
                extern uint8 *PRED_LO;      ///< Предзапуск
                extern uint8 *PRED_HI;
                extern uint8 *POST_LO;      ///< Послезапуск
                extern uint8 *POST_HI;

                extern uint8 *UPR;          ///< Управление
#define BIT_UPR_RAND            0       ///< Вкл. рандомизатор
#define BIT_UPR_PEAK            1       ///< Пиковый детектор
#define BIT_UPR_CALIBR_AC_DC    2       ///< Калибратор AC/DC
#define BIT_UPR_CALIBR_ZERO     3       ///< Калибратор 0/4В

                extern uint8 *FREQMETER;    ///< Управление частотомером
                extern uint8 *TRIG;         ///< Управление синхронизацией/запуском
#define BIT_TRIG_POLARITY       0       ///< Управление полярностью синхронизации
#define BIT_TRIG_ENABLED        1       ///< Бит включения сихронизации. 0 - вкл, 1 - выкл

                extern uint8 *START_ADDR;               ///< \brief Старт адреса. Нужно записываеть по этому адресу, чтобы начать чтение с 
                                                        ///  произвольного адреса (который мы записываем в предзапуск)
                extern uint8 *RESET_COUNTER_FREQ;       ///< Записывать сюда после изменения настройки частотомера для обнуления счётчика частоты
                extern uint8 *RESET_COUNTER_PERIOD;     ///< Записывать сюда после изменения настройки частотомера для обнуления счётчика периода

                extern uint8 *TRIG_HOLD_ENABLE;         ///< Управление задержкой синхронизации. "0" - выкл, "1" - вкл
                extern uint8 *TRIG_HOLD_VALUE_LOW;      ///< Младший байт задержки синхронизации
                extern uint8 *TRIG_HOLD_VALUE_MID;      ///<    Значение рассчитывается по формуле N = 0 - (задержка / 100нс). Максимальная задержка - 1.5 сек
                extern uint8 *TRIG_HOLD_VALUE_HI;       ///< Старший байт задержки синхронизации
            }

            namespace RD
            {
                extern uint8 *DATA_A;               ///< Данные первого канала
                extern uint8 *DATA_A_PEAK_MAX;      ///< Данные пикового детектора первого канала
                extern uint8 *DATA_A_PEAK_MIN;
                extern uint8 *DATA_B;               ///< Данные второго канала
                extern uint8 *DATA_B_PEAK_MAX;      ///< Данные пиквого детектора второго канала
                extern uint8 *DATA_B_PEAK_MIN;
                extern uint8 *LAST_RECORD_LO;       ///< Адрес последней записи
                extern uint8 *LAST_RECORD_HI;
                extern uint8 *FREQ_BYTE_0;          ///< Измеренное значение частоты
                extern uint8 *FREQ_BYTE_1;
                extern uint8 *FREQ_BYTE_2;
                extern uint8 *FREQ_BYTE_3;
                extern uint8 *PERIOD_BYTE_0;        ///< Измеренное значение периода
                extern uint8 *PERIOD_BYTE_1;
                extern uint8 *PERIOD_BYTE_2;
                extern uint8 *PERIOD_BYTE_3;
                extern uint8 *FLAG_LO;              ///< Младший байт флага
                extern uint8 *FLAG_HI;              ///< Старший байт флага
            }
        };
    }
}

