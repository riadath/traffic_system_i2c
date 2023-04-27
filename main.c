#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "I2C.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static char input_buff[50];
static uint8_t rcv_ptr;
static char *rcv_str;


void USART2_IRQHandler(void);
void I2C1_EV_IRQHandler(void);

void getString(void);
void sendString(char *str);
void inf_read_loop(void);


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


void I2C1_EV_IRQHandler(void){ 
    
//    NVIC_DisableIRQ(I2C1_EV_IRQn);
    I2C1->CR2 &= ~I2C_CR2_ITEVTEN;

    if(I2C1->SR1 & I2C_SR1_ADDR)
        rcv_str = I2C1_ReceiveSlave(&rcv_ptr,(uint32_t)4);     

    //rcv_str = (char *)&rcv_ptr;

    if(strlen(rcv_str) != 0){
            sendString("RCV :::");
            sendString(rcv_str);
            sendString(":::\n");
            strcpy(input_buff,"");
        }
//    NVIC_EnableIRQ(I2C1_EV_IRQn);
    I2C1->CR2 |= I2C_CR2_ITEVTEN;
}



int main(void)
{   
	uint8_t i = 0;
    GPIO_InitTypeDef gpio_config;
    gpio_config.Pin = GPIO_PIN_5;
    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    
    GPIO_Init(GPIOA,&gpio_config);
    
	/*	Configuration */
	initClock();
	sysInit();
	UART2_Config();
    I2C1_Config(i);
    
    strcpy(input_buff,"");
    sendString("HELLO I'M IN\n");
    
    if(i == 0){
        sendString("Inside Read Loop\n");
    strcpy(input_buff,"");
    while(1){   
        I2C1_Config(i);
        I2C1->CR1 |= I2C_CR1_ACK;
        GPIO_WritePin(GPIOA,5,GPIO_PIN_SET);
        ms_delay(500);
        GPIO_WritePin(GPIOA,5,GPIO_PIN_RESET);
        ms_delay(500);
        }
    }

    sendString("Inside Write Loop\n");
    while(i){
        if (strlen(input_buff) != 0){          
            sendString(input_buff);
            sendString(" << Data to send\n");
            
            I2C1_TransmitMaster(input_buff,strlen(input_buff));
            strcpy(input_buff, "");
        }
    }
    
    
}








