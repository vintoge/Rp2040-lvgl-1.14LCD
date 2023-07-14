#include <ToyINA/ina226.h>

INA226_DEV ina226;

int INA226_init(INA226_DEV *dev, i2c_inst_t *i2c)
{
    printf("Initing...\n");

    /*
        Init dev handler
    */
    dev->i2c = i2c;
    dev->shunt_raw = 0;
    dev->bus_raw = 0;

    /*
        Reset device
    */
    uint8_t buff[2] = {0xc1, 0x27}; // D15 is the reset bit
    uint8_t reg = INA226_REG_CFG;
    i2c_write_blocking(i2c, INA226_ADDR_I2C, &reg, 1, true);
    i2c_write_blocking(i2c, INA226_ADDR_I2C, &buff[0], 2, false);

    /*
        Check state
    */
    reg = INA226_REG_CFG;
    i2c_write_blocking(i2c, INA226_ADDR_I2C, &reg, 1, true);
    i2c_read_blocking(i2c, INA226_ADDR_I2C, &buff[0], 2, false);
    if (buff[0] != 0x41 || buff[1] != 0x27)
    {
        printf("Init failed!\n");
        return -1;
    }

    reg = INA226_REG_VID1;
    i2c_write_blocking(i2c, INA226_ADDR_I2C, &reg, 1, true);
    i2c_read_blocking(i2c, INA226_ADDR_I2C, &buff[0], 2, false);
    if (buff[0] != 0x54 || buff[1] != 0x49)
    {
        printf("Init failed!\n");
        return -1;
    }

    reg = INA226_REG_VID2;
    i2c_write_blocking(i2c, INA226_ADDR_I2C, &reg, 1, true);
    i2c_read_blocking(i2c, INA226_ADDR_I2C, &buff[0], 2, false);
    if (buff[0] != 0x22 || buff[1] != 0x60)
    {
        printf("Init failed!\n");
        return -1;
    }

    printf("Init success!\n");
    return 1;
}


void INA226_get_shunt(INA226_DEV *dev)
{
    uint8_t reg = INA226_REG_SV;
    int16_t value = 0;
    uint8_t buff[2] = {0};

    i2c_write_blocking(dev->i2c, INA226_ADDR_I2C, &reg, 1, true);
    i2c_read_blocking(dev->i2c, INA226_ADDR_I2C, &buff[0], 2, true);
    value = buff[0];
    value = value << 8;
    value |= buff[1];
    
    dev->shunt_raw = value;
}


void INA226_get_bus(INA226_DEV *dev)
{
    uint8_t reg = INA226_REG_BV;
    uint16_t value = 0;
    uint8_t buff[2] = {0};

    i2c_write_blocking(dev->i2c, INA226_ADDR_I2C, &reg, 1, true);
    i2c_read_blocking(dev->i2c, INA226_ADDR_I2C, &buff[0], 2, true);
    value = buff[0];
    value = value << 8;
    value |= buff[1];
    
    dev->bus_raw = value;
}
