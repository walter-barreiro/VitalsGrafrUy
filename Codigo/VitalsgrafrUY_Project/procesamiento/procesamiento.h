/**
 * @file procesamiento.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de procesar los datos para ser graficados y del chequeo de los valores para activación de alarma.
 *
 *
 * @author Diego Alonso Germán Minetti Walter Barreiro
 * @version 1.4
 * @date 05 de Junio de 2020
 *
 * procesamiento.h
 *
 */


#ifndef PROCESAMINETO_PROCESAMIENTO_H_
#define PROCESAMINETO_PROCESAMIENTO_H_
/* System Includes */
#include "display_interfaz.h"

/* DriverLib Includes */
#include "driverlib.h"

int32_t dataECGconv;
int32_t receivedataECG[8];
int32_t savedata260ECG[260];
int32_t removedata260ECG[260];
int32_t dataECGcopy [80];


uint32_t scaleECG;
uint32_t datasaveECG;
uint32_t dataremoveECG;
uint32_t dataECG;

int32_t dataIBPconv;
int32_t receivedataIBP[8];
int32_t savedata260IBP[260];
int32_t removedata260IBP[260];
int32_t dataIBPcopy [80];

uint32_t scaleIBP;
uint32_t datasaveIBP;
uint32_t dataremoveIBP;
uint32_t dataIBP;

int32_t dataSPO2conv;
int32_t receivedataSPO2[8];
int32_t savedata260SPO2[260];
int32_t removedata260SPO2[260];
int32_t dataSPO2copy [80];

uint32_t scaleSPO2;
uint32_t datasaveSPO2;
uint32_t dataremoveSPO2;
uint32_t dataSPO2;

int32_t savedataPRmed [16];
int32_t savedataPRmin [16];
int32_t savedataPRmax [16];
int32_t savedataIBPmed [16];
int32_t savedataIBPmin [16];
int32_t savedataIBPmax [16];
int32_t savedataSPO2med [16];
int32_t savedataSPO2min [16];
int32_t savedataSPO2max [16];


bool startremoveECG;
bool datasavefinishECG;

bool startremoveIBP;
bool datasavefinishIBP;

bool startremoveSPO2;
bool datasavefinishSPO2;

/**
 * @brief Función que ajusta los valores recibidos de ECG.
 */
void conversionECG(uint32_t data_conv);
/**
 * @brief Función que ajusta los valores de ECG a cantidad de pixels.
 */
void scale_adaptECG(void);
/**
 * @brief Función que va guardando los valores de ECG pregrabados en un array.
 */
void receivearrayECG(void);
/**
 * @brief Función que guarda en array de borrado del gráfico ECG, los últimos valores que se graficó.
 */
void saveremovelastECG(void);
/**
 * @brief Función que ajusta los valores recibidos de IBP.
 */
void conversionIBP(uint32_t data_conv);
/**
 * @brief Función que ajusta los valores de IBP a cantidad de pixels.
 */
void scale_adaptIBP(void);
/**
 * @brief Función que va guardando los valores de IBP pregrabados en un array.
 */
void receivearrayIBP(void);
/**
 * @brief Función que guarda en array de borrado del gráfico IBP, los últimos valores que se graficó.
 */
void saveremovelastIBP(void);
/**
 * @brief Función que ajusta los valores recibidos de SPO2.
 */
void conversionSPO2(uint32_t data_conv);
/**
 * @brief Función que ajusta los valores de SPO2 a cantidad de pixels.
 */
void scale_adaptSPO2(void);
/**
 * @brief Función que va guardando los valores de SPO2 pregrabados en un array.
 */
void receivearraySPO2(void);
/**
 * @brief Función que guarda en array de borrado del gráfico SPO2, los últimos valores que se graficó.
 */
void saveremovelastSPO2(void);
/**
 * @brief Función que indica posición de memoria del valor de alarma de ECG
 */
void SetAlarmECG(uint32_t * alarmECG_main);
/**
 * @brief Función que indica posición de memoria del valor de alarma de IBP
 */
void SetAlarmIBP(uint32_t * alarmIBP_main);
/**
 * @brief Función que indica posición de memoria del valor de alarma de SPO2
 */
void SetAlarmSPO2(uint32_t * alarmSPO2_main);
/**
 * @brief Función que indica posición de memoria de las banderas de alrma de ECG,IBP y SPO2
 */
void SetFlagAlarms (char* AlarmECGflag_main, char* AlarmIBPflag_main, char* AlarmSPO2flag_main);
/**
 * @brief Función que chequea si el valor de PR (frec. card.) pasado como parámetro está fuera de rango.
 */
void checkPR_alarm(int32_t value);
/**
 * @brief Función que chequea si el valor de IBP pasado como parámetro está fuera de rango
 */
void checkIBP_alarm(int32_t value);
/**
 * @brief Función que chequea si el valor de SPO2 pasado como parámetro está fuera de rango
 */
void checkSPO2_alarm(int32_t value);


#endif /* PROCESAMINETO_PROCESAMIENTO_H_ */
