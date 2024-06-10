#include "MKL46Z4.h"
#include "i2c.h"
#include "delay.h"

void I2C_Init(void) {
    SIM->CLKDIV1 |= (1u<<17) | (1u<<16); //bus clock is 24/3 = 8MHz

    SIM->SCGC5 |= (1<<13); //clock to PTE24 and PTE25 for I2C0
    SIM->SCGC4 |= (1<<6); //clock to I2C0
    
    PORTE->PCR[24] |= (1<<8) | (1<<10) & (~(1<<9)); //alternative 5 - 101 for bits 10, 9 8 respectively
    PORTE->PCR[25] |= (1<<8) | (1<<10) & (~(1<<9));
    
    I2C0->F = 0x80; //mult=2h ICR=00h
    I2C0->C1 = 0xB0; //10110000 - module enable, interrupt disable, master, transmit
    //acknowledge bit sent,repeated start off, wake up off, DMA off
    I2C0->C2 = 0x00;
}

 void I2C_WriteRegister(uint8_t device_address, uint8_t register_address, uint8_t data) {
     I2C_Start();
		 address(device_address, register_address);
	
     // Send data
     I2C0->D = data;
     Wait();
	
		 I2C_Stop();
 }

 void address(uint8_t device_address, uint8_t register_address) {
     // Send device address with write bit
     I2C0->D = device_address << 1;
     Wait();
     // Send register address
     I2C0->D = register_address;
     Wait();
 }

 uint8_t I2C_ReadRegister(uint8_t device_address, uint8_t register_address) {
     uint8_t result;
     I2C_Start();
	
     address(device_address, register_address);
    
     //Repeated start to change to read mode
     I2C0->C1 |= (1<<2);
	
     //Send device address
     I2C0->D = (device_address << 1) | 0x01;
     Wait();
	
     //Recieve mode
     I2C0->C1 &= ~(1<<4);

		 //Send NACK
     I2C0->C1 |= (1<<3); //Transmit Acknowledge Enable 
    // In master receive mode, the FACK bit must be set to zero
    // before the last byte transfer
    //-->
    // No acknowledge signal is sent to the bus on the 
    // following receiving data byte (if FACK is cleared)
	
     //Fake read
     result = I2C0->D;
     Wait();
	
     //Read data
     result = I2C0->D;
			Wait();
	
		 //Send stop signal
     I2C0->C1 &= ~(1<<5);
    
     return result;
 }

void I2C_Start(void) {
   I2C0->C1 |= (1 << 4); // Transmit mode
   I2C0->C1 |= (1 << 5); // Master mode
}

void I2C_Stop(void) {
   I2C0->C1 &= ~(1 << 5);// Send stop signal
   I2C0->C1 &= ~(1 << 4);// Set to receive mode
}

void Wait(void) {
    while (!(I2C0->S & (1<<1)));
    I2C0->S |= (1<<1);
}




