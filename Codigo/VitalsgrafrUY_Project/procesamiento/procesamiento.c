/**
 * @file procesamiento.c
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de procesar los datos para ser graficados y del chequeo de los valores para activación de alarma .
 *
 * @paragraph Los datos pregrabados son copiados a un array de 8 bits, guardándolos en otro array en dos etapas para luego ser borrados
 * para graficar los nuevos datos. Los datos son ajustados a la escala de cantidad de pixels designada.
 * Chequea además si los valores medios de PR, IBP y SPO2 están fuera de rango para activación de alarma.
 *
 *
 * @author Diego Alonso Germán Minetti Walter Barreiro
 * @version 1.4
 * @date 05 de Junio de 2020
 *
 * procesamiento.c
 *
 */
/*System Include*/
#include "recepcion.h"
#include "procesamiento.h"
#include "wifi.h"
/* DriverLib Includes */
#include "driverlib.h"

#define cantpixelsgrafECG  80;
#define cantpixelsgrafIBP  60;
#define cantpixelsgrafSPO2 60;

#define offsetECG  100;
#define gananciaECG  200;

#define offsetIBP 160;
#define gananciaIBP 200;

#define offsetSPO2  160;
#define gananciaSPO2  200;

#define Recibido 100
#define No_Recibido -1

float rangeECG;
float rangeIBP;
float rangeSPO2;

uint32_t dataECG_min = 60;
uint32_t dataECG_max = 260;

uint32_t dataIBP_min = 0;
uint32_t dataIBP_max = 320;

uint32_t dataSPO2_min = 0;
uint32_t dataSPO2_max = 320;

alarm_t  alarmECGset;
alarm_t  alarmIBPset;
alarm_t  alarmSPO2set;
alarm_t  alarmECGdisplay;
alarm_t  alarmIBPdisplay;
alarm_t  alarmSPO2display;
static char* AlarmECGflag;
static char* AlarmIBPflag;
static char* AlarmSPO2flag;



void scale_adaptECG (void)
{
    float scaleaux;
    float rangeECG;
    rangeECG = dataECG_max - dataECG_min;
    scaleaux = rangeECG/cantpixelsgrafECG;
    scaleECG = scaleaux+0.5;

}
void scale_adaptIBP (void)
{
    float scaleaux;
    float rangeIBP;
    rangeIBP = dataIBP_max - dataIBP_min;
    scaleaux = rangeIBP/cantpixelsgrafIBP;
    scaleIBP = scaleaux+0.5;

}
void scale_adaptSPO2 (void)
{
    float scaleaux;
    float rangeSPO2;
    rangeSPO2 = dataSPO2_max - dataSPO2_min;
    scaleaux = rangeSPO2/cantpixelsgrafSPO2;
    scaleSPO2 = scaleaux+0.5;
}

void conversionECG(uint32_t data_conv)
{

  dataECGconv = dataECGrecibido[data_conv] * gananciaECG;
  dataECGconv = dataECGconv + offsetECG;
}

void conversionIBP(uint32_t data_conv)
{
  dataIBPconv = dataIBPrecibido[data_conv] * gananciaIBP;
  dataIBPconv = dataIBPconv + offsetIBP;
}

void conversionSPO2(uint32_t data_conv)
{
  dataSPO2conv = dataSPO2recibido[data_conv] * gananciaSPO2;
  dataSPO2conv = dataSPO2conv + offsetSPO2;
}

/*En receivedataECG guado 8 valores de la base de datos.
 * En savedataECG guardo 260 valores que se iran graficando
 * En removedataECG guardo 260 valores que se iran borrando
 * Empiezo a cargar 240 valores removedataECG 10 pixels antes de terminar
 *
 *
*/

void receivearrayECG(void)
{

    uint32_t h = 0;
    uint32_t k = 0;
    uint32_t i = 0;
    for (h = 0; h < 8; h++)
    {

        conversionECG(dataECG);    // en procesamiento.c
        receivedataECG[h] = dataECGconv;   // de la base de datos guardo en array de 8
        dataECG++;
        savedata260ECG[datasaveECG] = receivedataECG[h];   // guardo en savedata260ECG
        savedata260ECG[datasaveECG] = savedata260ECG[datasaveECG]/scaleECG;
        datasaveECG++;

        if ((datasaveECG > 250) && (!datasavefinishECG) )   // 10 pixels antes de terminar y no lo tengo guardado en removedataECG
           {
               for (k = 0; k < 241 ; k++)          //guardo 240 valores en removedata260ECG
               {
                   removedata260ECG[dataremoveECG] = savedata260ECG[k];
                   dataremoveECG++;
               }
               datasavefinishECG = true;    //finalizo guardado de 240 valores
           }

        if (datasaveECG > 259)       // si me paso de 250 guardo desde el principio
        {
            datasaveECG = 0;
            datasavefinishECG = false;
        }
    }
    for (i = 0; i < 16; i++)
    {
        savedataPRmed[i] = dataPRmedrecibido [i];
        savedataPRmin [i] = dataPRminrecibido [i];
        savedataPRmax [i] = dataPRmaxrecibido [i];
    }

}


