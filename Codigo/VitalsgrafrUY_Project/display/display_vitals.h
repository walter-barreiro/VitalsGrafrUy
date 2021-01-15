 /**
 * @file display_vitals.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de graficar las señales ECG, IBP, SPO2 y de mostrar los valores medios, mínimos y máximos de la
 * frecuencia cardíaca, del IBP y del SPO2.
 *
 *
 * @author Diego Alonso Germán Minetti Walter Barreiro
 * @version 2.0
 * @date 5 de Junio de 2020
 *
 * display_vitals.h
 *
 */

#ifndef DISPLAY_DISPLAY_VITALS_H_
#define DISPLAY_DISPLAY_VITALS_H_

/* DriverLib Includes */
#include "driverlib.h"

/**
 * @brief Función de inicialización de variables
 */
void initvariables(void);
/**
 * @brief Función de gráfico de la señal ECG
 */
void display_graph_ECG(void);
/**
 * @brief Función de gráfico de la señal IBP
 */
void display_graph_IBP(void);
/**
 * @brief Función de gráfico de la señal SPO2
 */
void display_graph_SPO2(void);
/**
 * @brief Función que muestra el valor medio, mínimo y máximo de la frecuncia cardíaca
 */
void display_value_PR(void);
/**
 * @brief Función que muestra el valor medio, mínimo y máximo de IBP
 */
void display_value_IBP(void);
/**
 * @brief Función que muestra el valor medio, mínimo y máximo de SPO2
 */
void display_value_SPO2(void);

#endif /* DISPLAY_DISPLAY_VITALS_H_ */
