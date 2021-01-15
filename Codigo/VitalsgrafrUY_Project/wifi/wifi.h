/**
 * @file wifi.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de inicializar el Boosterpack CC3100 Wifi para la comunicación con la interfaz gráfica.
 *
 * @author Diego Alonso German Minetti Walter Barreiro
 * @version 1.1
 * @date 29 de Mayo de 2020
 *
 * wifi.h
 *
 */


#ifndef WIFI_H
#define WIFI_H


#include "simplelink.h"

#define MAX_SEND_BUF    4000
#define MAX_RECV_BUF    4000

/**@brief Función que inicializa la comunicacion por Wifi. Setea la UM como station
* en modo cliente y abre el socket TCP. 
* @return Retorna SUCCESS en caso de conexión exitosa.
*/
_i16 InitWifi();

/**@brief Función que cierra el socket y termina la comunicacion con el CC3100.
* @brief En caso de error se devuelve un número negativo, en caso de éxito SUCCESS.
*/
_i32 WifiClose();

/** @brief Función que envia datos a travez del soket hacia el servidor.
* @param pDataS puntero al inicio del buffer que contiene los datos a enviar.
* @param Tamaño del buffer de datos a enviar.
* @return Retorna un numero negativo en caso de error o SUCCESS en caso contrario.
*/
_i32 SendDataWifi( _u8* pDataS, _i16 TamData);

/**@brief Función que recibe datos a travéz del socket.
* @param pDataR puntero al inicio del buffer que contiene los datos recibidos.
* @return Devuelve un número negativo en caso de error, o la cantidad de datos recibidos en exito.
*/
_i32 RecvDataWifi(uint32_t*  pDataR);




#endif
