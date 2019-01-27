#include "defines.h"
#include <stm32f4xx_hal.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL_MspInit(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if (hadc->Instance == ADC3)
    {
        __HAL_RCC_ADC3_CLK_ENABLE();
        // ADC3 GPIO Configuration    
        // PF10     ------> ADC3_IN8 
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC3)
    {
        __HAL_RCC_ADC3_CLK_DISABLE();
        // ADC3 GPIO Configuration    
        // PF10     ------> ADC3_IN8 
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hdac->Instance==DAC)
    {
        __HAL_RCC_DAC_CLK_ENABLE();
    
        GPIO_InitStruct.Pin = TESTER_DAC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
    if(hdac->Instance==DAC)
    {
        __HAL_RCC_DAC_CLK_DISABLE();
  
        HAL_GPIO_DeInit(GPIOA, TESTER_DAC_Pin);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if (hspi->Instance == SPI4)
    {
        __HAL_RCC_SPI4_CLK_ENABLE();
        // SPI4 GPIO Configuration    
        // PE11     ------> SPI4_NSS
        // PE12     ------> SPI4_SCK
        // PE13     ------> SPI4_MISO
        // PE14     ------> SPI4_MOSI 
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI4)
    {
        __HAL_RCC_SPI4_CLK_DISABLE();

        // SPI4 GPIO Configuration    
        // PE11     ------> SPI4_NSS
        // PE12     ------> SPI4_SCK
        // PE13     ------> SPI4_MISO
        // PE14     ------> SPI4_MOSI 
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if(huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Инициализация VCP
void HAL_PCD_MspInit(PCD_HandleTypeDef *)
{
    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_11 | GPIO_PIN_12,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF10_OTG_FS
    };

    HAL_GPIO_Init(GPIOA, &isGPIO);

    isGPIO.Pin = GPIO_PIN_9;
    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOA, &isGPIO);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Инициалзиация флешки
void HAL_HCD_MspInit(HCD_HandleTypeDef *)
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_OTG_HS_FS
    };

    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 5, 1);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
