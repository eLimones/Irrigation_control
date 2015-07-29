#include "clock.h"
#include "stm32f0xx.h"

/**
 * Ticks elapsed
 */
static volatile uint32_t clock_tick;
static volatile uint32_t clock_tick_parity;

/**
 * xDDHHMMSS
 */
static volatile uint32_t CLOCK_TIME;

/**
 * Initialized the timer 15 as a downcounter periodic timer
 * Also enables the update interrupt for this timer
 * @param period_ms timer period in milliseconds
 */
void timer_15_init(int period_ms);


void clock_init(void){
	clock_tick=0;
	timer_15_init(1000);//tick every 1 second
}

uint8_t clock_getSeconds(void){
	return clock_tick;
}

uint32_t clock_getTime(void){
	return CLOCK_TIME;
}

void clock_wait(uint16_t waitSeconds){
	uint32_t start_parity=clock_tick_parity;
	uint32_t start_time=clock_tick;
	
	if(start_parity!=clock_tick_parity){//if interrupt and overflow happened while loading values
		start_parity=clock_tick_parity;
		start_time=clock_tick;
	}
	
	uint32_t end_time=clock_tick+waitSeconds;
	uint32_t end_parity=(end_time<start_time)?(!start_parity):start_parity;
	
	while(1){
		if(((clock_tick_parity==end_parity)&&(clock_tick>=end_time)) || 
				((start_parity==end_parity)&&(clock_tick<end_time)&&(clock_tick_parity!=end_parity))){
			break;
		}
	}
}

void timer_15_init(int period_ms){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
	TIM_TimeBaseInitTypeDef myTimer;
	TIM_TimeBaseStructInit(&myTimer);
	myTimer.TIM_CounterMode=TIM_CounterMode_Down;
	myTimer.TIM_Prescaler=(8000-1);//CountFreq=(8MHz/Prescaler+1)=1KHz
	myTimer.TIM_ClockDivision=TIM_CKD_DIV1;
	myTimer.TIM_Period=period_ms-1;
	TIM_TimeBaseInit(TIM15,&myTimer);
	
	TIM_InternalClockConfig(TIM15);
	
	TIM_ITConfig(TIM15,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM15_IRQn);
	
	TIM_Cmd(TIM15,ENABLE);
}

void TIM15_IRQHandler(void){
	
	TIM_ClearITPendingBit(TIM15,TIM_IT_Update);
	
	clock_tick++;
	if(clock_tick==0) clock_tick_parity=!clock_tick_parity;
	
	uint32_t s,//second
			m,//minutes
			h,//hours
			d;//days
	s=(CLOCK_TIME>>0)&(0xFF);
	m=(CLOCK_TIME>>8)&(0xFF);
	h=(CLOCK_TIME>>16)&(0xFF);
	d=(CLOCK_TIME>>24)&(0xFF);
	
	s++;
	if(s>=60){
		m++;
		s=0;
	}
	if(m>=60){
		h++;
		m=0;
	}
	if(h>=24){
		d++;
		h=0;
	}if(d>=7){
		d=0;
	}
	
	CLOCK_TIME=(d<<24)|(h<<16)|(m<<8)|(s<<0);
}
