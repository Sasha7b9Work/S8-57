#pragma once


#define ADDR_ALTERA             ((uint8*)NOR_MEMORY_ADRESS1)


namespace FPGA
{
    namespace VALUE
    {
        const uint8 AVE = 127;
        const uint8 MIN = AVE - 125;
        const uint8 MAX = AVE + 125;
        /// Если значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала
        const uint8 NONE = 0;
    }

    namespace ADDR
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
    }
}

