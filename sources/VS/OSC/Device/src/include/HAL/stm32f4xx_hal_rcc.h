#pragma once
#include "stm32f4xx_hal_rcc_ex.h"


#define __HAL_RCC_PWR_CLK_ENABLE()
#define __HAL_RCC_GPIOB_CLK_ENABLE()
#define __HAL_RCC_GPIOC_CLK_ENABLE()

#define RCC_OSCILLATORTYPE_HSE      0x00000001U
#define RCC_HSE_ON                  0
#define RCC_PLL_ON                  ((uint8_t)0x02)
#define RCC_PLLSOURCE_HSE           1
#define RCC_PLLP_DIV2               0x00000002U
#define RCC_CLOCKTYPE_HCLK          0x00000002U
#define RCC_CLOCKTYPE_SYSCLK        0x00000001U
#define RCC_CLOCKTYPE_PCLK1         0x00000004U
#define RCC_CLOCKTYPE_PCLK2         0x00000008U
#define RCC_SYSCLKSOURCE_PLLCLK     0
#define RCC_SYSCLK_DIV1             0
#define RCC_HCLK_DIV4               0
#define RCC_HCLK_DIV2               0


typedef struct
{
    uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                   This parameter can be a value of @ref RCC_Oscillator_Type                   */

    uint32_t HSEState;             /*!< The new state of the HSE.
                                   This parameter can be a value of @ref RCC_HSE_Config                        */

    uint32_t LSEState;             /*!< The new state of the LSE.
                                   This parameter can be a value of @ref RCC_LSE_Config                        */

    uint32_t HSIState;             /*!< The new state of the HSI.
                                   This parameter can be a value of @ref RCC_HSI_Config                        */

    uint32_t HSICalibrationValue;  /*!< The HSI calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT).
                                   This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */

    uint32_t LSIState;             /*!< The new state of the LSI.
                                   This parameter can be a value of @ref RCC_LSI_Config                        */

    RCC_PLLInitTypeDef PLL;        /*!< PLL structure parameters                                                    */
}RCC_OscInitTypeDef;


typedef struct
{
    uint32_t ClockType;             /*!< The clock to be configured.
                                    This parameter can be a value of @ref RCC_System_Clock_Type      */

    uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                    This parameter can be a value of @ref RCC_System_Clock_Source    */

    uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                    This parameter can be a value of @ref RCC_AHB_Clock_Source       */

    uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                    This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

    uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                    This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

}RCC_ClkInitTypeDef;


HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency);
uint32_t HAL_RCC_GetHCLKFreq(void);
