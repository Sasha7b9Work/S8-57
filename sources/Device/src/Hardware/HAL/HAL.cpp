#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SystemClockConfig();
static void MX_GPIO_Init();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::Init()
{
    HAL_Init();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;

    SystemClockConfig();

    MX_GPIO_Init();

    CRC32_::Init();

    FSMC::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SystemClockConfig()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 15;
    RCC_OscInitStruct.PLL.PLLN = 144;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config((uint32_t)(HAL_RCC_GetHCLKFreq() / 1000));

    //HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::ErrorHandler(const char *, int)
{
    while (true) {}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint HAL::TimeMS()
{
    return HAL_GetTick();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void MX_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, SEL_DISP_Pin | LED1_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF, TESTER_ON_Pin | TESTER_PNP_Pin | TESTER_U_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, TESTER_I_Pin | A_LF1_Pin | A_LF2_Pin | A_A1S_Pin
        | A_A0S_Pin | A_LFS_Pin | A_temp_Pin | SPI3_CS2_Pin
        | SPI3_CS3_Pin | KTG15_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(KT_GPIO_Port, KT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, A_A1_Pin | A_A2_Pin | A_A3_Pin | A_A4_Pin
        | SPI3_CS1_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : SEL_DISP_Pin LED1_Pin */
    GPIO_InitStruct.Pin = SEL_DISP_Pin | LED1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : TESTER_ON_Pin TESTER_PNP_Pin TESTER_U_Pin */
    GPIO_InitStruct.Pin = TESTER_ON_Pin | TESTER_PNP_Pin | TESTER_U_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : TESTER_I_Pin A_LF1_Pin A_LF2_Pin A_A1S_Pin
    A_A0S_Pin A_LFS_Pin A_temp_Pin SPI3_CS2_Pin
    SPI3_CS3_Pin KTG15_Pin */
    GPIO_InitStruct.Pin = TESTER_I_Pin | A_LF1_Pin | A_LF2_Pin | A_A1S_Pin
        | A_A0S_Pin | A_LFS_Pin | A_temp_Pin | SPI3_CS2_Pin
        | SPI3_CS3_Pin | KTG15_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pin : FLAG0_Pin */
    GPIO_InitStruct.Pin = FLAG0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FLAG0_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : KT_Pin */
    GPIO_InitStruct.Pin = KT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PB11 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : A_A1_Pin A_A2_Pin A_A3_Pin A_A4_Pin
    SPI3_CS1_Pin */
    GPIO_InitStruct.Pin = A_A1_Pin | A_A2_Pin | A_A3_Pin | A_A4_Pin
        | SPI3_CS1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : TESTER_STR_Pin */
    GPIO_InitStruct.Pin = TESTER_STR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TESTER_STR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PC10 PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : FL_DISP_Pin */
    GPIO_InitStruct.Pin = FL_DISP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FL_DISP_GPIO_Port, &GPIO_InitStruct);
}
