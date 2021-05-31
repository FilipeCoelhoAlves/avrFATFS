#ifndef HAL_H_INCLUDED
#define HAL_H_INCLUDED

#define F_CPU 16000000
#define BAUD  9600
#define SIZE 17
#define BUFFER_SIZE 50

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/setbaud.h>


#include "portManipulation.h"
#include "usart.h"


#endif // HAL_H_INCLUDED
