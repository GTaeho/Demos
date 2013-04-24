#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#define	 CNTL_PORT	   PORTF
#define	 DATA_PORT	   PORTA

#define	 ON		   	   1
#define	 OFF	   	   0
#define	 D_TIME	   	   10

#define	 FST_LIN   	   0
#define	 SND_LIN   	   2

#define	 LCD_SET	   0x38
#define	 SCR_CLR 	   0x01
#define	 RTN_HOME	   0x02
#define	 DISP_ON	   0x0F
// Shift Screen to Left
#define	 SCR_L_SHFT	   0x18	  
// Shift Screen to Right
#define	 SCR_R_SHFT	   0x1c

// printf 로 UART0 함수 사용시 기존 함수를 지우고 추가할 것.
/*int putchar(char c)
{
    while (((UCSR0A>>UDRE0)&0x01) == 0) ;  // UDRE, data register empty
        
    UDR0 = c;
    return c;
}

// scanf 함수 사용시 추가할 것.
int getchar(void)
{
    while ((UCSR0A & 0x80) == 0);
    return UDR0;
}*/

void port_init(void) {
	PORTA = 0x00;      // Data Port
	DDRA  = 0xFF;
	PORTB = 0x00;
	DDRB  = 0xFF;		
	PORTC = 0x00;
	DDRC  = 0x00;
	PORTD = 0x00;
	DDRD  = 0x00;
	PORTE = 0x00;
	DDRE  = 0x00;
	PORTF = 0x00;
	DDRF  = 0xFF;		// Control Port
	PORTG = 0x00;
	DDRG  = 0x00;
}

//call this routine to initialize all peripherals
void init_devices(void) {
	//stop errant interrupts until set up
	cli(); //disable all interrupts

	XMCRA = 0x00; //external memory
	port_init();


	MCUCR = 0x00;
	EICRA = 0x00; //extended ext ints
	EICRB = 0x00; //extended ext ints
	EIMSK = 0x00;
	TIMSK = 0x00; //timer interrupt sources
	ETIMSK = 0x00; //extended timer interrupt sources

 	sei(); //re-enable interrupts
	//all peripherals are now initialized
}

void delay(int dtimes) {
    int i,j;
    
    for(i=0 ; i<1000 ; i++){
        for(j=0 ; j<dtimes ; j++) ;
    }
}


// RS : PORTA D0
void LCD_RS(int onoff) {
    if(onoff == ON)
		 CNTL_PORT |= 0x01;
    else CNTL_PORT &= 0xFE;
}

// RW : PORTA D1
void LCD_RW(int onoff) {
    if(onoff == ON)
         CNTL_PORT |= 0x02;
    else CNTL_PORT &= 0xFD;
}

// RS : PORTA D2
void LCD_E(int onoff) {
    if(onoff == ON)
         CNTL_PORT |= 0x04;
    else CNTL_PORT &= 0xFB;
}

void LCD_E_Pulse(void) {
	 LCD_E(ON);			delay(D_TIME);
	 LCD_E(OFF); 		delay(D_TIME);
}

void Write_LCD(unsigned char val) {
 	 LCD_E(OFF);		delay(D_TIME);
 	 LCD_RS(OFF);		delay(D_TIME); 	 
	 LCD_RW(OFF);		delay(D_TIME);
	 LCD_E(ON);			delay(D_TIME);
	 DATA_PORT = val; 	delay(D_TIME); 
	 LCD_E(OFF); 		delay(D_TIME);
//	 LCD_E(ON);			delay(D_TIME); 	 // case in the "SC1602-model"
}

void Write_Char(unsigned char Ch) {
 	 LCD_E(OFF);	 	 delay(D_TIME);
	 LCD_RS(ON);		 delay(D_TIME);
	 LCD_RW(OFF);
	 LCD_E(ON);	 		 delay(D_TIME);
	 DATA_PORT = Ch;	 delay(D_TIME); 
	 LCD_E(OFF); 		 delay(D_TIME);
//	 LCD_E(ON);			 delay(D_TIME);	// case in the "SC1602-model"
}

void Write_Str(char *Str) {
	 while (*Str) {
	 	Write_Char(*Str++);
	 }
}

void Cur_XY(int Xpos, int Ypos) {
	 LCD_RS(OFF);
	 LCD_RW(OFF);
	 if ((Ypos%2)) DATA_PORT =Ypos*20+Xpos+0xc0;
	 else DATA_PORT =Ypos*20+Xpos+0x80;
	 
	 LCD_E_Pulse();
}

void Printf(int lin, char *Str) {
	 Write_LCD(LCD_SET);
	 Cur_XY(0, lin);
	 Write_Str(Str);
}

void Scr_R_Shift(int cnt, int dtimes) {
 	 while (cnt--) {	 
	 	 Write_LCD(SCR_R_SHFT);
		 delay(dtimes);
	 }
}

void Scr_L_Shift(int cnt, int dtimes) {
 	 while (cnt--) {	 
	 	 Write_LCD(SCR_L_SHFT);
		 delay(dtimes);
	 }
}

void init_LCD(void) {
 	 Write_LCD(LCD_SET);	 // lcd module setting
 	 Write_LCD(SCR_CLR);	 // clear
 	 Write_LCD(RTN_HOME);	 // return home
 	 Write_LCD(DISP_ON);	 // cursor blank
}

int main() {
 	 int cnt=6;
	 int dtimes=1000;
	 
 	 init_devices();
	 init_LCD();
 
	 Write_LCD(SCR_CLR);
	 Printf(FST_LIN,"Larry-G");
	 Printf(SND_LIN,"Insanity");

	while (1) {
	 	 Scr_R_Shift(cnt, dtimes);	 // Shift Screen to Right
		 _delay_ms(1000);
		 Scr_L_Shift(cnt, dtimes); 	 // Shift Screen to Left
		 _delay_ms(1000);
	 }
	 return 0;
}
