/*
 * (C) COPYRIGHT 2009 CRZ
 *
 * File Name : main.c
 * Author    : POOH
 * Version   : V1.0
 * Date      : 08/12/2009
 */

/* includes */

#include "hw_config.h"

/* global variables */
#define null 0
#define CHECK_CHECKSUM 1


RCC_ClocksTypeDef  rcc_clocks;

char ch;
bool g_TestProcessState = FALSE;

int g_rmcCount = 0; 
int g_count = 0;
char g_date[6]; //날짜용
char g_tempDate[6];
char g_time[9]; //시간용
char g_tempTime[6];
int g_type = 0; //g_type == 0이면 시간, g_type == 1이면 날짜
int i,j,k;

void System_Information()
{
    printf("SYSCLK_Frequency = %d\n",rcc_clocks.SYSCLK_Frequency );
    printf("HCLK_Frequency = %d\n",rcc_clocks.HCLK_Frequency );
    printf("PCLK1_Frequency = %d\n",rcc_clocks.PCLK1_Frequency );
    printf("PCLK2_Frequency = %d\n",rcc_clocks.PCLK2_Frequency );
    printf("ADCCLK_Frequency = %d\n",rcc_clocks.ADCCLK_Frequency );
}

void USB_Test_Start (void)
{
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
}

/*//
 * Name   : main
 * Input  : None
 * Output : None
 * Return : None
 */
int main(void)
{
    

    /* System Clocks Configuration */
    RCC_Configuration();

    RCC_GetClocksFreq(&rcc_clocks);

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* EXTI configuration */
    EXTI_Configuration();

    /* UART initialization */
    USART1_Init();
    USART3_Init();
    
    /* Setup SysTick Timer for 1 msec interrupts  */

 

   
    while(1)
    {        
        while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
        ch = USART_ReceiveData(USART3);                                            
        
        //printf("%c", ch);   //GPS DATA 출력
        if(ch == 82 && g_rmcCount == 0 ) 
        {
          g_rmcCount++;
          continue;
        }
        else if(ch == 77 && g_rmcCount == 1 )
        {
          g_rmcCount++;
          continue;
        }
        else if(ch == 67 && g_rmcCount == 2 )
        {
          g_rmcCount++;
        }       

        if(g_rmcCount == 3 ) // RMC가 비교되고 RMC 라인일때 동작  
        {  
          
         
          if(ch == '\n') // RMC가 아니면 다음 줄로 넘어가지 않고 다시 g_rmcCount로 온다
          {
            g_rmcCount = 0;
            continue;
          }
          
          if( g_type == 0 ) {
           
            if(46 <= ch && ch <= 57) // 아스키 코드 라서 (.) = 46 , 0 = 48 , 9 = 57  
            {
              g_tempTime[g_count] = ch;  
              g_count++;
            }
            else
              g_count = 0;

            if(g_count == 9)  //시간 출력  (hh.mm.ss.ss)
            {
              for( i = 0; i < 9; i++) 
                g_time[i] = g_tempTime[i];  
              
              if( g_time[0] == 49 && g_time[1] == 53 ){ g_time[0] = 48;  g_time[1] = 48;} //15 > 00                // 대한민국은 GMT + 9 시간이라서 +9를 해줌
              else if( g_time[0] == 49 && g_time[1] == 54 ) { g_time[0] = 48;  g_time[1] = 49;} // 16 > 01
              else if( g_time[0] == 49 && g_time[1] == 55 ) { g_time[0] = 48;  g_time[1] = 50;} // 17 > 02
              else if( g_time[0] == 49 && g_time[1] == 56 ) { g_time[0] = 48;  g_time[1] = 51;} // 18 > 03
              else if( g_time[0] == 49 && g_time[1] == 57 ) { g_time[0] = 48;  g_time[1] = 52;} // 19 > 04
              else if( g_time[0] == 50 && g_time[1] == 48 ) { g_time[0] = 48;  g_time[1] = 53;} // 20 > 05
              else if( g_time[0] == 50 && g_time[1] == 49 ) { g_time[0] = 48;  g_time[1] = 54;} // 21 > 06
              else if( g_time[0] == 50 && g_time[1] == 50 ) { g_time[0] = 48;  g_time[1] = 55;} // 22 > 07
              else if( g_time[0] == 50 && g_time[1] == 51 ) { g_time[0] = 48;  g_time[1] = 56;} // 23 > 08          
              else if( g_time[0] == 48 && g_time[1] == 48 ) { g_time[0] = 48; g_time[1] = 57; } // 00 > 09
              else if( g_time[0] == 48 && g_time[1] == 49 ) { g_time[0] = 49; g_time[1] = 48; } // 01 > 10
              else if( g_time[0] == 48 && g_time[1] == 50 ) { g_time[0] = 49; g_time[1] = 49; } // 02 > 11
              else if( g_time[0] == 48 && g_time[1] == 51 ) { g_time[0] = 49; g_time[1] = 50; } // 03 > 12
              else if( g_time[0] == 48 && g_time[1] == 52 ) { g_time[0] = 49; g_time[1] = 51; } // 04 > 13
              else if( g_time[0] == 48 && g_time[1] == 53 ) { g_time[0] = 49; g_time[1] = 52; } // 05 > 14
              else if( g_time[0] == 48 && g_time[1] == 54 ) { g_time[0] = 49; g_time[1] = 53; } // 06 > 15
              else if( g_time[0] == 48 && g_time[1] == 55 ) { g_time[0] = 49; g_time[1] = 54; } // 07 > 16
              else if( g_time[0] == 48 && g_time[1] == 56 ) { g_time[0] = 49; g_time[1] = 55; } // 08 > 17
              else if( g_time[0] == 48 && g_time[1] == 57 ) { g_time[0] = 49; g_time[1] = 56; } // 09 > 18
              else if( g_time[0] == 49 && g_time[1] == 48 ) { g_time[0] = 49; g_time[1] = 57; } // 10 > 19 
              else if( g_time[0] == 49 && g_time[1] == 49 ) { g_time[0] = 50; g_time[1] = 48; } // 11 > 20
              else if( g_time[0] == 49 && g_time[1] == 50 ) { g_time[0] = 50; g_time[1] = 49; } // 12 > 21
              else if( g_time[0] == 49 && g_time[1] == 51 ) { g_time[0] = 50; g_time[1] = 50; } // 13 > 22 
              else if( g_time[0] == 49 && g_time[1] == 52 ) { g_time[0] = 50; g_time[1] = 51; } // 14 > 23
            
              
            printf("GPS TIME\n");
            printf("%c%c:%c%c:%c%c%c%c%c\n",g_time[0],g_time[1], g_time[2], g_time[3], g_time[4], g_time[5],g_time[6],g_time[7],g_time[8]);
            
              g_count = 0;
              g_type = 1;
            }
          }
          else if( g_type == 1 ) // 날짜모드 
          {
            
            if(48 <= ch && ch <= 57) // 아스키 코드 라서 0 = 48 , 9 = 57  
            {
              g_tempDate[g_count] = ch;
              g_count++;
            }
            else
              g_count = 0;
            
            
            if(g_count == 6) //데이터 출력 (DD.MM.YY)
            {
              for(int i = 0; i < 6; i++)
                g_date[i] = g_tempDate[i];
              
              printf("GPS DATE\n"); 
              printf("YYYY,MM,DD \n20%c%c,%c%c,%c%c\n",g_date[4],g_date[5], g_date[2], g_date[3], g_date[0], g_date[1]);
              g_count = 0;
              g_type = 0;
            }
          }
          
          
          
          
        }
        
                                                          
    }
            
}         
        

