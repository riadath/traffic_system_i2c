#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "I2C.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/*********************
    USER INPUT
*********************/

static char input_buff[50];
void USART2_IRQHandler(void);
void getString(void);
void sendString(char *str);
void getString(void){
    uint8_t ch,idx = 0;
    ch = UART_GetChar(USART2);
    while(ch != '!'){
        input_buff[idx++] = ch;
        ch = UART_GetChar(USART2);
        if(ch == '!')break;
    }      
    input_buff[idx] = '\0';
    
}
void USART2_IRQHandler(void){
    USART2->CR1 &= ~(USART_CR1_RXNEIE);
    getString();
    USART2->CR1 |= (USART_CR1_RXNEIE);
}

void sendString(char *str){
    UART_SendString(USART2,str);
}



int main(void)
{   
		
	/*	Configuration */
	initClock();
	sysInit();
	UART2_Config();
    NVIC_SetPriority(USART2_IRQn, 1);
    NVIC_EnableIRQ(USART2_IRQn);
    
		
	while(1){
		
		ms_delay(2000);
		
		if(strlen(input_buff) != 0){
			UART_SendString(USART2,input_buff);
			strcpy(input_buff,"");
		}
		
        sendString("hello running<<<<\n");
	}
}