void receivearrayIBP(void)
{

    uint32_t h = 0;
    uint32_t k = 0;
    uint32_t i = 0;
    for (h = 0; h < 8; h++)
    {

        conversionIBP(dataIBP);    // en procesamiento.c
        receivedataIBP[h] = dataIBPconv;   // de la base de datos guardo en array de 8
        dataIBP++;
        savedata260IBP[datasaveIBP] = receivedataIBP[h];   // guardo en savedata260ECG
        savedata260IBP[datasaveIBP] = savedata260IBP[datasaveIBP]/scaleIBP;
        datasaveIBP++;

        if ((datasaveIBP > 250) && (!datasavefinishIBP) )   // 10 pixels antes de terminar y no lo tengo guardado en removedataECG
           {
               for (k = 0; k < 241 ; k++)          //guardo 240 valores en removedata260ECG
               {
                   removedata260IBP[dataremoveIBP] = savedata260IBP[k];
                   dataremoveIBP++;
               }
               datasavefinishIBP = true;    //finalizo guardado de 240 valores
           }

        if (datasaveIBP > 259)       // si me paso de 250 guardo desde el principio
        {
            datasaveIBP = 0;
            datasavefinishIBP = false;
        }
    }
    for (i = 0; i < 16; i++)
    {
        savedataIBPmed[i] = dataIBPmedrecibido[i];
        savedataIBPmin[i] = dataIBPminrecibido[i];
        savedataIBPmax[i] = dataIBPmaxrecibido[i];
    }

}

void receivearraySPO2(void)
{

    uint32_t h = 0;
    uint32_t k = 0;
    uint32_t i = 0;
    for (h = 0; h < 8; h++)
    {

        conversionSPO2(dataSPO2);    // en procesamiento.c
        receivedataSPO2[h] = dataSPO2conv;   // de la base de datos guardo en array de 8
        dataSPO2++;
        savedata260SPO2[datasaveSPO2] = receivedataSPO2[h];   // guardo en savedata260SPO2
        savedata260SPO2[datasaveSPO2] = savedata260SPO2[datasaveSPO2]/scaleSPO2;
        datasaveSPO2++;

        if ((datasaveSPO2 > 250) && (!datasavefinishSPO2) )   // 10 pixels antes de terminar y no lo tengo guardado en removedataECG
           {
               for (k = 0; k < 241 ; k++)          //guardo 240 valores en removedata260SPO2
               {
                   removedata260SPO2[dataremoveSPO2] = savedata260SPO2[k];
                   dataremoveSPO2++;
               }
               datasavefinishSPO2 = true;    //finalizo guardado de 240 valores
           }

        if (datasaveSPO2 > 259)       // si me paso de 250 guardo desde el principio
        {
            datasaveSPO2 = 0;
            datasavefinishSPO2 = false;
        }
    }
    for (i = 0; i < 16; i++)
    {
        savedataSPO2med[i] = dataSPO2medrecibido[i];
        savedataSPO2min[i] = dataSPO2minrecibido[i];
        savedataSPO2max[i] = dataSPO2maxrecibido[i];
    }

}

void saveremovelastECG(void)
{
    uint32_t k = 0;
    for (k = 241; k < 260 ; k++)
    {
        removedata260ECG[dataremoveECG] = savedata260ECG[k];
        dataremoveECG++;
    }
}

void saveremovelastIBP(void)
{
    uint32_t k = 0;
    for (k = 241; k < 260 ; k++)
    {
        removedata260IBP[dataremoveIBP] = savedata260IBP[k];
        dataremoveIBP++;
    }
}

void saveremovelastSPO2(void)
{
    uint32_t k = 0;
    for (k = 241; k < 260 ; k++)
    {
        removedata260SPO2[dataremoveSPO2] = savedata260SPO2[k];
        dataremoveSPO2++;
    }
}


void SetFlagAlarms (char* AlarmECGflag_main, char* AlarmIBPflag_main, char* AlarmSPO2flag_main)
{
    AlarmECGflag = AlarmECGflag_main;
    AlarmIBPflag = AlarmIBPflag_main;
    AlarmSPO2flag = AlarmSPO2flag_main;
}

void SetAlarmECG(uint32_t * alarmECGset_pointer)
{
    alarmECGset.lo = *alarmECGset_pointer;
    alarmECGset_pointer++;
    alarmECGset.hi = *alarmECGset_pointer;
}
void SetAlarmIBP(uint32_t * alarmIBPset_pointer)
{
    alarmIBPset.lo = *alarmIBPset_pointer;
    alarmIBPset_pointer++;
    alarmIBPset.hi = *alarmIBPset_pointer;
}
void SetAlarmSPO2(uint32_t * alarmSPO2set_pointer)
{
    alarmSPO2set.lo = *alarmSPO2set_pointer;
    alarmSPO2set_pointer++;
    alarmSPO2set.hi = *alarmSPO2set_pointer;
}



void checkPR_alarm(int32_t value)
{
    if (/*value <= alarmECGset.lo ||*/ value >= alarmECGset.hi)
    *AlarmECGflag = 1;
}

void checkIBP_alarm(int32_t value)
{
    if (/*value <= alarmIBPset.lo ||*/ value >= alarmIBPset.hi )
    *AlarmIBPflag = 1;
}

void checkSPO2_alarm(int32_t value)
{
    if (/*value <= alarmSPO2set.lo ||*/ value >= alarmSPO2set.hi )
    *AlarmSPO2flag = 1;
}



