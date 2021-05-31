/*
 * i2c.h
 *
 * Created: 21/10/2015 10:06:39
 * Author : Filipe
 */

#include "i2c.h"

void i2cConfigStart(void)
{
    TWBR = myTWBR; /* 16MHz / (16+2*TWBR*1) = 100kHz */
    TWCR|=(1<<TWEN); /* enable */
}

void i2cWaitComplete(void)
{
    while (!(TWCR & (1<<TWINT)));
}

void i2cStartCondition(void)
{
    TWCR=0b10100100;
    i2cWaitComplete();
}

void i2cStopCondition(void)
{
    TWCR=0b10010100;
}

uint8_t i2cReadACK(void)
{
    TWCR=0b11000100;
    i2cWaitComplete();
    return TWDR;
}

uint8_t i2cReadNACK(void)
{
    TWCR=0b10000100;
    i2cWaitComplete();
    return TWDR;
}

void i2cSend(uint8_t data)
{
    TWDR = data;
    TWCR = (_BV(TWINT) | _BV(TWEN));
    i2cWaitComplete();
}
