// S/W Environment : AVR Studio + WINAVR Compiler
// Target : M128
// Crystal: 16Mhz
//
// Made by New Technology Company(N.T.C) in Korea.
// DAEWOO RYU   
// Email : davidryu@newtc.co.kr
// 82-2-704-4774(TEL), 82-2-704-4733(FAX)
// http://www.NTC.co.kr
// example : Text LCD Example using AM-TLCD (16 * 02 textlcd)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdio.h>

#define CONTROL			PORTF
#define DATA			PORTA

#define TLCD_CTRL_DDR	DDRF
#define TLCD_DATA_DDR	DDRA

#define ON 1
#define OFF 0
#define DELAYTIME 10
#define RIGHT 1
#define LEFT 0


void tx0Char(char message);
void tx1Char(char message);


//int putchar(char c);
//int getchar(void);
void port_init(void);
void uart0_init(void);
void uart1_init(void);
void init_devices(void);
void delay(int n);
static int Putchar(char c, FILE *stream);

static int Putchar(char c, FILE *stream)
{
	tx0Char(c);
    tx1Char(c);
	return 0;
      
}

/*
// printf 함수에서 사용할 putchar 에 UART0와 UART1으로
// 데이터를 보내도록 하였다.
int putchar(char c)
{
    tx0Char(c);
    tx1Char(c);
	return c;
}*/

// UART0 을 이용한 출력
void tx0Char(char message)
{
	while (((UCSR0A>>UDRE0)&0x01) == 0) ;  // UDRE, data register empty        
    UDR0 = message;
}

// UART1 을 이용한 출력
void tx1Char(char message)
{
	while (((UCSR1A>>UDRE1)&0x01) == 0) ;  // UDRE, data register empty        
    UDR1 = message;
}


void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0xFF;
 PORTB = 0x00;
 DDRB  = 0x00;
 PORTC = 0x00; //m103 output only
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0x00;
 PORTE = 0x02;
 DDRE  = 0x00;
 PORTF = 0x00;
 DDRF  = 0xff;
 PORTG = 0x00;
 DDRG  = 0x03;
}


//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x67; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x18;
}

//UART1 initialize
// desired baud rate:9600
// actual baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart1_init(void)
{
 UCSR1B = 0x00; //disable while setting baud rate
 UCSR1A = 0x00;
 UCSR1C = 0x06;
// UBRR1L = 0x2F; //set baud rate lo 7.3728 MHz
// UBRR1L = 0x47; //set baud rate lo 11.0592 Mhz
 UBRR1L = 0x67; //set baud rate lo 16Mhz
 UBRR1H = 0x00; //set baud rate hi
 UCSR1B = 0x18;
}

//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 cli(); //disable all interrupts
 XDIV  = 0x00; //xtal divider
 XMCRA = 0x00; //external memory
 port_init();
 uart0_init();              // UART 0 초기화
 uart1_init();              // UART 1 초기화
 fdevopen(Putchar,0);   

 MCUCR = 0x00;
 EICRA = 0x00; //extended ext ints
 EICRB = 0x00; //extended ext ints
 EIMSK = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 ETIMSK = 0x00; //extended timer interrupt sources
 sei(); //re-enable interrupts
 //all peripherals are now initialized
}


// 시간 지연 함수
void delay(int n)
{
	volatile int i,j;
	for(i=1;i<n;i++)
	{
    	for(j=1;j<500;j++);
	}
}


// RS : PORTA A0
void LCD_RS(int onoff){
    if(onoff == ON)
        CONTROL = CONTROL | 0x01;
    else
        CONTROL = CONTROL & 0xFE;
}
// RW : PORTA A1
void LCD_RW(int onoff){
    if(onoff == ON)
        CONTROL = CONTROL | 0x02;
    else
        CONTROL = CONTROL & 0xFD;
}
// E : PORTA A2
void LCD_E(int onoff){
    if(onoff == ON)
        CONTROL = CONTROL | 0x04;
    else
        CONTROL = CONTROL & 0xFB;
}

void LCD_Clr(void){
	 LCD_RS(OFF);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 PORTF = 0x01;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}

void write_char(char s){
	 LCD_RS(ON);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 PORTF = s;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}	



void returnhome(void){
	 LCD_RS(OFF);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 DATA = 0x02;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}

void LCDONOFF(int onoff){
	 LCD_RS(OFF);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 if(onoff == ON){
	 	 DATA = 0x0F;
	 }else{
	 	 DATA = 0x08;
	 }
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}


void lcd_disp(char x, char y){
	 LCD_RS(OFF);
	 delay(DELAYTIME);
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
	 if(y==0){
	 	DATA = x+0x80;
	 }
	 else{
	 	DATA = x+0xC0;
	 }
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}

void write_lcd(int x, int y, char *ch){
	 lcd_disp(x, y);
	 while(*ch){
	 	write_char(*ch++);
	 }
}

void LCD_Shift(char rl){
	 LCD_RS(OFF);
	 delay(DELAYTIME);
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
	 if(rl == RIGHT)
	 	DATA = 0x1C;
	 else
	 	DATA = 0x18;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
} 

void func_set(int nf){
	 LCD_RS(OFF);
	 delay(DELAYTIME);
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
	 if(nf == OFF)
	 	 DATA = 0x30;
	 else
	 	 DATA = 0x3C;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}

void init_lcd(void){
	 TLCD_CTRL_DDR = 0xff;
	 TLCD_DATA_DDR = 0xff;

	 delay(DELAYTIME*15);
	 func_set(OFF);
	 delay(DELAYTIME*4);
	 func_set(OFF);
	 delay(DELAYTIME);
	 func_set(OFF);
	 delay(DELAYTIME);
	 func_set(ON);
	 delay(DELAYTIME);
	 LCDONOFF(OFF);
	 LCDONOFF(ON);
	 LCD_Clr();

	 
}




int main (void)
{	
	int i=0,t=0;
	char str1[40] = {"TEXT_LCD"};
	char str2[40] = {"N.T.C"};
	 
	init_devices();
	 
	 
	init_lcd();
	 
	printf("\n\n\rTesting TLCD V01  N.T.C\n\r");

	PORTG = 0x03;
	LCDONOFF(ON);
    returnhome();
	LCD_Clr();
	write_lcd(0,0,str1);
	write_lcd(0,1,str2);	 
	
	for(i=0; i<4 ; i++){
		delay(DELAYTIME*100);
		LCD_Shift(RIGHT);
	}

	while(1){
		for(i=0; i<8 ; i++){
			if(t%2 == 0){
				delay(DELAYTIME*100);
				LCD_Shift(LEFT);
			}
			else{
				delay(DELAYTIME*100);
				LCD_Shift(RIGHT);
			}
			PORTA = i;
        	PORTG = i;          // LED 출력
        	delay(100);
		}
		t++;
	}
	return 0;
}


