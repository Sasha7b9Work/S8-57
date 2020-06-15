#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef  inputCaptureHandleTIM;
static IWDG_HandleTypeDef handleIWDG;


static uint freqLSI = 0;
volatile static uint32_t uwMeasurementDone = 0;
static uint32_t uwCaptureNumber = 0;
static uint32_t uwPeriodValue = 0;

static uint16_t tmpCC4[2] = { 0, 0 };


static uint GetLSIFrequency();


void HAL_IWDG::Init()
{
    return;
    
    __HAL_RCC_TIM5_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(TIM5_IRQn);
    
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
    {
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }

    freqLSI = GetLSIFrequency();

    handleIWDG.Instance = IWDG;

    //*##-3- Configure the IWDG peripheral
    // Set counter reload value to obtain 250ms IWDG TimeOut.
    // IWDG counter clock Frequency = LsiFreq / 32
    // Counter Reload Value = 4.0 / (32 / freqLSI)
    //                      = freqLSI / (32 * 0.25)
    //                      = freqLSI / 8

    handleIWDG.Init.Prescaler = IWDG_PRESCALER_128;
    handleIWDG.Init.Reload = freqLSI / 32;

    if(HAL_IWDG_Init(&handleIWDG) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


static uint GetLSIFrequency()
{
    uint32_t pclk1 = 0, latency = 0;
    TIM_IC_InitTypeDef timinputconfig = { 0 };
    RCC_OscInitTypeDef oscinit = { 0 };
    RCC_ClkInitTypeDef  clkinit = { 0 };

    // Enable LSI Oscillator
    oscinit.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    oscinit.LSIState = RCC_LSI_ON;
    oscinit.PLL.PLLState = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&oscinit) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    // Configure the TIM peripheral
    // Set TIMx instance
    inputCaptureHandleTIM.Instance = TIM5;

    // TIMx configuration: Input Capture mode
    // The LSI clock is connected to TIM5 CH4.
    // The Rising edge is used as active edge.
    // The TIM5 CCR4 is used to compute the frequency value.
    inputCaptureHandleTIM.Init.Prescaler = 0;
    inputCaptureHandleTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
    inputCaptureHandleTIM.Init.Period = 0xFFFF;
    inputCaptureHandleTIM.Init.ClockDivision = 0;
    inputCaptureHandleTIM.Init.RepetitionCounter = 0;

    if(HAL_TIM_IC_Init(&inputCaptureHandleTIM) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    // Connect internally the  TIM5 CH4 Input Capture to the LSI clock output */
    HAL_TIMEx_RemapConfig(&inputCaptureHandleTIM, TIM_TIM5_LSI);

    /* Configure the Input Capture of channel 4 */
    timinputconfig.ICPolarity = TIM_ICPOLARITY_RISING;
    timinputconfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    timinputconfig.ICPrescaler = TIM_ICPSC_DIV8;
    timinputconfig.ICFilter = 0;

    if(HAL_TIM_IC_ConfigChannel(&inputCaptureHandleTIM, &timinputconfig, TIM_CHANNEL_4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    // Reset the flags
    inputCaptureHandleTIM.Instance->SR = 0;

    // Start the TIM Input Capture measurement in interrupt mode
    if(HAL_TIM_IC_Start_IT(&inputCaptureHandleTIM, TIM_CHANNEL_4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    // Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in stm32f4xx_it.c file)
    while(uwMeasurementDone == 0)
    {
    }
    uwCaptureNumber = 0;

    // Deinitialize the TIM5 peripheral registers to their default reset values
    HAL_TIM_IC_DeInit(&inputCaptureHandleTIM);

    // Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)
    // Get PCLK1 frequency
    pclk1 = HAL_RCC_GetPCLK1Freq();
    HAL_RCC_GetClockConfig(&clkinit, &latency);

    /* Get PCLK1 prescaler */
    if((clkinit.APB1CLKDivider) == RCC_HCLK_DIV1)
    {
        /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
        return ((pclk1 / uwPeriodValue) * 8);
    }
    else
    {
        /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
        return (((2 * pclk1) / uwPeriodValue) * 8);
    }
}


INTERRUPT_BEGIN


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    // Get the Input Capture value
    tmpCC4[uwCaptureNumber++] = (uint16)HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

    if(uwCaptureNumber >= 2)
    {
        /* Compute the period length */
        uwPeriodValue = static_cast<uint16_t>(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
        uwMeasurementDone = 1;
        uwCaptureNumber = 0;
    }
}


void EXTI15_10_IRQHandler(void)
{
    /* As the following address is invalid (not mapped), a Hardfault exception
    will be generated with an infinite loop and when the WWDG counter falls to 63
    the WWDG reset occurs */
    *reinterpret_cast<__IO uint32_t *>(0xA0002000U) = 0xFF; //-V566
}


void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&inputCaptureHandleTIM);
}


INTERRUPT_END
