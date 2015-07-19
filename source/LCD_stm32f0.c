#include "LCD_hw.h"
#include "stm32f0xx.h"                  // Device header

/*
	Connection diagram
	LCD uC
18 D4 PA8 I/O FT
19 D5 PA9 I/O FT
20 D6 PA10 I/O FT
21 D7 PA11 I/O FT
Control LCD
26 E PB3 I/O FT
27 RW PB4 I/O FT
28 RS PB5 I/O FT
	All pins must have a external 10k  pull-up resistor to 5v
*/

void lcd_hw_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	myGPIO.GPIO_OType=GPIO_OType_OD;
	myGPIO.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&myGPIO);
	
	myGPIO.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOB,&myGPIO);
	
	
}

void lcd_hw_outputCtrl(unsigned char ctrl_pin, unsigned char newState){
	if(PIN_E==ctrl_pin){
		GPIO_WriteBit(GPIOB,GPIO_Pin_3,((newState)?(Bit_SET):(Bit_RESET)));
	}else if(PIN_RW==ctrl_pin){
		GPIO_WriteBit(GPIOB,GPIO_Pin_4,((newState)?(Bit_SET):(Bit_RESET)));
	}else if(PIN_RS==ctrl_pin){
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,((newState)?(Bit_SET):(Bit_RESET)));
	}
}

void lcd_hw_output_LowNibble(unsigned char lowNibbleData){
	uint16_t setBits=0;
	uint16_t resetBits=0;
	setBits=lowNibbleData&0xF;
	resetBits=(~lowNibbleData)&0xF;
	GPIO_SetBits(GPIOA,setBits<<8);
	GPIO_ResetBits(GPIOA,resetBits<<8);
}

void lcd_hw_output_data(unsigned char data){
	//not implemented
}

void lcd_hw_delay_us(int delay_time){
	for(int i=0;i<(delay_time*0x2F);i++){
	}
}

void lcd_hw_delay_ms(int delay_time){
	for(int i=0;i<delay_time;i++){
		lcd_hw_delay_us(1000);
	}
}
