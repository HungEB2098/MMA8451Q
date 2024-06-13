 #include "MKL46Z4.h"
#include "i2c.h"
#include "accel.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "switch.h"
#include "slcd.h"

volatile uint8_t state;
//Rơi tự do: 0.35g
//For a freefall algorithm a minimum of 120 ms should be considered for 
//the freefall condition to be met to be considered a linear
//freefall and not a false trigger.

int main(void) {
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
			LED_Green();
			Systick_Shutdown();
			
		} else {
			Systick_Init();
			Value data = MMA8451_Read();
			float realData = AccelValue(data);
			//This condition would need to occur for a minimum of 100 ms 
			//to ensure that the event wasn't just noise
			if (realData > 2) {
				//For Motion detection the condition is > Threshold
				//For Freefall detection the condition is < Threshold
				while (1) {
					LED_Red();
					SLCD_WriteChar('1');
				}
			}
			delay(100);
		}
    }
}
