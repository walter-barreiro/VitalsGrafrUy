/**
 * @file timer.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este m�dulo se encarga de setear la hora que le ingresan, mandar la hora cuando la solicitan, incrementar 250 ms y de mostrar
 * la hora en pantalla.
 *
 *
 * @author Diego Alonso Germ�n Minetti Walter Barreiro
 * @version 1.3
 * @date 02 de Junio de 2020
 *
 * timer.h
 *
 */
#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "driverlib.h"
/**
 * @brief Estructura de tiempo
 */

typedef struct
    {
        int32_t hours;
        int32_t minutes;
        int32_t seconds;
        int32_t miliseconds;

    }time_t;

static time_t actual_time;
/**
 * @brief Funci�n que incrementa 250ms y actualiza segundos, minutos y horas si hay overflow.
 */

void inc_time();
/**
 * @brief Funci�n que guarda el tiempo actual.
 */

void get_time(int32_t * time_pointer);
/**
 * @brief Funci�n que setea el tiempo.
 */

void set_time(time_t time);
/**
 * @brief Funci�n que muestra el tiempo en display.
 */

void display_time();


#endif
