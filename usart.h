#ifndef USART_INCLUDED
#define USART_INCLUDED

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#include "hal.h"
#include <avr/io.h>
#include <util/setbaud.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>


uint8_t serialAvailable();
uint8_t serialRead();
uint8_t serialData();
uint8_t serialBufferSize();
void serialPurge();
void serialStart(void);
void serialSend(uint8_t data);
uint8_t serialReceive();
void serialString(const char stg[]);
void serialString_P(const char stg[]);
void serialStringLN(const char stg[]);
void serialSendInt(long int data,uint8_t base,uint8_t linefeed);
void serialSendFloat(float data,uint8_t linefeed);
void serialSendLongInt(long int data,uint8_t base,uint8_t linefeed);
void serialSendByte(uint8_t data,uint8_t base,uint8_t linefeed);
void serialSendBuffer(uint16_t buffersize);
void serialSendIp(uint8_t ip[4]);

void serialString_P(const char stg[]);
void serialStringLN_P(const char stg[]);



#endif
