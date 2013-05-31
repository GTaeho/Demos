/*
 * (C) COPYRIGHT 2009 CRZ
 *
 * File Name : hw_config.c
 * Author    : POOH
 * Version   : V1.0
 * Date      : 08/12/2009
 */

/* includes */

#include "hw_config.h"

/* global variables */

static volatile uint32_t TimingDelay;

/* functions */

void delay_1_second(void)
{
    Delay(1000);
}

void delay_100_milli_second(void)
{
    Delay(100);
}

void SerialPutChar(uint8_t c)
{
  USART_SendData(USART1, c);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_PutString(uint8_t *s)
{
  while (*s != '\0')
  {
    SerialPutChar(*s);
    s ++;
  }
}

/*
 * Name   : USART1_Init
 * Input  : None
 * Output : None
 * Return : None
 */
void USART1_Init(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     - BaudRate = 115200 baud  
     - Word Length = 8 Bits
     - One Stop Bit
     - No parity
     - Hardware flow control disabled (RTS and CTS signals)
     - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate   = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl
                                         = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure the USARTx */ 
    USART_Init(USART1, &USART_InitStructure);

    /* Enable the USART1 */
    USART1->CR1 |= CR1_UE_Set;
}

/*
 * Name   : RCC_Configuration
 * Input  : None
 * Output : None
 * Return : None
 */

__IO uint32_t StartUpCounter = 0;

void RCC_Configuration(void)
{
    __IO uint32_t  HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
    /* Enable HSE */    
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;  
    } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

    /* Flash 2 wait state */
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL6);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0){;}

    /* Select PLL as system clock source */
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08){;}
}

#define NVIC_VectTab_RAM             ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH           ((uint32_t)0x08000000)

void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

void NVIC_Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base location at 0x08000000 */ 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the EXTI1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * Name   : GPIO_Configuration
 * Input  : None
 * Output : None
 * Return : None
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // UART configuration ...

    /* Configure USARTx_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_USART_Tx_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIO_USART, &GPIO_InitStructure);

    /* Configure USARTx_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_USART_Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_USART, &GPIO_InitStructure);

    // Key configuration ...

    /* Configure gpio as input : Button Left-WKUP */
    GPIO_InitStructure.GPIO_Pin = GPIO_KEY1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_KEY, &GPIO_InitStructure);

    /* Configure gpio as input : Button Right-USER */
    GPIO_InitStructure.GPIO_Pin = GPIO_KEY2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_KEY, &GPIO_InitStructure);

    // LED configuration ...

    /* Configure gpio as output : LED1, LED2, LED3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_LED1_PIN | GPIO_LED2_PIN | GPIO_LED3_PIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_LED, &GPIO_InitStructure);
}

void EXTI_Configuration(void)
{ 
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Configure gpio as input : Button Left-WKUP */
    /* Connect EXTI Line to gpio pin */
    GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY1);

    /* Configure EXTI Line to generate an interrupt */
    EXTI_InitStructure.EXTI_Line    = GPIO_EXTI_Line_KEY1;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure gpio as input : Button Right-USER */
    /* Connect EXTI Line to gpio pin */
    GPIO_EXTILineConfig(GPIO_PORTSOURCE_KEY, GPIO_PINSOURCE_KEY2);

    /* Configure EXTI Line to generate an interrupt */  
    EXTI_InitStructure.EXTI_Line    = GPIO_EXTI_Line_KEY2;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
    /* Write a character to the USART */  
    if( ch == '\n') {
        USART_SendData(USART1, '\r');
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, '\n');
    }else {
        USART_SendData(USART1, (uint8_t) ch);
    }

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}

uint8_t USART_GetCharacter(USART_TypeDef *  usart_p)
{
    uint8_t data;

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(usart_p, USART_FLAG_RXNE) == RESET);

    /* Write a character to the USART */
    data = USART_ReceiveData(usart_p);

    USART_SendData(usart_p, data);
    while(USART_GetFlagStatus(usart_p, USART_FLAG_TXE) == RESET);

    if( data == '\r' )  return (int)('\n');
    else                return(data);
}

