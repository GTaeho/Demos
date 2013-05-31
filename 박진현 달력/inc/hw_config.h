/*
 * (C) COPYRIGHT 2009 CRZ
 *
 * File Name : hw_config.h
 * Author    : POOH
 * Version   : V1.0
 * Date      : 08/12/2009
 */

#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* includes */

#include <stdint.h>
#include <stdio.h>
#include "stm.h"

/* defines */

#define GPIO_LED             GPIOB
#define GPIO_KEY             GPIOA
#define GPIO_USART           GPIOA

#define GPIO_LED1_PIN        GPIO_Pin_9 /* RED */
#define GPIO_LED2_PIN        GPIO_Pin_5 /* YELLOW */ 
#define GPIO_LED3_PIN        GPIO_Pin_8 /* BLUE */

#define GPIO_KEY1_PIN        GPIO_Pin_0 /* LEFT_WKUP */
#define GPIO_KEY2_PIN        GPIO_Pin_1 /* RIGHT_USER */

#define GPIO_USART_Rx_Pin    GPIO_Pin_10
#define GPIO_USART_Tx_Pin    GPIO_Pin_9

#define GPIO_PORTSOURCE_KEY        GPIO_PortSourceGPIOA
#define GPIO_PINSOURCE_KEY1        GPIO_PinSource0
#define GPIO_PINSOURCE_KEY2        GPIO_PinSource1
#define GPIO_EXTI_Line_KEY1        EXTI_Line0
#define GPIO_EXTI_Line_KEY2        EXTI_Line1

/* functions */

void LED_On_Red (void);
void LED_Off_Red (void);
void LED_On_Yellow (void);
void LED_Off_Yellow (void);
void LED_On_Blue (void);
void LED_Off_Blue (void);
void LED_On_All (void);
void LED_Off_All (void);

void LED_Test (void);
void KEY_Test (void);
void Seven_Segment_Test (void);

void delay_1_second(void);
void delay_100_milli_second(void);

void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void USART1_Init(void);
uint8_t USART_GetCharacter(USART_TypeDef *  usart_p);

void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif  /* __HW_CONFIG_H */

