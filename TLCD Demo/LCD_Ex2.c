#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#define ON 1
#define OFF 0
#define DELAYTIME 10

#define control_port PORTF
#define data_port	 PORTA

void port_init(void) {
	PORTF = 0x00;
	DDRF  = 0xFF;		// Control Port
	PORTA = 0x00;
	DDRA  = 0xFF;		// Data Port
	PORTC = 0x00;
	DDRC  = 0x00;
	PORTD = 0x00;
	DDRD  = 0x00;
	PORTE = 0x00;
	DDRE  = 0x00;
	//PORTF = 0x00;
	//DDRF  = 0x00;
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
void delay(int dd){
    int i,j;
    
    for(i=0 ; i<1000 ; i++){
        for(j=0 ; j<dd ; j++){
        }
    }
}

// RS : PORTA D0
void LCD_RS(int onoff){
    if(onoff == ON)
        control_port |= 0x01;
    else
        control_port &= 0xFE;
}
// RW : PORTA D1
void LCD_RW(int onoff){
    if(onoff == ON)
        control_port = control_port | 0x02;
    else
        control_port = control_port & 0xFD;
}
// RS : PORTA D2
void LCD_E(int onoff){
    if(onoff == ON)
        control_port = control_port | 0x04;
    else
        control_port = control_port & 0xFB;
}

void E_Pulse(void){
	 LCD_E(ON);
	 delay(DELAYTIME*10);
	 LCD_E(OFF);
	 delay(DELAYTIME*10);
}

void write_char(char s){
	 LCD_RS(ON);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 data_port = s;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}	

void clrscr(){
	 LCD_RS(OFF);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 data_port = 0x01;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}

void returnhome(){
	 LCD_RS(OFF);
	 delay(DELAYTIME);		 
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
 	 delay(DELAYTIME);
	 data_port = 0x02;
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
	 	 data_port = 0x0F;
	 }else{
	 	 data_port = 0x08;
	 }
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}


void lcd_disp(char x, char y) {
	LCD_RS(OFF);
	LCD_RW(OFF);
	if(y==1)y++;
	else if(y==2) y--;
	if(y<=1){
        data_port=y*20+x+0x80;
	} else {
        y=2;
        data_port=y*20+x+0xc0;
	}
	E_Pulse();
}

void write_lcd(char x, char y, char *str) {
	lcd_disp(x,y);
	while(*str) write_char(*str++);
}

int main(void){
     init_devices();
     returnhome();
	 clrscr();
	 
	 while(1){
	    LCDONOFF(ON);
		_delay_ms(1000);
	 	write_char('s');
		LCDONOFF(OFF);
		_delay_ms(1000);

	 }
	 return 0;
}

