/**
 * @file display_interfaz.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de graficar las pantallas, actualizar los valores de alarmas y hora en pantalla de ajustes. Recepción de configuraciones desde
 * la interfaz gráfica.
 *
 * @author Diego Alonso Germán Minetti Walter Barreiro
 * @version 2.0
 * @date 30 de Mayo de 2020
 *
 * display_interfaz.h
 *
 */

#ifndef DISPLAY_DISPLAY_INTERFAZ_H_
#define DISPLAY_DISPLAY_INTERFAZ_H_


/* GrLib Includes */
#include "grlib.h"
#include "button.h"
#include "imageButton.h"
#include "radioButton.h"
#include "checkbox.h"
#include "kitronix320x240x16_ssd2119_spi.h"
#include "images.h"
#include "touch_P401R.h"



/*Touch screen context*/
touch_context g_sTouchContext;
/* Graphic library context*/
Graphics_Context g_sContext;
Graphics_Button setupButton;
Graphics_Button setButton;
Graphics_Button backButton;
Graphics_Button stopAlarmButton;
Graphics_Button stopAlarmButtonoff;
Graphics_Button startButton;

Graphics_Button loSpinnerECGButtonup;
Graphics_Button loSpinnerECGButtondown;
Graphics_Button hiSpinnerECGButtonup;
Graphics_Button hiSpinnerECGButtondown;

Graphics_Button loSpinnerIBPButtonup;
Graphics_Button loSpinnerIBPButtondown;
Graphics_Button hiSpinnerIBPButtonup;
Graphics_Button hiSpinnerIBPButtondown;

Graphics_Button loSpinnerSPO2Buttonup;
Graphics_Button loSpinnerSPO2Buttondown;
Graphics_Button hiSpinnerSPO2Buttonup;
Graphics_Button hiSpinnerSPO2Buttondown;

Graphics_Button SpinnerHourButtonup;
Graphics_Button SpinnerHourButtondown;
Graphics_Button SpinnerMinutesButtonup;
Graphics_Button SpinnerMinutesButtondown;

/**
 * @brief Estructura de la variable alarm_t
 */
typedef struct
    {
        int32_t lo;
        int32_t hi;
    }alarm_t;
/**
* @brief Estructura de la variable timedisplay_t
*/

typedef struct
        {
            int32_t hours;
            int32_t minutes;
        }timedisplay_t;

/**
* @brief Variable indica si está en pantalla principal
*/
volatile bool g_runMainMenu;

/**
* @brief Funcion de inicialización de botones del display
*/
void initializeButtons(void);

/**
* @brief Función del gráfico de la pantalla principal
*/

void drawMainMenu(void);

/**
* @brief Función del gráfico de la pantalla ajustes
*/
void drawSetupMenu(void);

/**
* @brief Función que recibe las configuraciones enviadas desde la interfaz
*/

int16_t SetInterfaz();

/**
* @brief Función que actualiza las alarmas y el tiempo luego de recibir las configuraciones
*/

void alarmsupdate(void);

/**
* @brief Función que muestra mensaje en display en caso de fuera de rango la frecuencia cardíaca
*/
void MessageAlarmECG();
/**
* @brief Función que muestra mensaje de alarma en display en caso de fuera de rango del valor IBP
*/

void MessageAlarmIBP();
/**
* @brief Función que muestra mensaje de alarma en display en caso de fuera de rango del valor SPO2
*/
void MessageAlarmSPO2();
/**
* @brief Función que borra del display los mensaje de alarmas
*/

void EraseMessageAlarm();
/**
* @brief Función que muestra en display, el valor bajo de activación de alarma de la frecuencia cardíaca
*/

void display_loAlarmECG(alarm_t value);
/**
* @brief Función que muestra en display, el valor alto de activación de alarma de la frecuencia cardíaca
*/
void display_hiAlarmECG(alarm_t value);
/**
* @brief Función que muestra en display, el valor bajo de activación de alarma del valor IBP
*/

void display_loAlarmIBP(alarm_t value);
/**
* @brief Función que muestra en display, el valor alto de activación de alarma del valor IBP
*/

void display_hiAlarmIBP(alarm_t value);
/**
* @brief Función que muestra en display, el valor bajo de activación de alarma del valor SPO2
*/
void display_loAlarmSPO2(alarm_t value);
/**
* @brief Función que muestra en display, el valor alto de activación de alarma del valor SPO2
*/
void display_hiAlarmSPO2(alarm_t value);
/**
* @brief Función que muestra en display, la hora actual.
*/
void display_hour(timedisplay_t value);
/**
* @brief Función que muestra en display, los minutos actuales.
*/
void display_minutes(timedisplay_t value);
/**
* @brief Enable the coprocessors used by the floating-point unit. Realizado por Texas Instruments.
*/
void boardInit();
/**
* @brief Función delay en milisegundos realizado por Texas Instruments y utilizada en el proyecto.
*/
void DelayLcd(uint16_t msec);

#endif /* DISPLAY_DISPLAY_INTERFAZ_H_ */
