#include "defines.h"
#include "HAL.h"
#include "HAL_PIO.h"
#include <stm32f4xx_hal.h>


static UART_HandleTypeDef handler;

static pFuncVV recvHandler = 0;


void HAL_USART3::Init(pFuncVV _recvHandler)
{
    __HAL_RCC_USART3_CLK_ENABLE();

    HAL_PIO::Init(PIN_USART3_RX, HMode::AF_PP, HPull::Up, HSpeed::VeryHigh, HAlternate::AF7_USART3);
    HAL_PIO::Init(PIN_USART3_TX, HMode::AF_PP, HPull::Up, HSpeed::VeryHigh, HAlternate::AF7_USART3);

    recvHandler = _recvHandler;

    handler.Instance = USART3;
    handler.Init.BaudRate = 9600;
    handler.Init.WordLength = UART_WORDLENGTH_8B;
    handler.Init.StopBits = UART_STOPBITS_1;
    handler.Init.Parity = UART_PARITY_NONE;
    handler.Init.Mode = UART_MODE_TX_RX;
    handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handler.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&handler) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}


void HAL_USART3::Transmit(void *buffer, int size, uint timeout)
{
    HAL_UART_Transmit(&handler, static_cast<uint8 *>(buffer), static_cast<uint16>(size), timeout);
}


void HAL_USART3::StartReceiveIT(void *buffer, int size)
{
    HAL_UART_Receive_IT(&handler, static_cast<uint8 *>(buffer), static_cast<uint16>(size));
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *)
{
    recvHandler();
}

INTERRUPT_BEGIN

void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&handler);
}
INTERRUPT_END
