/*
 * ----------------------------------------------------------------------------
 * FAT32 demo with SD/SDHC card
 *                         
 * Author       : Jungho Moon
 * Target MCU   : ATMEL AVR ATmega128/64
 *
 * ÀÌ ¿¹Á¦´Â SD/SDHC ¸Þ¸ð¸® Ä«µå Á¤º¸¿Í ÀúÀåµÈ ³»¿ëÀ» ÀÐ´Â ¹æ¹ýÀ»
 * ¼³¸íÇÏ±â À§ÇÑ °ÍÀÌ´Ù.
 *
 * Åë½Å ÅÍ¹Ì³Î¿¡ ¡®h¡¯¸¦ ÀÔ·ÂÇÏ¸é µµ¿ò È­¸éÀ» Ãâ·ÂÇÑ´Ù.
 * Åë½Å ÅÍ¹Ì³Î¿¡ ¡®i¡¯¸¦ ÀÔ·ÂÇÏ¸é ¸Þ¸ð¸® Ä«µå Á¤º¸¸¦ Ãâ·ÂÇÑ´Ù.
 * Åë½Å ÅÍ¹Ì³Î¿¡ ¡®m¡¯¸¦ ÀÔ·ÂÇÏ¸é ¿øÇÏ´Â ¼½ÅÍÀÇ ³»¿ëÀ» ÀÐÀ» ¼ö ÀÖ´Ù.
 * ----------------------------------------------------------------------------
 */

#define SYS_GLOBALS

#include "bsp.h"
#include "uart.h"
#include "spi.h"
#include "mmc.h"
#include "sd_card_reader_demo.h"

/* ----------------------------------------------------------------------------
 * generate a delay in seconds (maximum value: 25.5)
 * -------------------------------------------------------------------------- */
static void _delay_s(float seconds)
{
    uint16_t i;
    
    for(i=0; i<(uint16_t)(100*seconds+0.5); i++)
        _delay_ms(10);
}

/* ----------------------------------------------------------------------------
 * handler for timer0 compare match interrupt
 * -------------------------------------------------------------------------- */
ISR(TIMER0_COMP_vect)
{
}

/* ----------------------------------------------------------------------------
 * handler for external interrupt INT7
 * -------------------------------------------------------------------------- */
ISR(INT7_vect)
{
}

/* ----------------------------------------------------------------------------
 * initialize I/O ports
 * -------------------------------------------------------------------------- */
void ioport_init(void)
{
    /* SD Ä«µå¸¸ testÇÏ±â¸¦ ¿øÇÒ ¶§¿¡µµ SPI ¹ö½º¿¡ ¿©·¯ µð¹ÙÀÌ½º°¡ °øÅëÀ¸·Î
     * ¿¬°áµÇ¾î ÀÖ´Ù¸é SD Ä«µå¸¦ Á¦¿ÜÇÑ ´Ù¸¥ ¸ðµç SPI ÀåÄ¡ÀÇ CS´Â 'H'·Î ¼³Á¤ÇØ
     * disable½ÃÄÑ¾ß¸¸ SD Ä«µå¿ÍÀÇ Åë½Å¿¡ °£¼·À» ÀÏÀ¸Å°Áö ¾Ê´Â´Ù. */
        
    // PORT B
    DDRB = _BV(SD_CS) | _BV(FLASH1_CS) | _BV(FLASH2_CS) | _BV(LCD_BL_EN);
    PORTB = _BV(SD_CS) | _BV(FLASH1_CS) | _BV(FLASH2_CS) | _BV(LCD_BL_EN);
    
    // PORT C
    DDRC = _BV(VCC3_EN);
    
    // PORT D
    DDRD = _BV(MP3_xCS) | _BV(MP3_xDCS);
    PORTD = _BV(MP3_xCS) | _BV(MP3_xDCS);

    // PORT F
    PORTF = _BV(SD_DET) | _BV(SD_WP);           // enable pullup resistors
}

/* ----------------------------------------------------------------------------
 * initialize timers
 * -------------------------------------------------------------------------- */
void timer_init(void)
{
}

/* ----------------------------------------------------------------------------
 * initialize ADC
 * -------------------------------------------------------------------------- */
void adc_init(void)
{
}

/* ----------------------------------------------------------------------------
 * initialize interrupt-related SFRs
 * -------------------------------------------------------------------------- */
void interrupt_init(void)
{
}

/* ----------------------------------------------------------------------------
 * initialize ATmega128 on-chip peripherals
 * -------------------------------------------------------------------------- */
void atmega128_peripherals_init(void)
{
    // initialize on-chip peripherals
    ioport_init();
    timer_init();
    spi_master_init();
    adc_init();
    uart_init();
    interrupt_init();    
}

/* ----------------------------------------------------------------------------
 * initialize off-chip peripherals
 * -------------------------------------------------------------------------- */
void off_chip_peripherals_init(void)
{
    v30_enable();                           // enable 3.0V regulator
    _delay_ms(1);                           // wait until 3.0V power is stabilized
}
    
/* ----------------------------------------------------------------------------
 * display help screen
 * -------------------------------------------------------------------------- */
