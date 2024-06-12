#include "MKL46Z4.h"
#include "delay.h"

volatile uint32_t msTick;
volatile uint32_t LED_tick;
//volatile

// typedef struct
// {
// __IO uint32_t CTRL; /*!< Offset: 0x000 (R/W) SysTick Control and Status Register */
// __IO uint32_t LOAD; /*!< Offset: 0x004 (R/W) SysTick Reload Value Register */
// __IO uint32_t VAL; /*!< Offset: 0x008 (R/W) SysTick Current Value Register */
// __I uint32_t CALIB; /*!< Offset: 0x00C (R/ ) SysTick Calibration Register */
// } SysTick_Type;

void Systick_Init(void) {
	SysTick->CTRL |= (1 << 0); //Enable counter
	SysTick->CTRL |= (1 << 1); //Enable interrupt
	SysTick->CTRL |= (1 << 2); //Clock source is processor clock
	SysTick->LOAD = SystemCoreClock/1000; //1ms
	
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 15);
}

void Systick_Shutdown(void) {
	SysTick->CTRL &= ~((uint32_t)(1 << 0));
}

void SysTick_Handler(void) {
	msTick++;
	LED_tick++;
	if (LED_tick == 500) { //500ms
		PTD->PTOR |= 1 <<5;//Toggle LED xanh
		LED_tick = 0;
	}
}

void delay(uint32_t TICK) {
	while (msTick < TICK);
	msTick = 0;
}
