#include "I2C.h"
#include "GPIO.h"
#include "USART.h"
#include "SYS_INIT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void I2C1_Config(uint8_t mode){
    //Enable I2C and GPIO
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    
    //Configure Pin 8,9 (GPIOB)
    GPIOB->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1);
    GPIOB->OTYPER |= (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0);
    GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2);
    
    //Configure I2C1
    
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    
    //master mode config
    /*
        Fpcklk1 = 45 mhz
        Tpclk1 = 1/45 mhz
        CCR = (Tr(scl) + Tw(sclh)) / (Tpcklk1)
        [using values from datasheet table 61]
        CCR = 225
    
        Trise = Tr(scl) / Tpcklk1 + 1
        [using values from datasheet table 61]
        Trise = 46
        
    */
    
    I2C1->CR2 |= (45 << I2C_CR2_FREQ_Pos);
    I2C1->CCR = 225 << I2C_CCR_CCR_Pos;
    I2C1->TRISE = 46 << I2C_TRISE_TRISE_Pos;

    I2C1->CR1 |= I2C_CR1_PE;
    I2C1->CR1 &= ~I2C_CR1_POS;
    
    if(mode == 0){
        /*Set Address and Enable Interrupts for Slave Receieve*/
        I2C1_SetAddress(0);
        
        I2C1->CR2 |= I2C_CR2_ITEVTEN; /*Enable Event Interrupt*/
        I2C1->CR2 |= I2C_CR2_ITERREN; /*Enable Error Interrupt*/
        I2C1->CR2 |= I2C_CR2_ITBUFEN; /*Enable Buffer Interrupt*/
        
       
        NVIC_EnableIRQ(I2C1_EV_IRQn);  
    }
}

void I2C1_Start(void){
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C1_Stop(void){
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C1_Address(uint8_t address){
    I2C1->DR = (uint8_t)(address << 1);
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    //Clear ADDR (read SR1 and SR2) 
    address = (uint8_t)(I2C1->SR1 | I2C1->SR2);
}


void I2C1_SetAddress(uint8_t address){
    I2C1->OAR1 |= (uint32_t)(address << 1U);
    I2C1->OAR1 &= ~I2C_OAR1_ADDMODE;
}


void I2C1_Write(uint8_t data){
    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while(!(I2C1->SR1 & I2C_SR1_BTF));
}


/*read and write functions*/
void I2C1_TransmitMaster(char *buffer, uint32_t size){
    I2C1_Start();    /* generate start */
    I2C1_Address(0); /* send address */
    for(int idx = 0;idx < (int)size;idx++)I2C1_Write(buffer[idx]);/* send data */
    I2C1_Stop();  /* generate stop */
}






char* I2C1_ReceiveSlave(uint8_t *buffer){
    uint32_t idx = 0,size = 0;
    uint8_t ch = 0;
    char ret[100];

    while(!(I2C1->SR1 & I2C_SR1_ADDR)); /*wait for address to set*/
    idx = I2C1->SR1 | I2C1->SR2; /*clear address flag*/
    
    idx = 0;
    while(ch != '@'){
        while(!(I2C1->SR1 & I2C_SR1_RXNE)); /*wait for rxne to set*/
        ch = (uint8_t)I2C1->DR;/*read data from DR register*/
        if(ch == '@')break;
        buffer[idx++] = ch;
        size++;
    }
    buffer[idx] = '\0';
    
    while(!(I2C1->SR1 & I2C_SR1_STOPF)); /*wait for stopf to set*/
    /*clear stop flag*/
    idx = I2C1->SR1; 
    I2C1->CR1 |= I2C_CR1_PE;
    
    I2C1->CR1 &= ~I2C_CR1_ACK; /*disable ack*/
    
    
    for(idx = 0;idx < size;idx++){
        ret[idx] = buffer[idx];
    }ret[idx] = '\0';
    
    return ret;
}










