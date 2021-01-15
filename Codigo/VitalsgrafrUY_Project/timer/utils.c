/*
 * funciones_auxiliares.c
 *
 *  Created on: Mar 21, 2019
 *      Author: jschandy
 */

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C":
 */

#include <utils.h>
#include "timer.h"


/* DriverLib Includes */
#include "driverlib.h"

#define BASE 10

void strreverse(int8_t* begin, int8_t* end) {

    char aux;
    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}

void itoa(int32_t value, int8_t * str) {
    static int8_t num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int8_t* wstr=str;
    int sign;
    // Take care of sign
    if ((sign=value) < 0) value = -value;
    // Conversion. Number is reversed.
    do *wstr++ = num[value%BASE]; while(value/=BASE);
    if(sign<0) *wstr++='-';
    //*wstr='\0';
    // Reverse string
    strreverse(str,wstr-1);
}
