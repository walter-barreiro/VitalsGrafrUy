/**
 * @file wifi.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este m�dulo se encarga de inicializar el Boosterpack CC3100 Wifi para la comunicaci�n con la interfaz gr�fica.
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

/**@brief Funci�n que inicializa la comunicacion por Wifi. Setea la UM como station
* en modo cliente y abre el socket TCP. 
* @return Retorna SUCCESS en caso de conexi�n exitosa.
*/
_i16 InitWifi();

/**@brief Funci�n que cierra el socket y termina la comunicacion con el CC3100.
* @brief En caso de error se devuelve un n�mero negativo, en caso de �xito SUCCESS.
*/
_i32 WifiClose();

/** @brief Funci�n que envia datos a travez del soket hacia el servidor.
* @param pDataS puntero al inicio del buffer que contiene los datos a enviar.
* @param Tama�o del buffer de datos a enviar.
* @return Retorna un numero negativo en caso de error o SUCCESS en caso contrario.
*/
_i32 SendDataWifi( _u8* pDataS, _i16 TamData);

/**@brief Funci�n que recibe datos a trav�z del socket.
* @param pDataR puntero al inicio del buffer que contiene los datos recibidos.
* @return Devuelve un n�mero negativo en caso de error, o la cantidad de datos recibidos en exito.
*/
_i32 RecvDataWifi(uint32_t*  pDataR);




#endif
