#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define control_port PORTF
#define data_port	 PORTA

#define ON 1
#define OFF 0
#define DELAYTIME 10
#define RIGHT 1
#define LEFT 0

void port_init(void)
{
 PORTF = 0x00;
 DDRF  = 0xFF;
 PORTA = 0x00;
 DDRA  = 0xFF;
 PORTC = 0x00; //m103 output only
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
void init_devices(void)
{
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

// RS : PORTA A0
void LCD_RS(int onoff){
    if(onoff == ON)
        control_port = control_port | 0x01;
    else
        control_port = control_port & 0xFE;
}
// RW : PORTA A1
void LCD_RW(int onoff){
    if(onoff == ON)
        control_port = control_port | 0x02;
    else
        control_port = control_port & 0xFD;
}
// E : PORTA A2
void LCD_E(int onoff){
    if(onoff == ON)
        control_port = control_port | 0x04;
    else
        control_port = control_port & 0xFB;
}

void LCD_Clr(){
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


void lcd_disp(char x, char y){
	 LCD_RS(OFF);
	 delay(DELAYTIME);
	 LCD_RW(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
	 if(y==0){
	 	data_port = x+0x80;
	 }
	 else{
	 	data_port = x+0xC0;
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
	 	data_port = 0x1C;
	 else
	 	data_port = 0x18;
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
	 	 data_port = 0x30;
	 else
	 	 data_port = 0x3C;
	 delay(DELAYTIME);
	 LCD_E(OFF);
	 delay(DELAYTIME);
	 LCD_E(ON);
	 delay(DELAYTIME);
}

void init_lcd(){
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

int main(void) {
	 int i, t;
	 char str1[40] = {"TEXT_LCD"};
	 char str2[40] = {"TEST_MODE"};
	 
	 init_devices();
	 init_lcd();
	 PORTG = 0x03;
	 LCDONOFF(ON);
     returnhome();
	 LCD_Clr();
	 write_lcd(0,0,str1);
	 write_lcd(0,1,str2);

	 for(i=0; i<4 ; i++){
		_delay_ms(1000);
		LCD_Shift(RIGHT);
	 }
	 while(1){
	 	for(i=0; i<4  ; i++){
			if(t%2 == 0){
				_delay_ms(1000);
				LCD_Shift(LEFT);
			}
			else{
				_delay_ms(1000);
				LCD_Shift(RIGHT);
			}
		}
		t-=-1;
	 }
	 return 0;
}

