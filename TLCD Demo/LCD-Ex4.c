#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdio.h>

#define CONTROL  		PORTB
#define DATA   			PORTD
#define LED				PORTF

#define TLCD_CTRL_DDR 	DDRB
#define TLCD_DATA_DDR 	DDRD
#define LED_DDR			DDRF

#define ON 1
#define OFF 0
#define DELAYTIME 10
#define RIGHT 1
#define LEFT 0

// FND에 나타날 숫자. 차례대로 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
// 10진수로 변환하면  차례대로 63, 6, 91, 79, 102, 109, 125, 7, 127, 103
char number[10] ={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};

// Seven segment 4자리 숫자 출력 버퍼
unsigned char display_num[4]={0,1,2,3};  

void port_init() {
	CONTROL = 0x00;
	TLCD_CTRL_DDR  = 0xFF;
	DATA = 0x00;
	TLCD_DATA_DDR  = 0xFF;
}

void uart0_init() {
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x67; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi
 	UCSR0B = 0x18;
}
 
// 시간 지연 함수
void delay(int n) {
	volatile int i,j;

	for(i=1;i<n;i++) {
    	for(j=1;j<600;j++);
	}
}

// RS : PORTA A0
void LCD_RS(int onoff) {
    if(onoff == ON)
        CONTROL |= 0x01;
    else
        CONTROL &= 0xFE;
}

// RW : PORTA A1
void LCD_RW(int onoff) {
    if(onoff == ON)
        CONTROL |= 0x02;
    else
        CONTROL &= 0xFD;
}

// E : PORTA A2
void LCD_E(int onoff) {
    if(onoff == ON)
        CONTROL |= 0x04;
    else
        CONTROL &= 0xFB;
}

void LCD_Clr(void) {
	LCD_RS(OFF);
	delay(DELAYTIME);   
	LCD_RW(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
	CONTROL = 0x01;
	delay(DELAYTIME);
	LCD_E(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
}

void write_char(char s) {
	LCD_RS(ON);
	delay(DELAYTIME);   
	LCD_RW(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
 	delay(DELAYTIME);
	DATA = s;
	delay(DELAYTIME);
	LCD_E(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
} 
 
void LCDONOFF(int onoff) {
	LCD_RS(OFF);
	delay(DELAYTIME);   
	LCD_RW(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
	if(onoff == ON) {
		DATA = 0x0F;
	} else {
		DATA = 0x08;
	}
	delay(DELAYTIME);
	LCD_E(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
}

void lcd_disp(char x, char y) {
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

void write_lcd(int x, int y, char *ch) {
	lcd_disp(x, y);
	while(*ch) {
		write_char(*ch++);
	}
}
 
void func_set(int nf) {
	LCD_RS(OFF);
	delay(DELAYTIME);
	LCD_RW(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
	if(nf == OFF) DATA = 0x30;
	else DATA = 0x3C;
	delay(DELAYTIME);
	LCD_E(OFF);
	delay(DELAYTIME);
	LCD_E(ON);
	delay(DELAYTIME);
}

void init_lcd() {
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
	LCD_Clr();// 클리어를 시키지 않으면 덧씌우는듯한 모습이 나온다.
}

int main() { 
	int i;
	char str1[40] = "Choi Hun sucks";
	char str2[40] = "and Park either";

	LED = 0x00;
	LED_DDR = 0xFF;
	PORTG = 0x00;
	DDRG = 0x03;

	//FND 관련 DDR 설정
	DDRA = 0xFF;
	DDRC = 0x0F;

	init_lcd();			// 역시 출력의 역할
	write_lcd(0,0,str1);	// 첫번째 줄 출력역할
	write_lcd(0,1,str2);	// 두전째 줄 출력역할 

	/* PORTG = 0x01 이면 보드상 D101(PG0) LED 점등
	   PORTG = 0x02 이면 보드상 D102(PG1) LED 점등 
	   PORTG = 0x03 이면 보드상 D101, D102 둘 다 점등 */
	while(1) {

		PORTC = ~0x01;	//FND의 가장 왼쪽(PORTA와 PORTF가 FND아래에 있을때)
		PORTA = number[display_num[0]];	
		//PORTC = ~(0x01 << digit_num);
		delay(3);

		PORTC = ~0x02;
		PORTA = number[display_num[1]];
		delay(3);

		PORTC = ~0x04;
		PORTA = number[display_num[2]];
		delay(3);

		PORTC = ~0x08;	//FND의 가장 오른쪽(PORTA와 PORTF가 FND아래에 있을때)
		PORTA = number[display_num[3]];
		delay(3);		

		if(PING == 0x04) {
			for(i=0;i<8;i++) {
				PORTG = 0x01;
				LED = 1<<i;
				PORTC = ~0x08;
				PORTA = number[i];
				delay(1000);
			}
		} else if(PING == 0x08) {
			for(i=0;i<8;i++) {
				PORTG = 0x02;
				LED |= 1<<i;
				delay(1000);
			}
		}
	}
	return 0;
}
