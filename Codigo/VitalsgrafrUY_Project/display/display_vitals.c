/**********************************************************
*
* @file display_vitals.c
*
* @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
* Este módulo se encarga de graficar las señales ECG, IBP, SPO2 y de mostrar los valores medios, mínimos y máximos de la
* frecuencia cardíaca, del IBP y del SPO2.
*
* @paragraph
* Grafica las señales ECG, IBP y SPO2 que están guardadas en memoria Flash en recepción.h
* La señal ECG es una señal real descargada Phisionet.org. Las otras dos son sinusoidal y triangular para IBP y SPO2 respectivamente a efectos de demostración.
* Los valores medios, mínimos y máximos de frecuencia cardíaca, IBP y SPO2 también están gaurdadas en el mismo header.
*
*
* @author  Diego Alonso Germán Minetti Walter Barreiro
* @version 2.0
* @date 5 Junio 2020
*
* display_vitals.c
* */


/*System Includes*/
#include <display_vitals.h>
#include "display_interfaz.h"
#include "procesamiento.h"
#include "utils.h"

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>



static uint32_t ndrawECG;
static uint32_t xdrawECG;
static uint32_t ydraw_actualECG;
static uint32_t ydraw_anteriorECG;
static uint32_t nremoveECG;
static uint32_t xremoveECG;
static uint32_t yremove_actualECG;
static uint32_t yremove_anteriorECG;

static uint32_t ndrawIBP;
static uint32_t xdrawIBP;
static uint32_t ydraw_actualIBP;
static uint32_t ydraw_anteriorIBP;
static uint32_t nremoveIBP;
static uint32_t xremoveIBP;
static uint32_t yremove_actualIBP;
static uint32_t yremove_anteriorIBP;

static uint32_t ndrawSPO2;
static uint32_t xdrawSPO2;
static uint32_t ydraw_actualSPO2;
static uint32_t ydraw_anteriorSPO2;
static uint32_t nremoveSPO2;
static uint32_t xremoveSPO2;
static uint32_t yremove_actualSPO2;
static uint32_t yremove_anteriorSPO2;


uint32_t cantdataECG;
uint32_t cantdataIBP;
uint32_t cantdataSPO2;

uint32_t medPR;
uint32_t minPR;
uint32_t maxPR;
uint32_t medIBP;
uint32_t minIBP;
uint32_t maxIBP;
uint32_t medSPO2;
uint32_t minSPO2;
uint32_t maxSPO2;

static int8_t Buffitoa[3];
static int8_t BuffauxPRmed[3];
static int8_t BuffauxPRmin[3];
static int8_t BuffauxPRmax[3];
static int8_t BuffauxIBPmed[3];
static int8_t BuffauxIBPmin[3];
static int8_t BuffauxIBPmax[3];
static int8_t BuffauxSPO2med[3];
static int8_t BuffauxSPO2min[3];
static int8_t BuffauxSPO2max[3];



