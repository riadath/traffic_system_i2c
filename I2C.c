#include "I2C.h"
#include "GPIO.h"
#include "USART.h"
#include "SYS_INIT.h"
#include <stdio.h>

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
    
    if(mode == 0){
        I2C1_SetAddress(0);
    }
}

void I2C1_Start(void){
//    I2C1->CR1 |= I2C_CR1_ACK;
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

void I2C1_WriteData(char *buffer, uint8_t size){
    uint8_t idx = 0;
    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    sendString(buffer);
    sendString(" << Data to send\n");
    
    //enable peripheral
    I2C1->CR1 |= I2C_CR1_PE;
    
    //disable pos
    I2C1->CR1 &= ~I2C_CR1_POS;
    
    //generate start
    I2C1_Start();
    
    //send address
    I2C1_Address(0);
    
    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sendString("Address Sent\n");
    
    for(idx = 0;idx < size;idx++){
        I2C1_Write(buffer[idx]);
    }
    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sendString("Data Sent\n");
    
    //generate stop
    I2C1_Stop();

    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sendString("end master tranmit\n");
}

void I2C1_Read(uint8_t *buffer, uint8_t size){
    
    uint32_t idx = 0,temp_read;
    sendString("inside read\n");
    
    //peripheral enable
    I2C1->CR1 |= I2C_CR1_PE;
    
    //disable pos
    I2C1->CR1 &= ~I2C_CR1_POS;
    
    //enable ack
    I2C1->CR1 |= I2C_CR1_ACK;
    
    //wait for address to set
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    //clear address flag
    temp_read = I2C1->SR1 | I2C1->SR2;
    
    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sendString("address matched\n");
    
    for(idx = 0;idx < size;idx++){
        //wait for rxne to set
        while(!(I2C1->SR1 & I2C_SR1_RXNE));
        //read data from DR register
        buffer[idx] = (uint8_t)I2C1->DR;
    }
    
    buffer[idx] = '\0';
    
    
    
//    if (I2C1->SR1 & I2C_SR1_BTF){
//        buffer[idx] = (uint8_t)I2C1->DR;
//        
//        //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//        sendString("data read (btf) : ");
//        UART_SendChar(USART2,buffer[idx]);
//        sendString(" <<\n");
//    }
    
    
    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sendString("data received\n");
    
    
    //wait for stopf to set
    while(!(I2C1->SR1 & I2C_SR1_STOPF));
    //clear stop flag
    temp_read = I2C1->SR1;
    I2C1->CR1 |= I2C_CR1_PE;
    
    //disable ack
    I2C1->CR1 &= ~I2C_CR1_ACK;
    
    
    //DEBUG <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sendString("read end\n");

}










