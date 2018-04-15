#include "stm32f1xx_hal_conf.h"
#include "stm32f1xx_hal_cortex.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal.h"
#include "system_stm32f1xx.h"

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

void HAL_MspInit(void)
{
  SystemInit();
  SystemClock_Config();
}

void HAL_MspDeInit(void)
{
}

/* ---------------------------------------------------------------------------------------------
 * I2C
 */

static I2C_HandleTypeDef *i2c_array[3] = { NULL };

#define I2Cx                            I2C1
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_6
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_7
#define I2Cx_SDA_GPIO_PORT              GPIOB

/* Definition for I2Cx's NVIC */
#define I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define I2Cx_ER_IRQn                    I2C1_ER_IRQn
#define I2Cx_EV_IRQHandler              I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHandler              I2C1_ER_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  I2Cx_SCL_GPIO_CLK_ENABLE();
  I2Cx_SDA_GPIO_CLK_ENABLE();
  /* Enable I2Cx clock */
  I2Cx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

  /* I2C RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SDA_PIN;
  HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for I2C ########################################*/
  /* NVIC for I2Cx */
  HAL_NVIC_SetPriority(I2Cx_ER_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(I2Cx_ER_IRQn);
  HAL_NVIC_SetPriority(I2Cx_EV_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(I2Cx_EV_IRQn);
  i2c_array[0] = hi2c; // TODO: Fix
}

/**
  * @brief I2C MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

  /*##-1- Reset peripherals ##################################################*/
  I2Cx_FORCE_RESET();
  I2Cx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

  /*##-3- Disable the NVIC for I2C ##########################################*/
  HAL_NVIC_DisableIRQ(I2Cx_ER_IRQn);
  HAL_NVIC_DisableIRQ(I2Cx_EV_IRQn);
}

void I2Cx_EV_IRQHandler(void)
{
  int idx;
  for(idx = 0; idx < 3; ++idx) // TODO: fix
  {
    if(NULL != i2c_array[idx])
      HAL_I2C_EV_IRQHandler(i2c_array[idx]);
  }
}

void I2Cx_ER_IRQHandler(void)
{
  int idx;
  for(idx = 0; idx < 3; ++idx) // TODO: fix
  {
    if(NULL != i2c_array[idx])
      HAL_I2C_ER_IRQHandler(i2c_array[idx]);
  }
}

/* ---------------------------------------------------------------------------------------------
 * USART 
 */

static UART_HandleTypeDef *uart_list[2] = { NULL }; // TODO: Fix

#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);

  uart_list[0] = huart; // TODO: Fix
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
}

void USARTx_IRQHandler(void)
{
  int idx;

  for(idx = 0; idx < 2; ++idx)  // TODO: fix
  {
    if(NULL != uart_list[idx])
      HAL_UART_IRQHandler(uart_list[idx]);
  }
}

/* ---------------------------------------------------------------------------------------------
 * Interrupt Handlers 
 */

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @biief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/**
  * @brief  This function handles external lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
}

void PPP_IRQHandler(void)
{
}