void display_help(void)
{
    printf("\r\n\n");
    printf("*===========================================================*\r\n");
    printf("                  SD/SDHC Carder Reader             \r\n");
    printf("    Baud Rate: 115200, 8 Data Bits, 1 Stop Bit, No Parity     \r\n");
    printf("         Dept. of EE, Kangnung National University           \r\n\n");
    printf("          [H]: help\r\n");
    printf("          [I]: show disk information\r\n");
    printf("          [M]: show memory contents\r\n");
    printf("*===========================================================*\r\n\n");
}

uint8_t ascii2num(uint8_t ch)
/* ----------------------------------------------------------------------------
 * converts an ASCII character to a numberÍ
 * arguments
 *  - ch: hexadecimal character between '0' and 'F'
 * return value
 *  - number corresponding to ch
 * example
 *   ascii2num('3') returns 3
 * -------------------------------------------------------------------------- */
{
    if(ch >= '0' && ch <= '9')
        return(ch - '0');
    else if(tolower(ch) >= 'a' && tolower(ch) <= 'f')
        return(tolower(ch) - 'a' + 10);
    else 
        return 0xff;
}


/* ----------------------------------------------------------------------------
 * show memory card information
 * -------------------------------------------------------------------------- */
void show_memory_card_info(void)
{
    MMC_CARD_INFO mmc_card_info;
    
    if(!mmc_get_card_info(&mmc_card_info)) {
        printf("\n\r\n*===========================================================*\r\n");
        printf("                MEMORY CARD INFORMATIION\n\r");
        printf("             Manufacturer ID (HEX): %03X\n\r", mmc_card_info.manufacturer);
        printf("             OEM ID               : %s\n\r", &mmc_card_info.oem[0]);
        printf("             Product name         : %s\n\r", mmc_card_info.product);
        printf("             Revision (HEX)       : %x\n\r", mmc_card_info.revision);
        printf("             Serial number        : %lu\n\r", mmc_card_info.serial);        
        printf("             Manufactured date    : %d/%02d\n\r", mmc_card_info.manufacture_year+2000, mmc_card_info.manufacture_month);        
        printf("             Size                 : %lu MB\n\r", (uint32_t)(mmc_card_info.capacity/1000/1000));
        printf("*===========================================================*\r\n\n");
    }
}

/* ----------------------------------------------------------------------------
 * show memory contents
 * -------------------------------------------------------------------------- */
void show_memory_contents(void)
{
    #define BYTES_PER_SECTOR    512
    
    uint8_t ch, value[16], i, j;
    uint8_t mmc_buffer[BYTES_PER_SECTOR], *buffer_ptr;
    uint32_t sector;
    
    printf("\r\n");    
    printf("\r\n Enter sector number (HEX): ");

    sector = 0;
    while((ch = uart_getch()) != '\r') {
        uart_putch(ch);
        if(isxdigit(ch))
            sector = (sector<<4) + ascii2num(ch);
        else {
            printf(" \n\r Wrong number\n\r");
            return;
        }
    }
    printf("\n\r");

    do {  
        mmc_read_block(sector, mmc_buffer);           
        printf("\n\r Sector number (HEX): %04lX\n\r", sector);
        
        buffer_ptr = mmc_buffer;
        for(i=0; i<BYTES_PER_SECTOR/16; i++) {        
            printf(" %04X : ", i*16);
            for(j=0; j<0x10; j++) {
                value[j] = *buffer_ptr++;
                printf("%02X ", value[j]);
                if(j==7)
                    printf("- ");
            }
            printf("    ");
            
            for(j=0; j<0x10; j++) {
                if(value[j] >= ' ' && value[j] <= '~')
                    uart_putch(value[j]);
                else
                    uart_putch('`');
            }
            printf("\n\r");
        }
    
        printf("\n\r Enter RETURN to continue or 'q' to stop: ");
        uart_putch(ch = uart_getch());
        if(ch == '\r')
            sector++;        
        printf("\r\n");
    } while(tolower(ch) != 'q');
    
    printf("\n");
}

/* ----------------------------------------------------------------------------
 * entry point to the program
 * -------------------------------------------------------------------------- */
int16_t main(void)
{
    int8_t ch;
    uint16_t i;

    fdevopen(uart_putch, uart_getch);   // printf are directed to UART
    atmega128_peripherals_init();
    off_chip_peripherals_init();
    
    if(!sd_card_check()) {
        printf(">>\n\r SD/SDHC CARD NOT FOUND. INSERT A CARD.\r\n");
        while(!sd_card_check()) ;
        _delay_s(0.2);
    } 
    
    if(mmc_init()) {
        printf(">>\n\r SD/SDHC MEMORY CARD INITIALIZATION FAILURE! RESET THE BOARD.\n\r"); 
        while(1) ;
    }
     
    display_help();
    while(1) {
        uart_puts(">> ");
        ch = uart_getch();
        uart_putch(ch);
        
        switch(tolower(ch)) {
            case 'h':  
                display_help(); 
                break;
            case 'i':
                show_memory_card_info();
                break;                
            case 'm': 
                show_memory_contents();
                break;                
            case '\r':
                uart_puts("\r\n");   
                break;
            default: 
                uart_puts("\r\n Error: Unknown command\r\n"); 
                break;
        }
    }
   
    return 0;
}

