#include "MKL46Z4.h"
#include "i2c.h"
#include "accel.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "switch.h"
#include "slcd.h"

volatile uint8_t state;

int main(void) {
	UART_Init();
	Systick_Init();
  	I2C_Init();
  	MMA8451_Init();
	delay(1);
	LED_Init();
	Switch_Init();
	SLCD_Init();
	SLCD_EnablePins();
	SLCD_WriteChar('0');
	
    while (1) {
		if (state ==0) {
			LED_Active_Reset();
			Systick_Shutdown();
			
		} else {
			Systick_Init();
			Value data = MMA8451_Read();
			float realData = AccelValue(data);
			if (realData > 2.5) {
				while (1) {
					LED_Fall_Detected();
					SLCD_WriteChar('1');
				}
			}
			delay(100);
		}
    }
}
