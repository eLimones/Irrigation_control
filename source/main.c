#include "stm32f0xx.h"                  // Device header
#include "retarget_STM32F0.h"
#include "serial_stdio.h"
#include "LCD.h"
#include "encoder.h"
#include "clock.h"
void setToMaxSpeed(void);
Serial_t UART2_serial = {UART2_getChar, UART2_sendChar};
Serial_t LCD_serial = {NULL, lcd_sendChar};

#define IN_BUFFER_SIZE 40
char inputBuffer[IN_BUFFER_SIZE];


const char* dias[]={"Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo"};

int timer_count;
int last_timer_count;
volatile uint32_t time_val;

int main(void)
{	
	clock_time_t * myClock=(clock_time_t *) &time_val;
	encoder_init();
	UART2_init(9600);
	clock_init();
	lcd_init();
	serial_printf(UART2_serial,"\nSystem ready\n");
	serial_printf(LCD_serial,"\fPOS %d",encoder_getPosition());
	
	while(1){
		if(encoder_hasChanged()){
			lcd_goto(4,0);
			serial_printf(LCD_serial,"%d  ",encoder_getPosition());
		}
		timer_count=clock_getSeconds();
		if(last_timer_count!=timer_count){
			time_val=clock_getTime();
			lcd_goto(0,1);
			serial_printf(LCD_serial,"%7s:%2d:%2d:%2d",dias[myClock->days],myClock->hours,myClock->minutes,myClock->seconds);
			last_timer_count=timer_count;
		}
	}
}
