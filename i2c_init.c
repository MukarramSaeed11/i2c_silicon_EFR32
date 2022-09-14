/*
 * i2c_init.c
 *
 *  Created on: 30-Aug-2022
 *      Author: Mukarram Saeed
 */


#include "i2c_init.h"


void i2c_init()
{
   I2CSPM_Init_TypeDef i2c_init;
   i2c_init.port=I2C0;
   i2c_init.sclPort=gpioPortD;
   i2c_init.sclPin=2;
   i2c_init.sdaPort=gpioPortD;
   i2c_init.sdaPin=3;
   i2c_init.i2cRefFreq=0;
   i2c_init.i2cMaxFreq=I2C_FREQ_STANDARD_MAX;
   i2c_init.i2cClhr=i2cClockHLRStandard;
   I2CSPM_Init (&i2c_init);
}