void initvariables(void)
{
    datasaveECG = 0;
    dataremoveECG = 0;
    dataECG = 0;
    dataECGconv = 0;
    ndrawECG = 0;
    xdrawECG = 0;
    ydraw_actualECG = 0;
    ydraw_anteriorECG = 0;
    nremoveECG = 0;
    xremoveECG = 0;
    yremove_actualECG = 0;
    yremove_anteriorECG = 0;
    startremoveECG = false;
    datasavefinishECG = false;

    datasaveIBP = 0;
    dataremoveIBP = 0;
    dataIBP = 0;
    dataIBPconv = 0;
    ndrawIBP = 0;
    xdrawIBP = 0;
    ydraw_actualIBP = 0;
    ydraw_anteriorIBP = 0;
    nremoveIBP = 0;
    xremoveIBP = 0;
    yremove_actualIBP = 0;
    yremove_anteriorIBP = 0;
    startremoveIBP = false;
    datasavefinishIBP = false;

    datasaveSPO2 = 0;
    dataremoveSPO2 = 0;
    dataSPO2 = 0;
    dataSPO2conv = 0;
    ndrawSPO2 = 0;
    xdrawSPO2 = 0;
    ydraw_actualSPO2 = 0;
    ydraw_anteriorSPO2 = 0;
    nremoveSPO2 = 0;
    xremoveSPO2 = 0;
    yremove_actualSPO2 = 0;
    yremove_anteriorSPO2 = 0;
    startremoveSPO2 = false;
    datasavefinishSPO2 = false;

    medPR = 0;
    minPR = 0;
    maxPR = 0;
    medIBP = 0;
    minIBP = 0;
    maxIBP = 0;
    medSPO2 = 0;
    minSPO2 = 0;
    maxSPO2 = 0;
    cantdataECG = 0;
    cantdataIBP = 0;
    cantdataSPO2 = 0;
}
/*void LoadVData(void)
{

    int n;
    FILE *fp;

    fp = fopen("SamplesECGprueba.csv", "r");
    if(fp == NULL)
    {
        return;
    }else{}

    n = 0;
    while ((!feof(fp)) && (n < 3600))
        {
            fscanf(fp, "%d", &(dataECG[n]));
            n++;
        }
    fclose(fp);

}*/



void display_graph_ECG(void)

{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    ydraw_actualECG = 80 - (savedata260ECG[ndrawECG]);
    Graphics_drawPixel(&g_sContext,xdrawECG,ydraw_actualECG);
    if (xdrawECG > 0)
    {
        ydraw_anteriorECG = 80 - (savedata260ECG[ndrawECG-1]);
        Graphics_drawLine(&g_sContext, xdrawECG-1, ydraw_anteriorECG, xdrawECG, ydraw_actualECG);
    }
    xdrawECG++;//aumento x para graficar
    ndrawECG++;

    if (xdrawECG > 259)
    {
        xdrawECG = 0;
        ndrawECG = 0;
    }


       /*  antes de finalizar el grafico, guardo en removedata260ECG desde 241 al 260  */
        if (xdrawECG > 258)
        {
            xremoveECG = 0;
            nremoveECG = 0;
            startremoveECG = true;
            saveremovelastECG();
            dataremoveECG = 0;
        }

        if (startremoveECG)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            yremove_actualECG = 80 - (removedata260ECG[nremoveECG]);
            Graphics_drawPixel(&g_sContext,xremoveECG,yremove_actualECG);
            if (xremoveECG > 0)
            {
                yremove_anteriorECG = 80 - (removedata260ECG[nremoveECG-1]);
                Graphics_drawLine(&g_sContext, xremoveECG-1, yremove_anteriorECG, xremoveECG, yremove_actualECG);
            }
            xremoveECG++;
            nremoveECG++;
        }

    cantdataECG++;
     if (cantdataECG > 7)
    {
        receivearrayECG();
        cantdataECG = 0;
    }

}

void display_graph_IBP(void)

{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    ydraw_actualIBP = 142 - (savedata260IBP[ndrawIBP]);
    Graphics_drawPixel(&g_sContext,xdrawIBP,ydraw_actualIBP);
    if (xdrawIBP > 0)
    {
        ydraw_anteriorIBP = 142 - (savedata260IBP[ndrawIBP-1]);
        Graphics_drawLine(&g_sContext, xdrawIBP-1, ydraw_anteriorIBP, xdrawIBP, ydraw_actualIBP);
    }
    xdrawIBP++;//aumento x para graficar
    ndrawIBP++;

    if (xdrawIBP > 259)
    {
        xdrawIBP = 0;
        ndrawIBP = 0;
    }


       // antes de finalizar el grafico, guardo en removedata260ECG desde 241 al 260
        if (xdrawIBP > 258)
        {
            xremoveIBP = 0;
            nremoveIBP = 0;
            startremoveIBP = true;
            saveremovelastIBP();
            dataremoveIBP = 0;
        }

        if (startremoveIBP)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            yremove_actualIBP = 142 - (removedata260IBP[nremoveIBP]);
            Graphics_drawPixel(&g_sContext,xremoveIBP,yremove_actualIBP);
            if (xremoveIBP > 0)
            {
                yremove_anteriorIBP = 142 - (removedata260IBP[nremoveIBP-1]);
                Graphics_drawLine(&g_sContext, xremoveIBP-1, yremove_anteriorIBP, xremoveIBP, yremove_actualIBP);
            }
            xremoveIBP++;
            nremoveIBP++;
        }

    cantdataIBP++;
     if (cantdataIBP > 7)
    {
        receivearrayIBP();
        cantdataIBP = 0;
    }

}

