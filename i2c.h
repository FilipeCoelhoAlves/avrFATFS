#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

/*
 * i2c.h
 *
 * Created: 21/10/2015 10:06:39
 * Author : Filipe
 */

#include "hal.h"
#define _BV(bit) (1 << (bit))

#ifndef SCLCLOCK
#define SCLCLOCK 400000L
#endif
#ifndef SCLPRESCALE
#define SCLPRESCALE 1
#endif
#define myTWBR ((F_CPU/SCLCLOCK)-16)/(2*SCLPRESCALE)


void    i2cConfigStart(void);
void    i2cWaitComplete(void);
void    i2cStartCondition(void);
void    i2cStopCondition(void);
uint8_t i2cReadACK(void);
uint8_t i2cReadNACK(void);
void    i2cSend(uint8_t data);

#endif // I2C_H_INCLUDED
