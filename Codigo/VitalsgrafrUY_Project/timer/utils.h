/**
 * @file funciones_auxiliares.h
 * @brief Funciones auxiliares para facilitar el parseo de datos
 *
 * @author  Javier Schandy <jschandy@fing.edu.uy>
 * @version 1.0
 * Implementacion inicial
 * @date 14 Feb 2019
 */

#ifndef FUNCIONES_AUXILIARES_H_INCLUDED
#define FUNCIONES_AUXILIARES_H_INCLUDED

#include "timer.h"

/**
 * Convierte un entero a ASCII
 * @param int value: valor entero a covertir
 * @param char* str: puntero a char donde guardar el resultado
 */
void itoa(int32_t value, int8_t * str);

#endif // FUNCIONES_AUXILIARES_H_INCLUDED