void display_graph_SPO2(void)

{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    ydraw_actualSPO2 = 212 - (savedata260SPO2[ndrawSPO2]);
    Graphics_drawPixel(&g_sContext,xdrawSPO2,ydraw_actualSPO2);
    if (xdrawSPO2 > 0)
    {
        ydraw_anteriorSPO2 = 212 - (savedata260SPO2[ndrawSPO2-1]);
        Graphics_drawLine(&g_sContext, xdrawSPO2-1, ydraw_anteriorSPO2, xdrawSPO2, ydraw_actualSPO2);
    }
    xdrawSPO2++;//aumento x para graficar
    ndrawSPO2++;

    if (xdrawSPO2 > 259)
    {
        xdrawSPO2 = 0;
        ndrawSPO2 = 0;
    }


       // antes de finalizar el grafico, guardo en removedata260ECG desde 241 al 260
        if (xdrawSPO2 > 258)
        {
            xremoveSPO2 = 0;
            nremoveSPO2 = 0;
            startremoveSPO2 = true;
            saveremovelastSPO2();
            dataremoveSPO2 = 0;
        }

        if (startremoveSPO2)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            yremove_actualSPO2 = 212 - (removedata260SPO2[nremoveSPO2]);
            Graphics_drawPixel(&g_sContext,xremoveSPO2,yremove_actualSPO2);
            if (xremoveSPO2 > 0)
            {
                yremove_anteriorSPO2 = 212 - (removedata260SPO2[nremoveSPO2-1]);
                Graphics_drawLine(&g_sContext, xremoveSPO2-1, yremove_anteriorSPO2, xremoveSPO2, yremove_actualSPO2);
            }
            xremoveSPO2++;
            nremoveSPO2++;
        }

    cantdataSPO2++;
     if (cantdataSPO2 > 7)
    {
        receivearraySPO2();
        cantdataSPO2 = 0;
    }

}


void display_value_PR(void)
{
   int32_t value;
   // Valor medio PR
   Graphics_setFont(&g_sContext, &g_sFontCmss26b);
   value = savedataPRmed[medPR];
   checkPR_alarm(value);
   medPR++;
   itoa(value, Buffitoa);
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
      Graphics_drawString(&g_sContext, &BuffauxPRmed ,3, 275, 20, TRANSPARENT_TEXT);
      if (value >= 0 && value <= 9)
          {
              BuffauxPRmed[0] = ' ';
              BuffauxPRmed[1] = Buffitoa[0];
          } else if (value >= 10 && value <= 99)
          {
              BuffauxPRmed[0] = ' ';
              BuffauxPRmed[1] = Buffitoa[0];
              BuffauxPRmed[2] = Buffitoa[1];
          } else
              {
                  BuffauxPRmed[0] = Buffitoa[0];
                  BuffauxPRmed[1] = Buffitoa[1];

              }
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
      Graphics_drawString(&g_sContext, &BuffauxPRmed ,3, 275, 20, TRANSPARENT_TEXT);

      //Valor min PR
      Graphics_setFont(&g_sContext, &g_sFontCmss18b);
      value = savedataPRmin[minPR];
      minPR++;
      itoa(value, Buffitoa);
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
         Graphics_drawString(&g_sContext, &BuffauxPRmin ,3, 257, 50, TRANSPARENT_TEXT);
         if (value >= 0 && value <= 9)
             {
                 BuffauxPRmin[0] = ' ';
                 BuffauxPRmin[1] = Buffitoa[0];
             } else if (value >= 10 && value <= 99)
             {
                 BuffauxPRmin[0] = ' ';
                 BuffauxPRmin[1] = Buffitoa[0];
                 BuffauxPRmin[2] = Buffitoa[1];
             } else
                 {
                     BuffauxPRmin[0] = Buffitoa[0];
                     BuffauxPRmin[1] = Buffitoa[1];

                 }
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
         Graphics_drawString(&g_sContext, &BuffauxPRmin ,3, 257, 50, TRANSPARENT_TEXT);

         //Valor max PR
         Graphics_setFont(&g_sContext, &g_sFontCmss18b);
         value = savedataPRmax[maxPR];
         maxPR++;
         itoa(value, Buffitoa);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawString(&g_sContext, &BuffauxPRmax ,3, 295, 50, TRANSPARENT_TEXT);
            if (value >= 0 && value <= 9)
                {
                    BuffauxPRmax[0] = ' ';
                    BuffauxPRmax[1] = Buffitoa[0];
                } else if (value >= 10 && value <= 99)
                {
                    BuffauxPRmax[0] = ' ';
                    BuffauxPRmax[1] = Buffitoa[0];
                    BuffauxPRmax[2] = Buffitoa[1];
                } else
                    {
                        BuffauxPRmax[0] = Buffitoa[0];
                        BuffauxPRmax[1] = Buffitoa[1];

                    }
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
            Graphics_drawString(&g_sContext, &BuffauxPRmax ,3, 295, 50, TRANSPARENT_TEXT);
}

