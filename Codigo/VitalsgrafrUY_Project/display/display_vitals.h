 /**
 * @file display_vitals.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este m�dulo se encarga de graficar las se�ales ECG, IBP, SPO2 y de mostrar los valores medios, m�nimos y m�ximos de la
 * frecuencia card�aca, del IBP y del SPO2.
 *
 *
 * @author Diego Alonso Germ�n Minetti Walter Barreiro
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
 * @brief Funci�n de inicializaci�n de variables
 */
void initvariables(void);
/**
 * @brief Funci�n de gr�fico de la se�al ECG
 */
void display_graph_ECG(void);
/**
 * @brief Funci�n de gr�fico de la se�al IBP
 */
void display_graph_IBP(void);
/**
 * @brief Funci�n de gr�fico de la se�al SPO2
 */
void display_graph_SPO2(void);
/**
 * @brief Funci�n que muestra el valor medio, m�nimo y m�ximo de la frecuncia card�aca
 */
void display_value_PR(void);
/**
 * @brief Funci�n que muestra el valor medio, m�nimo y m�ximo de IBP
 */
void display_value_IBP(void);
/**
 * @brief Funci�n que muestra el valor medio, m�nimo y m�ximo de SPO2
 */
void display_value_SPO2(void);

#endif /* DISPLAY_DISPLAY_VITALS_H_ */
