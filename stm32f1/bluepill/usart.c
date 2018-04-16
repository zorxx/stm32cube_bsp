#include "stm32f1xx_hal_conf.h"
#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_gpio.h"

#define USART_COUNT 3

static USART_HandleTypeDef *uart_list[USART_COUNT] = { NULL }; // TODO: Fix

void HAL_USART_MspInit(USART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  if(huart->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_9;  // Tx 
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_10; // Rx
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    uart_list[0] = huart;

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
  else
  {
     // TODO
  }
}

void HAL_USART_MspDeInit(USART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

    HAL_NVIC_DisableIRQ(USART1_IRQn);

    uart_list[0] = NULL;
  }
  else
  {
    // TODO
  }
}

void USART1_IRQHandler(void)
{
  if(NULL != uart_list[0])
    HAL_USART_IRQHandler(uart_list[0]);
}

void USART2_IRQHandler(void)
{
  if(NULL != uart_list[1])
    HAL_USART_IRQHandler(uart_list[1]);
}

void USART3_IRQHandler(void)
{
  if(NULL != uart_list[2])
    HAL_USART_IRQHandler(uart_list[2]);
}

void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}

void HAL_USART_TxHalfCpltCallback(USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}

void HAL_USART_RxHalfCpltCallback(USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}

void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}

void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}

void HAL_USART_AbortCpltCallback (USART_HandleTypeDef *husart)
{
  UNUSED(husart);
}
