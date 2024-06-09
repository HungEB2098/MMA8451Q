#include "MKL46Z4.h"

extern volatile uint32_t msTick;
extern volatile uint32_t LED_tick;

void Systick_Init(void);
void Systick_Shutdown(void);
void delay(uint32_t TICK);