void display_value_IBP(void)
{
   int32_t value;
   //Valor medio IBP
   Graphics_setFont(&g_sContext, &g_sFontCmss26b);
   value = savedataIBPmed[medIBP];
   checkIBP_alarm(value);
   medIBP++;
   itoa(value, Buffitoa);
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
      Graphics_drawString(&g_sContext, &BuffauxIBPmed ,3, 275, 91, TRANSPARENT_TEXT);
      if (value >= 0 && value <= 9)
          {
              BuffauxIBPmed[0] = ' ';
              BuffauxIBPmed[1] = Buffitoa[0];
          } else if (value >= 10 && value <= 99)
          {
              BuffauxIBPmed[0] = ' ';
              BuffauxIBPmed[1] = Buffitoa[0];
              BuffauxIBPmed[2] = Buffitoa[1];
          } else
              {
                  BuffauxIBPmed[0] = Buffitoa[0];
                  BuffauxIBPmed[1] = Buffitoa[1];
                  BuffauxIBPmed[2] = Buffitoa[2];
              }
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
      Graphics_drawString(&g_sContext, &BuffauxIBPmed ,3, 275, 91, TRANSPARENT_TEXT);

      //Valor min IBP
      Graphics_setFont(&g_sContext, &g_sFontCmss18b);
      value = savedataIBPmin[minIBP];
      minIBP++;
      itoa(value, Buffitoa);
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
         Graphics_drawString(&g_sContext, &BuffauxIBPmin ,3, 257, 121, TRANSPARENT_TEXT);
         if (value >= 0 && value <= 9)
             {
                 BuffauxIBPmin[0] = ' ';
                 BuffauxIBPmin[1] = Buffitoa[0];
             } else if (value >= 10 && value <= 99)
             {
                 BuffauxIBPmin[0] = ' ';
                 BuffauxIBPmin[1] = Buffitoa[0];
                 BuffauxIBPmin[2] = Buffitoa[1];
             } else
                 {
                     BuffauxIBPmin[0] = Buffitoa[0];
                     BuffauxIBPmin[1] = Buffitoa[1];

                 }
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
         Graphics_drawString(&g_sContext, &BuffauxIBPmin ,3, 257, 121, TRANSPARENT_TEXT);

         //Valor max IBP
         Graphics_setFont(&g_sContext, &g_sFontCmss18b);
         value = savedataIBPmax[maxIBP];
         maxIBP++;
         itoa(value, Buffitoa);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawString(&g_sContext, &BuffauxIBPmax ,3, 295, 121, TRANSPARENT_TEXT);
            if (value >= 0 && value <= 9)
                {
                    BuffauxIBPmax[0] = ' ';
                    BuffauxIBPmax[1] = Buffitoa[0];
                } else if (value >= 10 && value <= 99)
                {
                    BuffauxIBPmax[0] = ' ';
                    BuffauxIBPmax[1] = Buffitoa[0];
                    BuffauxIBPmax[2] = Buffitoa[1];
                } else
                    {
                        BuffauxIBPmax[0] = Buffitoa[0];
                        BuffauxIBPmax[1] = Buffitoa[1];

                    }
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
            Graphics_drawString(&g_sContext, &BuffauxIBPmax ,3, 295, 121, TRANSPARENT_TEXT);
}

