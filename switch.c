#include "MKL46Z4.h"
#include "delay.h"
#include "switch.h"

void Switch_Init(void) {
	state = 0;
	//PTC3 - SW1
	//PTC12 - SW2
	SIM->SCGC5 |= (1<<11); //Eable clock portC
	PORTC->PCR[3] |= (1<<8); //Alternative 1 (GPIO)
	PORTC->PCR[3] |= (1<<0); //Internal pullup resistor is enabled
	PORTC->PCR[3] |= (1<<1); //PE: Internal pullup or pulldown resistor is enabled
	PORTC->PCR[3] |= (1<<17) | (1<<19); //IRCQ = 1010: Interupt on falling edge
	PTC->PDDR &= (~(uint32_t)(1<<3)); //Input mode

	PORTC->PCR[12] |= (1<<8);
	PORTC->PCR[12] |= (1<<0);
	PORTC->PCR[12] |= (1<<1);
	PORTC->PCR[12] |= (1<<17) | (1<<19);
	PTC->PDDR &= (~(uint32_t)(1<<12));
	
	//Pin detect (Single interrupt vector for Port C and Port D)
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}

void PORTC_PORTD_IRQHandler(void) {
	if ((PTC->PDIR & (1<<3)) == 0) { //Pressed SW1 
		if (state == 0 ) state = 1;
		else if (state == 1) state = 0;
	}

	//Clear interrupt flag
	PORTC->PCR[3] |= (1 << 24);
	
	if ((PTC->PDIR & (1<<12)) == 0) { //Pressed SW2 
		state = 0;
		NVIC_SystemReset();
	}
	PORTC->PCR[12] |= (1<<24);
}