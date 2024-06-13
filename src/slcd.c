#include "MKL46Z4.h"
#include "slcd.h"

const unsigned char Pin_Table[ ] =
{
    LCD_1a,     
    LCD_1b,     
    LCD_2a,     
    LCD_2b,     
    LCD_3a,     
    LCD_3b,     
    LCD_4a,     
    LCD_4b,     
    LCD_COM0,   
    LCD_COM1,   
    LCD_COM2,   
    LCD_COM3,   
};

void SLCD_Init(void)
{
	SIM->SCGC5 |= (1<<10) | (1<<11) | (1<<12) | (1<<13) | (1<<19); //BCDE and LCD clock enable

    // configure pins for LCD operation
    PORTC->PCR[20] = 0x00000000;     //VLL2
    PORTC->PCR[21] = 0x00000000;     //VLL1
    PORTC->PCR[22] = 0x00000000;     //VCAP2
    PORTC->PCR[23] = 0x00000000;     //VCAP1
    //Các pin ở trên là các pin dùng để cấp nguồn cho LCD

    // Enable IRCLK
    MCG->C1 = (1<<1) //Internal Reference Select
    | (1<<0); //Internal Reference Stop Enable

    //0 32KHZ internal reference clock; 1= 4MHz IRC -> p155
    MCG->C2 &= ~MCG_C2_IRCS_MASK; 

    /* LCD configurartion according to */
    LCD->GCR |= LCD_GCR_RVEN(0) //Regulated voltage disabled
		| LCD_GCR_RVTRIM(0)				//Regulated Voltage Trim 0
		| LCD_GCR_CPSEL(1)				//LCD charge pump is selected. Resistor network disabled
		| LCD_GCR_LADJ(3)					//Load Adjust, Slowest clock source for charge pump (LCD glass capacitance 1000 pF or 500pF or lower if FFR is set)	
		| LCD_GCR_VSUPPLY(0)			//Drive VLL3 internally from VDD
		| LCD_GCR_PADSAFE(0)			//LCD frontplane and backplane functions enabled according to other LCD control bits
		| LCD_GCR_FDCIEN(0)				//LCD Fault Detection Complete Interrupt Disenable, No interrupt request is generated by this event
		| LCD_GCR_ALTDIV(0)				//LCD Alternate Clock Divider, Divide factor = 1 (No divide)
		| LCD_GCR_ALTSOURCE(0) 		//Select Alternate Clock Source 1 (default)
		| LCD_GCR_FFR(0)					//Fast Frame Rate Select, Standard Frame Rate LCD Frame Freq: 23.3 (min) 73.1 (max)
		| LCD_GCR_LCDDOZE(0)			//LCD Doze Disable, Allows the LCD driver, charge pump, resistor bias network, and voltage regulator to continue running during Doze mode
		| LCD_GCR_LCDSTP(0)				//LCD Stop, Allows the LCD driver, charge pump, resistor bias network, and voltage regulator to continue running during Stop mode
		| LCD_GCR_LCDEN(1)				//LCD Driver Enable, Starts LCD controller waveform generator
		| LCD_GCR_SOURCE(1)				//LCD Clock Source Select the default clock as the LCD clock source.
		| LCD_GCR_LCLK(4)					//LCD Clock Prescaler, Used as a clock divider to generate the SLCD frame frequency,LCD controller frame frequency = LCD clock/((DUTY + 1) � 8 � (4 + LCLK[2:0]) � Y)
		| LCD_GCR_DUTY(3);				//LCD duty select, 011 Use 4 BP (1/4 duty cycle)

    // Enable LCD pins and Configure BackPlanes
    SLCD_EnablePins();

    LCD->GCR |= (1<<7); //LCD controller enable. 
    //LCD Driver Enable
    //Starts LCD controller waveform generator
    LCD->AR = LCD_AR_BRATE(0); //LCD controller blink rate = LCD clock /2^(12 + BRATE)
}

void SLCD_EnablePins(void)
{
    //bật Pen, BPen và WF8B cho các pin
	LCD->PEN[1] |= (1 << 5);        
	LCD->PEN[0] |= (1 << 17);       
	LCD->PEN[0] |= (1 << 7);        
	LCD->PEN[0] |= (1 << 8);        
	LCD->PEN[1] |= (1 << 21);       
	LCD->PEN[1] |= (1 << 6);       
	LCD->PEN[0] |= (1 << 10);       
	LCD->PEN[0] |= (1<< 11);       

	LCD->PEN[1] |= (1<< 8);        
	LCD->BPEN[1] |= (1<< 8);       
	LCD->WF8B[40] = (1 << 0);

	LCD->PEN[1] |= (1 << 20);       
	LCD->BPEN[1] |= (1 << 20);      
	LCD->WF8B[52] = (1 << 1);

	LCD->PEN[0] |= (1 << 19);       
	LCD->BPEN[0] |= (1 << 19);      
	LCD->WF8B[19] = (1 << 2);

	LCD->PEN[0] |= (1 << 18);       
	LCD->BPEN[0] |= (1 << 18);      
	LCD->WF8B[18] = (1 << 3);
}

void SLCD_WriteChar(unsigned char value) {
    // ASCII offset
    value -= '0';

    // Define segments for character '0' và '1'
    char LCD_Char_0[2] = {0x0B, 0x0E}; //  '0': SEGA, SEGB, SEGC, SEGD, SEGE, SEGF
    char LCD_Char_1[2] = {0x00, 0x06}; //  '1': SEGB, SEGC

    // ( SEGD+ SEGE+ SEGF+!SEGG) , ( SEGC+ SEGB+ SEGA) 
    // (!SEGD+!SEGE+!SEGF+!SEGG) , ( SEGC+ SEGB+!SEGA) 
    // Write segments for character '0' or '1' to the LCD positions
    char *segments = (value == 0) ? LCD_Char_0 : LCD_Char_1;

    for (int i = 0; i < 2; i++) {
        LCD->WF8B[Pin_Table[i]] = segments[i];
    }
}





