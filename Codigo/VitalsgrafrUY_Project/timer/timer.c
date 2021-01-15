/**
 * @file timer.c
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de setear la hora que le ingresan, mandar la hora cuando la solicitan, incrementar 250 ms y de mostrar
 * la hora en pantalla.
 *
 * @paragraph
 * Las funciones son idénticas al laboratorio 1 y 2 de esta asignatura. Se agrega la función que muestra la hora en pantalla.
 *
 * @author Diego Alonso Germán Minetti Walter Barreiro
 * @version 1.3
 * @date 02 de Junio de 2020
 *
 * timer.c
 *
 */
/*System Include*/
#include <msp432.h>
#include "timer.h"

/*Utils Include*/
#include "utils.h"

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

/* GrLib Includes */
#include "grlib.h"
#include "button.h"
#include "touch_P401R.h"

static int8_t Buffitoa[2];
static int8_t Buffauxtime[8];

void set_time(time_t time)  //guardo en la variable actual el tiempo pasado como parámetro
{
    actual_time = time;
}


void get_time(int32_t * time_pointer)   // guardo en la dirección de memoria pasado como parámetro, el tiempo guardado en la variable actual
{
     *time_pointer = actual_time.hours;
      time_pointer++;
     *time_pointer = actual_time.minutes;
      time_pointer++;
     *time_pointer = actual_time.seconds;
      time_pointer++;
     *time_pointer = actual_time.miliseconds;
}

void display_time()
{
    Graphics_setFont(&g_sContext, &g_sFontCmss22);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &Buffauxtime,
                                        8, 100, 218, TRANSPARENT_TEXT);
    itoa(actual_time.hours, Buffitoa);
    if(actual_time.hours>=0 && actual_time.hours<=9)
    {
        Buffauxtime[0] = '0';
        Buffauxtime[1] = Buffitoa[0];
    }else
    {
        Buffauxtime[0] = Buffitoa[0];
        Buffauxtime[1] = Buffitoa[1];
    }
    Buffauxtime[2] = ':';
    itoa(actual_time.minutes, Buffitoa);
    if(actual_time.minutes>=0 && actual_time.minutes<=9)
    {
        Buffauxtime[3] = '0';
        Buffauxtime[4] = Buffitoa[0];
    }else
    {
        Buffauxtime[3] = Buffitoa[0];
        Buffauxtime[4] = Buffitoa[1];
    }
    Buffauxtime[5] = ':';
    itoa(actual_time.seconds, Buffitoa);
    if(actual_time.seconds>=0 && actual_time.seconds<=9)
    {
        Buffauxtime[6] = '0';
        Buffauxtime[7] = Buffitoa[0];
    }else
    {
        Buffauxtime[6] = Buffitoa[0];
        Buffauxtime[7] = Buffitoa[1];
    }

    Graphics_setFont(&g_sContext, &g_sFontCmss22);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext, &Buffauxtime,
                                    8, 100, 218, TRANSPARENT_TEXT);

}


void inc_time()         //incremento 250 milisegundos
{
    actual_time.miliseconds = actual_time.miliseconds + 250;
    if(actual_time.miliseconds >= 1000)
    {
        actual_time.miliseconds = actual_time.miliseconds-1000;
        actual_time.seconds++;
        if(actual_time.seconds == 60)
        {
            actual_time.seconds = 00;
            actual_time.minutes ++;
            if(actual_time.minutes == 60)
            {
                actual_time.minutes= 00;
                actual_time.hours++;
                if(actual_time.hours==24)
                {
                    actual_time.hours= 00;
                }
            }
        }
    }
}
