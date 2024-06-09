// LCD PIN1 to LCDWF0  Rev B
#define LCD_1a      37      // LCD Pin 5
#define LCD_1b      17      // LCD Pin 6
#define LCD_2a      7       // LCD Pin 7
#define LCD_2b      8       // LCD Pin 8
#define LCD_3a      53      // LCD Pin 9
#define LCD_3b      38      // LCD Pin 10
#define LCD_4a      10      // LCD Pin 11
#define LCD_4b      11      // LCD Pin 12
#define LCD_COM0    40      // LCD Pin 1
#define LCD_COM1    52      // LCD Pin 2
#define LCD_COM2    19      // LCD Pin 3
#define LCD_COM3    18      // LCD Pin 4

#define SEGDP       0x01
#define SEGC        0x02
#define SEGB        0x04
#define SEGA        0x08
                    
#define SEGD        0x01
#define SEGE        0x02
#define SEGG        0x04
#define SEGF        0x08

#define _CHAR_SIZE      (2)         // Used only when Dot Matrix is used
#define _LCDTYPE        (2) 


void SLCD_Init(void);
void SLCD_EnablePins(void);
void SLCD_WriteMsg(unsigned char *lbpMessage);
void SLCD_WriteLCD_(unsigned char lbValue);

