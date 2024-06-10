#include "MKL46Z4.h"
#include "i2c.h"
#include "accel.h"
#include "delay.h"
#include <math.h>

void MMA8451_Init(void) {
    I2C_WriteRegister(0x1D, 0x2A, 0x18); //Put device in Standby mode
    I2C_WriteRegister(0x1D, 0x15, 0xD8);
    I2C_WriteRegister(0x1D, 0x17, 0x20); // //Set Threshold for Motion Detection to 32 counts
    //2g/0.063g/count = 32counts
    I2C_WriteRegister(0x1D, 0x18, 0x0A); //100 ms debounce timing
    // Lấy mẫu
    // FF_MT_COUNT: 0x18
    // FF_MT_THS: 0x17
    // CTRL_REG1: 0x2A
    I2C_WriteRegister(0x1D, 0x2A, 0x01); //Put device in active mode, ODR = 800 Hz
}

Value MMA8451_Read(void) {
    Value data;
	
    uint8_t X_MSB = I2C_ReadRegister(0x1D, OUT_X_MSB);
	delay(1);
    uint8_t X_LSB = I2C_ReadRegister(0x1D, OUT_X_LSB);
	delay(1);
    uint8_t Y_MSB = I2C_ReadRegister(0x1D, OUT_Y_MSB);
	delay(1);
    uint8_t Y_LSB = I2C_ReadRegister(0x1D, OUT_Y_LSB);
	delay(1);
    uint8_t Z_MSB = I2C_ReadRegister(0x1D, OUT_Z_MSB);
	delay(1);
    uint8_t Z_LSB = I2C_ReadRegister(0x1D, OUT_Z_LSB);
    delay(1);
	
    int16_t X = ((int16_t)(X_MSB << 8 | X_LSB)) >> 2;
    int16_t Y = ((int16_t)(Y_MSB << 8 | Y_LSB)) >> 2;
    int16_t Z = ((int16_t)(Z_MSB << 8 | Z_LSB)) >> 2;
	
    //Output buffer data format full scale. Default value: 00 (2g)
    //Mỗi đơn vị đếm từ sensor tương ứng với 0.25 mg gia tốc
    //Sensitivity: 4096 counts/g
    //2^14 = 16384 -> 2g: -8192 to 8191
	data.x = (float)X / 8192 * 2;
    data.y = (float)Y / 8192 * 2;
	data.z = (float)Z / 8192 * 2;
	
    return data;
}

float AccelValue(Value data) {
	return sqrt(data.x * data.x + data.y * data.y + data.z * data.z);
}
// Calibration from 2g Mode
// If the calibration is done reading in 2g mode (0.25 mg/count) 
// then every value will be divided by 8 when storing it in the calibration registers.
// One (1) count in the calibration register corresponds to 8 counts from the 
// output data. For example if the offset result is -12 counts then the value 
// would be 12/8 = 1.5 counts. The value to be stored is therefore +1 count 
// or +2 counts to shift the offset to zero. This is 1 or 2 as a 2’s complement number
//  stored in the calibration register.
//--------------------------------------------------------------------------------------------
//Calibration from 2g Mode
// Step1:
    //  Calculate 2’s complement for XYZ.
    // X-axis example: OUT_X_MSB = 0xFF = -1
    // OUT_X_LSB = 0x44 = 68
    // -1(256) + 68 = -188
    // 2’s complement X value = -188

// Step2:Calculate offset correction value for each axis.
// Take value read on each axis, divide by 8 (2g mode)
// and calculate inverse.
// X-axis @ 0g offset correction example: x = -188
// x = (-188/8) x -1 = 23 = 0x17
// Z-axis @ 1g offset correction example: z = 3988
// z = (4096-3988/8) x - 1 = 14 = 0x0E

//Step3:
// Put device in Standby mode
// CTRL_REG1 (0x2A) = 0x10

//Step4:
// Write offset correction values to XYZ:
// OFF_X (0x0F) // 23
// OFF_Y (0x30) // 0
// OFF_Z (0x031) // 14

//Step5:
//Read XYZ Values to confirm offset correction:
// OUT_X_MSB (0x01)     
// OUT_X_LSB (0x02)
// OUT_Y_MSB (0x03)
// OUT_Y_LSB (0x04)
// OUT_Z_MSB (0x05)
// OUT_Z_LSB (0x06)
