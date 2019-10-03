#pragma once

namespace Address
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

struct Flag
{
    enum E
    {
        _DATA_READY = 0,  ///< Данные готовы для считывания (окончание счётчика послезапуска)
        _TRIG_READY = 1,  ///< Флаг синхроимпульса
        _PRED = 2,  ///< Если 1, то предзапуск отсчитал, можно давать принудительный запуск (окончание
        _P2P = 3,  ///< 
        _FREQ_READY = 4,  ///< Флаг готовности измерения частоты
        _PERIOD_READY = 5,  ///< Флаг готовности измерения периода
        _HOLD_OFF_FLAG = 7,  ///< Принудительный запуск можно давать только когда этот флаг в единице
        _FREQ_OVERFLOW = 8,  ///< Признак переполнения счётчика частоты
        _PERIOD_OVERFLOW = 9,  ///< Признак переполнения счётчика периода
        _FREQ_IN_PROCESS = 10, ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
        _PERIOD_IN_PROCESS = 11  ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
    } value;
};

struct FPin
{
    enum E
    {
        SPI3_SCK,
        SPI3_DAT,
        SPI3_CS1,
        SPI3_CS2,
        A1,
        A2,
        A3,
        A4,
        LF1,
        LF2,
        A1S,
        A0S,
        LFS,
        Number
    } value;
    explicit FPin(E v) : value(v) {};
};


namespace FPGA
{
    struct GetFlag
    {
        static bool DATA_READY();
        static bool TRIG_READY();
        static bool PRED();
        static bool FREQ_READY();
        static bool HOLD_OFF();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };

    namespace HAL
    {
        void LoadRegUPR();

        void ReadFlag();

        extern uint16 flag;
     
        namespace GPIO
        {
            void Init();

            uint16 GetPin(FPin::E pin);

            void WritePin(FPin::E pin, int enable);

            void SetPin(FPin::E pin);

            void ResetPin(FPin::E pin);

            void WriteRegisters(FPin::E cs, uint16 value);
        }
    }
}