void display_value_SPO2(void)
{
   int32_t value;
   //Valor medio SPO2
   Graphics_setFont(&g_sContext, &g_sFontCmss26b);
   value = savedataSPO2med[medSPO2];
   checkSPO2_alarm(value);
   medSPO2++;
   itoa(value, Buffitoa);
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
      Graphics_drawString(&g_sContext, &BuffauxSPO2med ,3, 275, 162, TRANSPARENT_TEXT);
      if (value >= 0 && value <= 9)
          {
              BuffauxSPO2med[0] = ' ';
              BuffauxSPO2med[1] = Buffitoa[0];
          } else if (value >= 10 && value <= 99)
          {
              BuffauxSPO2med[0] = ' ';
              BuffauxSPO2med[1] = Buffitoa[0];
              BuffauxSPO2med[2] = Buffitoa[1];
          } else
              {
                  BuffauxSPO2med[0] = Buffitoa[0];
                  BuffauxSPO2med[1] = Buffitoa[1];

              }
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
      Graphics_drawString(&g_sContext, &BuffauxSPO2med ,3, 275, 162, TRANSPARENT_TEXT);

      //Valor min SPO2
      Graphics_setFont(&g_sContext, &g_sFontCmss18b);
      value = savedataSPO2min[minSPO2];
      minSPO2++;
      itoa(value, Buffitoa);
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
         Graphics_drawString(&g_sContext, &BuffauxSPO2min ,3, 257, 192, TRANSPARENT_TEXT);
         if (value >= 0 && value <= 9)
             {
                 BuffauxSPO2min[0] = ' ';
                 BuffauxSPO2min[1] = Buffitoa[0];
             } else if (value >= 10 && value <= 99)
             {
                 BuffauxSPO2min[0] = ' ';
                 BuffauxSPO2min[1] = Buffitoa[0];
                 BuffauxSPO2min[2] = Buffitoa[1];
             } else
                 {
                     BuffauxSPO2min[0] = Buffitoa[0];
                     BuffauxSPO2min[1] = Buffitoa[1];

                 }
         Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
         Graphics_drawString(&g_sContext, &BuffauxSPO2min ,3, 257, 192, TRANSPARENT_TEXT);

         //Valor max IBP
         Graphics_setFont(&g_sContext, &g_sFontCmss18b);
         value = savedataSPO2max[maxSPO2];
         maxSPO2++;
         itoa(value, Buffitoa);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawString(&g_sContext, &BuffauxSPO2max ,3, 295, 192, TRANSPARENT_TEXT);
            if (value >= 0 && value <= 9)
                {
                    BuffauxSPO2max[0] = ' ';
                    BuffauxSPO2max[1] = Buffitoa[0];
                } else if (value >= 10 && value <= 99)
                {
                    BuffauxSPO2max[0] = ' ';
                    BuffauxSPO2max[1] = Buffitoa[0];
                    BuffauxSPO2max[2] = Buffitoa[1];
                } else
                    {
                        BuffauxSPO2max[0] = Buffitoa[0];
                        BuffauxSPO2max[1] = Buffitoa[1];

                    }
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
            Graphics_drawString(&g_sContext, &BuffauxSPO2max ,3, 295, 192, TRANSPARENT_TEXT);
}



