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

/* functions */
char start_calendar[8]={2,9,9,9,1,1,0,1};               //GPS에서 수신하는 초기달력 데이터

int i;                                                                //for,if문 사용을 위한 변수
int year_tmp;                                                   //달력 배열을 int형으로 변형하기위한 변수











extern __IO uint32_t StartUpCounter;

RCC_ClocksTypeDef  rcc_clocks;

void System_Information()
{

    printf("StartUpCounter : %d\n", StartUpCounter);

    printf("SYSCLK_Frequency = %d\n",rcc_clocks.SYSCLK_Frequency );
    printf("HCLK_Frequency = %d\n",rcc_clocks.HCLK_Frequency );
    printf("PCLK1_Frequency = %d\n",rcc_clocks.PCLK1_Frequency );
    printf("PCLK2_Frequency = %d\n",rcc_clocks.PCLK2_Frequency );
    printf("ADCCLK_Frequency = %d\n",rcc_clocks.ADCCLK_Frequency );
}

void DELAY_Test(void)
{
    uint32_t i;

    for(i=0; i<50; i++)
    {
        delay_1_second();
    }

    printf("DELAY_Test done !!\n");
}





void increase_year(void)                                        //년도 증가하기위한 함수
{
  start_calendar[3]++;
  if(start_calendar[3]==10)
  {
    start_calendar[2]++;
    start_calendar[3]=0;
    if(start_calendar[2]==10)
    {
      start_calendar[2]=0;
      start_calendar[1]++;
      if(start_calendar[1]==10)
      {
        start_calendar[1]=0;
        start_calendar[0]++;
      }
    }
  }
}

void year_calculation()         //입력받은 년,월,일 배열중 년도만을 int변수로 뽑아내기위해
{
  year_tmp=(start_calendar[0]*1000)+(start_calendar[1]*100)+(start_calendar[2]*10)+(start_calendar[3]*1);
}
/*
 * Name   : main
 * Input  : None
 * Output : None
 * Return : None
 */
int main(void)
{
    uint8_t ch;

    /* System Clocks Configuration */
    RCC_Configuration();

    RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
    RCC->APB2ENR |= RCC_APB2Periph_USART1;

    RCC_GetClocksFreq(&rcc_clocks);

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* EXTI configuration */
    EXTI_Configuration();

    /* UART initialization */
    USART1_Init();

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(rcc_clocks.SYSCLK_Frequency / 1000))
    { 
        /* Capture error */ 
        while (1);
    }
    

    
    
    
    
    
    
    
   while(1)
   {
     
     
     
           
      //delay_1_second();
      delay_100_milli_second();
      year_calculation();
        
        
       /*               //year_tmp가 제대로 나오는지 확인 대충짠것
        printf("%d",(year_tmp/1000));                             
        printf("%d",((year_tmp/100)-20));       
        printf("%d",((year_tmp/10)-200));       
        printf("%d",((year_tmp/1)-2010));
        printf("\n");
         */
    
   
          
          
       for(i=0;i<8;i++)        //UART DISPLAY
        {
        printf("%d",start_calendar[i]);
          if(i==7)
          {
          printf("\n");
          }
        }
       
       
       
       
	start_calendar[7]++;            //1일씩 증가
        
       
       
      
              if(((year_tmp%4==0)&&(year_tmp%100!=0))||(year_tmp%400==0))         //윤년이면
              {
                if((start_calendar[4]==0)&&(start_calendar[5]==2))                //윤년은 2월에만 영향을 주기때문!!! 2월이면
                {
                  if(start_calendar[7]==10)                         
                  {
                  start_calendar[6]++;
                  start_calendar[7]=0;
                    }
                  if((start_calendar[6]==3)&&(start_calendar[7]==0))      //29일인지 비교
                  {
                   start_calendar[5]++;                 //월 증가
                    start_calendar[6]=0;              //일의 10의자리를 0으로
                    start_calendar[7]=1;                //일의 1의자리를 1로
                  }
                }
              }
              else                              //윤년 아니면
              {
                if((start_calendar[4]==0)&&(start_calendar[5]==2))                        //윤년이 아니고 2월이면
                {
                  if(start_calendar[7]==10)                     //일이 10이면
                  {
                    start_calendar[6]++;                        //일의 10의자리 증가
                    start_calendar[7]=0;                        //1의 1의자리는 0으로 초기화
                  }
                  
                  if((start_calendar[6]==2)&&(start_calendar[7]==9))      //28일 비교
                  {
                    start_calendar[5]++;                //월 증가
                    start_calendar[6]=0;              //일의 10의자리0으로
                    start_calendar[7]=1;                //일의 1의자리 1로 초기화
                  }
                }
              }
              
              
    
       
       
       
       if((start_calendar[5]==1)||(start_calendar[5]==3)||(start_calendar[5]==5)||(start_calendar[5]==7)||(start_calendar[5]==8)||((start_calendar[4]==1)&&(start_calendar[5]==0))||((start_calendar[4]==1)&&(start_calendar[5]==2)))          //31일짜리 달력(1,3,5,7,8,10,12월이면)
        {
          
          if((start_calendar[6]==3)&&(start_calendar[7]==2))  //31일인지 비교
            {
              start_calendar[5]++;
              start_calendar[6]=0;              //십의자리 일은0으로
              start_calendar[7]=1;              //일은 1로지만 밑에서 1로증가시키기때문에 0으로
           
             
              if(start_calendar[5]==10)         //월이 10의자리로 넘어가면 
              {
                start_calendar[4]++;             
                start_calendar[5]=0;
              }
             } 
          if(start_calendar[7]==10)             //start_calendar[4]=1,start_calendar[5]=2,start_calendar[6]=3,start_calendar[7]=1               
          {
              start_calendar[6]++;
              start_calendar[7]=0;
          }
            
          if((start_calendar[4]==1)&&(start_calendar[5]==3))      //12월이면 년도증가
               {
                    increase_year();                            //년도증가 함수
                    start_calendar[4]=0;                //년도증가시 01월 01일로 초기화
                    start_calendar[5]=1;
                     start_calendar[6]=0;
                    start_calendar[7]=1;
                  }
          
          
          
           }
        
        
        
        if((start_calendar[5]==4)||(start_calendar[5]==6)||(start_calendar[5]==9)||((start_calendar[4]==1)&&(start_calendar[5]==1)))         //30일짜리 달력(4,6,8,11월이면)
        {
          if((start_calendar[4]==1)&&(start_calendar[5]==3))      //년도증가
                  {     
                     increase_year();                           //년도 증가 함수
                     start_calendar[4]=0;                //년도증가시 01월 01일로 초기화
                    start_calendar[5]=1;
                     start_calendar[6]=0;
                    start_calendar[7]=1;
                    
                  }
           
          if(start_calendar[7]==10)             
          {
              start_calendar[6]++;
              start_calendar[7]=0;
          }
            if((start_calendar[6]==3)&&(start_calendar[7]==1))  //30일인지 비교
            {
              start_calendar[5]++;
              start_calendar[6]=0;              //십의자리 일은0으로
              start_calendar[7]=1;
              
              if(start_calendar[5]==10)         //월이 10의자리로 넘어가면 
              {
                start_calendar[4]++;
                start_calendar[5]=0;                
              }
            }
          }
        
    }
	
}
