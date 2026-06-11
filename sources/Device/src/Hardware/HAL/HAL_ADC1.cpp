// 2023/11/21 09:58:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


namespace HAL_ADC1
{
    // Для датчика напряжения аккумулятора
    static ADC_HandleTypeDef handle;

    // Массив для хранения результатов: [0] - Battery (CH2), [1] - Charger (CH9)
    static uint32_t results[2];

    static void ReadBothChannels();
}


void HAL_ADC1::Init()
{
    // Настроим входы АЦП для контроля напряжений
    // 36 : PA2 - ADC1 IN2 - контроль АКБ
    // 47 : PB1 - ADC1 IN9 - контроль источника
    // Режим работы:

    __ADC1_CLK_ENABLE(); //-V2571

    HAL_Delay(1);

    HAL_PIO::Init(PIN_ADC1_IN2, HMode::Analog, HPull::No);
    HAL_PIO::Init(PIN_ADC1_IN9, HMode::Analog, HPull::No);

    handle.Instance = ADC1;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION_12B;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = ENABLE;          // ВКЛЮЧАЕМ СКАН РЕЖИМ
    handle.Init.EOCSelection = ADC_EOC_SEQ_CONV; // Ждем завершения ВСЕЙ последовательности
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 2;             // ДВА преобразования в последовательности
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init(&handle);

    // Настройка последовательности
    ADC_ChannelConfTypeDef sConfig = { 0 };

    // Канал 2 (Battery) - Ранк 1
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // Можно увеличить для стабильности
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&handle, &sConfig);

    // Канал 9 (Charger) - Ранк 2
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = 2;
    HAL_ADC_ConfigChannel(&handle, &sConfig);
}


uint HAL_ADC1::ValueBattery()
{
    ReadBothChannels();

    return results[0];
}


uint HAL_ADC1::ValueCharger()
{
    ReadBothChannels();

    return results[1];
}


void HAL_ADC1::ReadBothChannels()
{
    HAL_ADC_Start(&handle);

    // Ожидаем завершения всех преобразований (EOC_SEQ_CONV)
    if (HAL_ADC_PollForConversion(&handle, 100) != HAL_OK) // Таймаут 10 мс
    {
        return;
    }

    // Читаем результаты в правильном порядке
    results[0] = HAL_ADC_GetValue(&handle); // Результат Rank 1 (CH2)
    results[1] = HAL_ADC_GetValue(&handle); // Результат Rank 2 (CH9)

    HAL_ADC_Stop(&handle);
}
