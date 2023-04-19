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

static char input_buff[50],recv_buff[50];
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



int main(void)
{   
	uint8_t i = 0;
    uint8_t rcv_ptr;

	/*	Configuration */
	initClock();
	sysInit();
	UART2_Config();
    I2C1_Config(i);
    
    NVIC_SetPriority(USART2_IRQn, 1);
    NVIC_EnableIRQ(USART2_IRQn);
    strcpy(input_buff,"iamademigod");
    if (i == 1){
        I2C1_WriteData(input_buff,strlen(input_buff));
    }
    else{
        I2C1_Read(&rcv_ptr,strlen(input_buff));
        
        sendString("Data Read : ");
        char *rcv_str = (char *)&rcv_ptr;
        sendString(rcv_str);
        sendString(" <<<::: end slave receive\n");
        
    }
    
    
}

