#ifndef __INA226_H__
#define __INA226_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hardware/i2c.h"


/*
    Register map
*/
#define INA226_REG_CFG	 	0x00  // 0x4127  R/W-
#define INA226_REG_SV 		0x01  // 0x0000  R
#define INA226_REG_BV 		0x02  // 0x0000  R
#define INA226_REG_PWR 		0x03  // 0x0000  R
#define INA226_REG_CUR 		0x04  // 0x0000  R
#define INA226_REG_CAL 		0x05  // 0x0000  R/W-
#define INA226_REG_ALCFG 	0x06  // 0x0000  R/W-
#define INA226_REG_AL 		0x07  // 0x0000  R/W-
#define INA226_REG_VID1     0xFE  // 0x5449  R
#define INA226_REG_VID2     0xFF  // 0x2260  R


/*
    Address
*/
#define INA226_ADDR_I2C	        0x40  //
#define INA226_ADDR_WRITE	    (INA226_ADDR_I2C << 1) 
#define INA226_ADDR_READ	    (INA226_ADDR_I2C << 1) + 1
// #define INA226_GETALADDR	    0x14 
 

/*
    Configuration
*/



/*
    Device handler
*/
typedef struct __INA226_DEV
{
    i2c_inst_t *i2c;            // i2c handler
	int16_t shunt_raw; 
    uint16_t bus_raw;
} INA226_DEV;

extern INA226_DEV ina226;



/*
    Device init
*/
int INA226_init(INA226_DEV *dev, i2c_inst_t *i2c);
// enum pico_error_codes INA226_xxx();


/*
    Device data transfer
*/
void INA226_get_shunt(INA226_DEV *dev);
void INA226_get_bus(INA226_DEV *dev);



#endif
 

