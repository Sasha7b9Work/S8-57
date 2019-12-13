#include "defines.h"
#include <stm32f4xx_hal.h>


void HAL_SPI_MspInit(const SPI_HandleTypeDef* hspi)
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


void HAL_SPI_MspDeInit(const SPI_HandleTypeDef* hspi)
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


// Инициализация VCP
void HAL_PCD_MspInit(PCD_HandleTypeDef *)
{
    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE(); //-V760
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


// Инициалзиация флешки
void HAL_HCD_MspInit(HCD_HandleTypeDef *)
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE(); //-V760
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
