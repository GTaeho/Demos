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
char start_calendar[8]={2,9,9,9,1,1,0,1};               //GPS���� �����ϴ� �ʱ�޷� ������

int i;                                                                //for,if�� ����� ���� ����
int year_tmp;                                                   //�޷� �迭�� int������ �����ϱ����� ����











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





void increase_year(void)                                        //�⵵ �����ϱ����� �Լ�
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

void year_calculation()         //�Է¹��� ��,��,�� �迭�� �⵵���� int������ �̾Ƴ�������
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
        
        
       /*               //year_tmp�� ����� �������� Ȯ�� ����§��
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
       
       
       
       
	start_calendar[7]++;            //1�Ͼ� ����
        
       
       
      
              if(((year_tmp%4==0)&&(year_tmp%100!=0))||(year_tmp%400==0))         //�����̸�
              {
                if((start_calendar[4]==0)&&(start_calendar[5]==2))                //������ 2������ ������ �ֱ⶧��!!! 2���̸�
                {
                  if(start_calendar[7]==10)                         
                  {
                  start_calendar[6]++;
                  start_calendar[7]=0;
                    }
                  if((start_calendar[6]==3)&&(start_calendar[7]==0))      //29������ ��
                  {
                   start_calendar[5]++;                 //�� ����
                    start_calendar[6]=0;              //���� 10���ڸ��� 0����
                    start_calendar[7]=1;                //���� 1���ڸ��� 1��
                  }
                }
              }
              else                              //���� �ƴϸ�
              {
                if((start_calendar[4]==0)&&(start_calendar[5]==2))                        //������ �ƴϰ� 2���̸�
                {
                  if(start_calendar[7]==10)                     //���� 10�̸�
                  {
                    start_calendar[6]++;                        //���� 10���ڸ� ����
                    start_calendar[7]=0;                        //1�� 1���ڸ��� 0���� �ʱ�ȭ
                  }
                  
                  if((start_calendar[6]==2)&&(start_calendar[7]==9))      //28�� ��
                  {
                    start_calendar[5]++;                //�� ����
                    start_calendar[6]=0;              //���� 10���ڸ�0����
                    start_calendar[7]=1;                //���� 1���ڸ� 1�� �ʱ�ȭ
                  }
                }
              }
              
              
    
       
       
       
       if((start_calendar[5]==1)||(start_calendar[5]==3)||(start_calendar[5]==5)||(start_calendar[5]==7)||(start_calendar[5]==8)||((start_calendar[4]==1)&&(start_calendar[5]==0))||((start_calendar[4]==1)&&(start_calendar[5]==2)))          //31��¥�� �޷�(1,3,5,7,8,10,12���̸�)
        {
          
          if((start_calendar[6]==3)&&(start_calendar[7]==2))  //31������ ��
            {
              start_calendar[5]++;
              start_calendar[6]=0;              //�����ڸ� ����0����
              start_calendar[7]=1;              //���� 1������ �ؿ��� 1��������Ű�⶧���� 0����
           
             
              if(start_calendar[5]==10)         //���� 10���ڸ��� �Ѿ�� 
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
            
          if((start_calendar[4]==1)&&(start_calendar[5]==3))      //12���̸� �⵵����
               {
                    increase_year();                            //�⵵���� �Լ�
                    start_calendar[4]=0;                //�⵵������ 01�� 01�Ϸ� �ʱ�ȭ
                    start_calendar[5]=1;
                     start_calendar[6]=0;
                    start_calendar[7]=1;
                  }
          
          
          
           }
        
        
        
        if((start_calendar[5]==4)||(start_calendar[5]==6)||(start_calendar[5]==9)||((start_calendar[4]==1)&&(start_calendar[5]==1)))         //30��¥�� �޷�(4,6,8,11���̸�)
        {
          if((start_calendar[4]==1)&&(start_calendar[5]==3))      //�⵵����
                  {     
                     increase_year();                           //�⵵ ���� �Լ�
                     start_calendar[4]=0;                //�⵵������ 01�� 01�Ϸ� �ʱ�ȭ
                    start_calendar[5]=1;
                     start_calendar[6]=0;
                    start_calendar[7]=1;
                    
                  }
           
          if(start_calendar[7]==10)             
          {
              start_calendar[6]++;
              start_calendar[7]=0;
          }
            if((start_calendar[6]==3)&&(start_calendar[7]==1))  //30������ ��
            {
              start_calendar[5]++;
              start_calendar[6]=0;              //�����ڸ� ����0����
              start_calendar[7]=1;
              
              if(start_calendar[5]==10)         //���� 10���ڸ��� �Ѿ�� 
              {
                start_calendar[4]++;
                start_calendar[5]=0;                
              }
            }
          }
        
    }
	
}
