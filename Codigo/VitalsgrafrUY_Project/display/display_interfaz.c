/**********************************************************
*
* @file display_interfaz.c
*
* @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
* Este módulo se encarga de graficar las pantallas, actualizar los valores de alarmas y hora en pantalla de ajustes. Recepción de configuraciones desde
* la interfaz gráfica.
*
* @paragraph Grafica las pantallas principal y al de ajustes. Inicializa los botones, recibe y actualiza los valores de configuración de alarmas y hora desde la interfaz gráfica.
* En pantalla de ajustes muestra valores de alarma y hora para realizar cambios y actualización.
* Muestra los mensajes de alarma y los borra.
*
* @author  Diego Alonso Germán Minetti Walter Barreiro
* @version 2.0
* @date 30 Mayo 2020
*
* display_interfaz.c
* */
/*System Includes*/
#include <msp432.h>
#include <display_interfaz.h>
#include "timer.h"
#include "procesamiento.h"
#include "utils.h"
#include "wifi.h"
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>


bool button_selectedset = false;
bool button_selectedback = false;

bool button_selectedloSpinnerECGup = false;
bool button_selectedloSpinnerECGdown = false;
bool button_selectedloSpinnerIBPup = false;
bool button_selectedloSpinnerIBPdown = false;
bool button_selectedloSpinnerSPO2up = false;
bool button_selectedloSpinnerSPO2down = false;

bool button_selectedhiSpinnerECGup = false;
bool button_selectedhiSpinnerECGdown = false;
bool button_selectedhiSpinnerIBPup = false;
bool button_selectedhiSpinnerIBPdown = false;
bool button_selectedhiSpinnerSPO2up = false;
bool button_selectedhiSpinnerSPO2down = false;

bool button_selectedSpinnerHourup = false;
bool button_selectedSpinnerHourdown = false;
bool button_selectedSpinnerMinutesup = false;
bool button_selectedSpinnerMinutesdown = false;

const uint32_t loAlarmECG_min = 15;
const uint32_t loAlarmECG_max = 70;
const uint32_t hiAlarmECG_min = 90;
const uint32_t hiAlarmECG_max = 350;
const uint32_t loAlarmIBP_min = 0;
const uint32_t loAlarmIBP_max = 95;
const uint32_t hiAlarmIBP_min = 100;
const uint32_t hiAlarmIBP_max = 300;
const uint32_t loAlarmSPO2_min = 0;
const uint32_t loAlarmSPO2_max = 93;
const uint32_t hiAlarmSPO2_min = 95;
const uint32_t hiAlarmSPO2_max = 130;
const uint32_t hour_max = 23;
const uint32_t hour_min = 00;
const uint32_t minutes_max = 59;
const uint32_t minutes_min = 00;

alarm_t AlarmECG_display = {60,120};
alarm_t AlarmIBP_display = {90,110};
alarm_t AlarmSPO2_display = {90,100};
timedisplay_t display = {10,53};


alarm_t AlarmECG_set;
alarm_t AlarmIBP_set;
alarm_t AlarmSPO2_set;


volatile time_t time_set;

static int8_t Buffitoa[3];
static int8_t BuffauxloECG[3];
static int8_t BuffauxhiECG[3];
static int8_t BuffauxloIBP[3];
static int8_t BuffauxhiIBP[3];
static int8_t BuffauxloSPO2[3];
static int8_t BuffauxhiSPO2[3];
static int8_t Buffauxhour[2];
static int8_t Buffauxminutes[2];

static unsigned char BufferTx [MAX_SEND_BUF];
static uint32_t BufferRx [MAX_RECV_BUF];


void drawMainMenu(void)
{

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_clearDisplay(&g_sContext);
    g_runMainMenu = true;
    //240-213= 27 pixels banda horizontal. 320-260=60 pixels banda vertical
    Graphics_drawLine(&g_sContext, 260, 0, 260, 213); // vertical
    Graphics_drawLine(&g_sContext, 260, 71, 320, 71);
    Graphics_drawLine(&g_sContext, 260, 142, 320, 142);
    Graphics_drawLine(&g_sContext, 0, 213, 320, 213);  //horizontal

    Graphics_setFont(&g_sContext, &g_sFontCmss12);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    Graphics_drawString(&g_sContext, "ECG HR/PR",9, 263, 0, TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawString(&g_sContext, "IBP mmHg", 8, 263, 72, TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_drawString(&g_sContext, "SPO2 %", 6, 263, 143, TRANSPARENT_TEXT);

    Graphics_drawButton(&g_sContext, &setupButton);
    Graphics_drawButton(&g_sContext, &startButton);

    //inicializo alarmas que aparecen en pantalla Setup
    AlarmECG_set = AlarmECG_display;
    AlarmIBP_set = AlarmIBP_display;
    AlarmSPO2_set = AlarmSPO2_display;
    SetAlarmECG(&AlarmECG_set);
    SetAlarmIBP(&AlarmIBP_set);
    SetAlarmSPO2(&AlarmSPO2_set);


}

void drawSetupMenu(void)
{

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_clearDisplay(&g_sContext);
    g_runMainMenu = false;

    Graphics_setFont(&g_sContext, &g_sFontCmss26b);

    Graphics_drawString(&g_sContext, "Alarms", 6, 50 , 0, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Time", 4, 230 , 0, TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss14);
    Graphics_drawString(&g_sContext, "ECG", 3, 3 , 50, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "HR/PR", 5, 3 , 64, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "IBP", 3, 3 , 120, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "mmHg", 4, 3 , 134, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "SPO2", 4, 3 , 190, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "%", 1, 3 , 204, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "HOUR", 4, 245 , 27, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "MINUTES", 7, 233 , 88, TRANSPARENT_TEXT);


    Graphics_setFont(&g_sContext, &g_sFontCmss12);
    Graphics_drawString(&g_sContext, "Low", 3, 70 , 27, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Low", 3, 70 , 95, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Low", 3, 70 , 163, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "High", 4, 150 , 27, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "High", 4, 150 , 95, TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "High", 4, 150 , 163, TRANSPARENT_TEXT);

    Graphics_setFont(&g_sContext, &g_sFontCmss14);

    //Buttons Set and Back
    Graphics_drawButton(&g_sContext, &setButton);
    Graphics_drawButton(&g_sContext, &backButton);
    // Spinner Min
    Graphics_drawButton(&g_sContext, &loSpinnerECGButtonup);
    Graphics_drawButton(&g_sContext, &loSpinnerECGButtondown);
    Graphics_drawButton(&g_sContext, &loSpinnerIBPButtonup);
    Graphics_drawButton(&g_sContext, &loSpinnerIBPButtondown);
    Graphics_drawButton(&g_sContext, &loSpinnerSPO2Buttonup);
    Graphics_drawButton(&g_sContext, &loSpinnerSPO2Buttondown);
    //Spinner Max
    Graphics_drawButton(&g_sContext, &hiSpinnerECGButtonup);
    Graphics_drawButton(&g_sContext, &hiSpinnerECGButtondown);
    Graphics_drawButton(&g_sContext, &hiSpinnerIBPButtonup);
    Graphics_drawButton(&g_sContext, &hiSpinnerIBPButtondown);
    Graphics_drawButton(&g_sContext, &hiSpinnerSPO2Buttonup);
    Graphics_drawButton(&g_sContext, &hiSpinnerSPO2Buttondown);
    //Spinner Time
    Graphics_drawButton(&g_sContext, &SpinnerHourButtonup);
    Graphics_drawButton(&g_sContext, &SpinnerHourButtondown);
    Graphics_drawButton(&g_sContext, &SpinnerMinutesButtonup);
    Graphics_drawButton(&g_sContext, &SpinnerMinutesButtondown);

    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    display_loAlarmECG(AlarmECG_display);
    display_hiAlarmECG(AlarmECG_display);
    display_loAlarmIBP(AlarmIBP_display);
    display_hiAlarmIBP(AlarmIBP_display);
    display_loAlarmSPO2(AlarmSPO2_display);
    display_hiAlarmSPO2(AlarmSPO2_display);
    display_hour(display);
    display_minutes(display);


    do
    {
        touch_updateCurrentTouch(&g_sTouchContext);
        /*setbutton*/
        if(Graphics_isButtonSelected(&setButton, g_sTouchContext.x, g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &setButton);
            button_selectedset = true;

        }else
            {
                if (button_selectedset)
                {
                    button_selectedset = false;
                    Graphics_drawReleasedButton(&g_sContext, &setButton);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
                    Graphics_drawString(&g_sContext, "Set success", 11, 210 , 158, TRANSPARENT_TEXT);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
                    time_set.hours = display.hours;
                    time_set.minutes = display.minutes;
                    time_set.seconds = 00;
                    time_set.miliseconds = 000;
                    set_time(time_set);

                    AlarmECG_set = AlarmECG_display;
                    AlarmIBP_set = AlarmIBP_display;
                    AlarmSPO2_set = AlarmSPO2_display;

                    DelayLcd(1000);
                    Graphics_drawString(&g_sContext, "Set success", 11, 210 , 158, TRANSPARENT_TEXT);
                }
            }

        /*loSpinnerECGButtonup*/
        if(Graphics_isButtonSelected(&loSpinnerECGButtonup, g_sTouchContext.x,
                                                    g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &loSpinnerECGButtonup);
            button_selectedloSpinnerECGup = true;
            if(AlarmECG_display.lo < loAlarmECG_max)
            AlarmECG_display.lo++;
            display_loAlarmECG(AlarmECG_display);
        }else
             {
                if (button_selectedloSpinnerECGup)
                {

                    button_selectedloSpinnerECGup = false;
                    Graphics_drawReleasedButton(&g_sContext, &loSpinnerECGButtonup);
                }
             }
        /*loSpinnerECGButtondown*/
        if(Graphics_isButtonSelected(&loSpinnerECGButtondown, g_sTouchContext.x,
                                                        g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &loSpinnerECGButtondown);
            button_selectedloSpinnerECGdown = true;
            if(AlarmECG_display.lo > loAlarmECG_min)
            AlarmECG_display.lo--;
            display_loAlarmECG(AlarmECG_display);
        }else
            {
                if (button_selectedloSpinnerECGdown)
                {
                    button_selectedloSpinnerECGdown = false;
                    Graphics_drawReleasedButton(&g_sContext, &loSpinnerECGButtondown);
                }
            }
        /*hiSpinnerECGButtonup*/
        if(Graphics_isButtonSelected(&hiSpinnerECGButtonup, g_sTouchContext.x,
                                                            g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &hiSpinnerECGButtonup);
            button_selectedhiSpinnerECGup = true;
            if(AlarmECG_display.hi < hiAlarmECG_max)
            AlarmECG_display.hi++;
            display_hiAlarmECG(AlarmECG_display);
        }else
            {
                if (button_selectedhiSpinnerECGup)
                {
                    button_selectedhiSpinnerECGup = false;
                    Graphics_drawReleasedButton(&g_sContext, &hiSpinnerECGButtonup);
                }
            }
        /*hiSpinnerECGButtondown*/
        if(Graphics_isButtonSelected(&hiSpinnerECGButtondown, g_sTouchContext.x,
                                                                g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &hiSpinnerECGButtondown);
            button_selectedhiSpinnerECGdown = true;
            if(AlarmECG_display.hi > hiAlarmECG_min)
            AlarmECG_display.hi--;
            display_hiAlarmECG(AlarmECG_display);
        }else
            {
                if (button_selectedhiSpinnerECGdown)
                {
                    button_selectedhiSpinnerECGdown = false;
                    Graphics_drawReleasedButton(&g_sContext, &hiSpinnerECGButtondown);
                }
            }
        /*loSpinnerIBPButtonup*/
        if(Graphics_isButtonSelected(&loSpinnerIBPButtonup, g_sTouchContext.x,
                                                            g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &loSpinnerIBPButtonup);
            button_selectedloSpinnerIBPup = true;
            if(AlarmIBP_display.lo < loAlarmIBP_max)
            AlarmIBP_display.lo++;
            display_loAlarmIBP(AlarmIBP_display);
        }else
             {
                if (button_selectedloSpinnerIBPup)
                {
                    button_selectedloSpinnerIBPup = false;
                    Graphics_drawReleasedButton(&g_sContext, &loSpinnerIBPButtonup);
                }
             }
        /*loSpinnerIBPButtondown*/
        if(Graphics_isButtonSelected(&loSpinnerIBPButtondown, g_sTouchContext.x,
                                                            g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &loSpinnerIBPButtondown);
            button_selectedloSpinnerIBPdown = true;
            if(AlarmIBP_display.lo > loAlarmIBP_min)
            AlarmIBP_display.lo--;
            display_loAlarmIBP(AlarmIBP_display);
        }else
            {
                if (button_selectedloSpinnerIBPdown)
                {
                    button_selectedloSpinnerIBPdown = false;
                    Graphics_drawReleasedButton(&g_sContext, &loSpinnerIBPButtondown);
                }
            }
        /*hiSpinnerIBPButtonup*/
        if(Graphics_isButtonSelected(&hiSpinnerIBPButtonup, g_sTouchContext.x,
                                                                    g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &hiSpinnerIBPButtonup);
            button_selectedhiSpinnerIBPup = true;
            if(AlarmIBP_display.hi < hiAlarmIBP_max)
            AlarmIBP_display.hi++;
            display_hiAlarmIBP(AlarmIBP_display);
        }else
            {
                if (button_selectedhiSpinnerIBPup)
                {
                    button_selectedhiSpinnerIBPup = false;
                    Graphics_drawReleasedButton(&g_sContext, &hiSpinnerIBPButtonup);
                }
            }
        /*hiSpinnerIBPButtondown*/
        if(Graphics_isButtonSelected(&hiSpinnerIBPButtondown, g_sTouchContext.x,
                                                                    g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &hiSpinnerIBPButtondown);
            button_selectedhiSpinnerIBPdown = true;
            if(AlarmIBP_display.hi > hiAlarmIBP_min)
            AlarmIBP_display.hi--;
            display_hiAlarmIBP(AlarmIBP_display);
        }else
            {
                if (button_selectedhiSpinnerIBPdown)
                {
                    button_selectedhiSpinnerIBPdown = false;
                    Graphics_drawReleasedButton(&g_sContext, &hiSpinnerIBPButtondown);
                }
            }
        /*loSpinnerSPO2Buttonup*/
        if(Graphics_isButtonSelected(&loSpinnerSPO2Buttonup, g_sTouchContext.x,
                                                                  g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &loSpinnerSPO2Buttonup);
            button_selectedloSpinnerSPO2up = true;
            if(AlarmSPO2_display.lo < loAlarmSPO2_max)
            AlarmSPO2_display.lo++;
            display_loAlarmSPO2(AlarmSPO2_display);
        }else
            {
                if (button_selectedloSpinnerSPO2up)
                {
                    button_selectedloSpinnerSPO2up = false;
                    Graphics_drawReleasedButton(&g_sContext, &loSpinnerSPO2Buttonup);
                }
            }
        /*loSpinnerSPO2Buttondown*/
        if(Graphics_isButtonSelected(&loSpinnerSPO2Buttondown, g_sTouchContext.x,
                                                                  g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &loSpinnerSPO2Buttondown);
            button_selectedloSpinnerSPO2down = true;
            if(AlarmSPO2_display.lo > loAlarmSPO2_min)
            AlarmSPO2_display.lo--;
            display_loAlarmSPO2(AlarmSPO2_display);
        }else
            {
                if (button_selectedloSpinnerSPO2down)
                {
                    button_selectedloSpinnerSPO2down = false;
                    Graphics_drawReleasedButton(&g_sContext, &loSpinnerSPO2Buttondown);
                }
            }
        /*hiSpinnerSPO2Buttonup*/
        if(Graphics_isButtonSelected(&hiSpinnerSPO2Buttonup, g_sTouchContext.x,
                                                                          g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &hiSpinnerSPO2Buttonup);
            button_selectedhiSpinnerSPO2up = true;
            if(AlarmSPO2_display.hi < hiAlarmSPO2_max)
            AlarmSPO2_display.hi++;
            display_hiAlarmSPO2(AlarmSPO2_display);
        }else
            {
                if (button_selectedhiSpinnerSPO2up)
                {
                    button_selectedhiSpinnerSPO2up = false;
                    Graphics_drawReleasedButton(&g_sContext, &hiSpinnerSPO2Buttonup);
                }
            }
        /*hiSpinnerSPO2Buttondown*/
        if(Graphics_isButtonSelected(&hiSpinnerSPO2Buttondown, g_sTouchContext.x,
                                                                          g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &hiSpinnerSPO2Buttondown);
            button_selectedhiSpinnerSPO2down = true;
            if(AlarmSPO2_display.hi > hiAlarmSPO2_min)
            AlarmSPO2_display.hi--;
            display_hiAlarmSPO2(AlarmSPO2_display);
        }else
             {
                if (button_selectedhiSpinnerSPO2down)
                {
                    button_selectedhiSpinnerSPO2down = false;
                    Graphics_drawReleasedButton(&g_sContext, &hiSpinnerSPO2Buttondown);
                }
             }
        /*SpinnerHourButtonup*/
        if(Graphics_isButtonSelected(&SpinnerHourButtonup, g_sTouchContext.x,
                                                                 g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &SpinnerHourButtonup);
            button_selectedSpinnerHourup = true;
            if(display.hours < hour_max)
            display.hours++;
            display_hour(display);
        }else
            {
                if (button_selectedSpinnerHourup)
                {
                    button_selectedSpinnerHourup = false;
                    Graphics_drawReleasedButton(&g_sContext, &SpinnerHourButtonup);
                }
            }
              /*SpinnerHourButtondown*/
        if(Graphics_isButtonSelected(&SpinnerHourButtondown, g_sTouchContext.x,
                                                                   g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &SpinnerHourButtondown);
            button_selectedSpinnerHourdown = true;
            if(display.hours > hour_min)
            display.hours--;
            display_hour(display);
        }else
            {
                if (button_selectedSpinnerHourdown)
                {
                    button_selectedSpinnerHourdown = false;
                    Graphics_drawReleasedButton(&g_sContext, &SpinnerHourButtondown);
                }
            }
        /*SpinnerMinutesButtonup*/
        if(Graphics_isButtonSelected(&SpinnerMinutesButtonup, g_sTouchContext.x,
                                                                 g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &SpinnerMinutesButtonup);
            button_selectedSpinnerMinutesup = true;
            if(display.minutes < minutes_max)
            display.minutes++;
            display_minutes(display);
        }else
            {
                if (button_selectedSpinnerMinutesup)
                {
                    button_selectedSpinnerMinutesup = false;
                    Graphics_drawReleasedButton(&g_sContext, &SpinnerMinutesButtonup);
                }
            }
        /*SpinnerMinutesButtondown*/
        if(Graphics_isButtonSelected(&SpinnerMinutesButtondown, g_sTouchContext.x,
                                                                   g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &SpinnerMinutesButtondown);
            button_selectedSpinnerMinutesdown = true;
            if(display.minutes > minutes_min)
            display.minutes--;
            display_minutes(display);
        }else
             {
                 if (button_selectedSpinnerMinutesdown)
                 {
                     button_selectedSpinnerMinutesdown = false;
                     Graphics_drawReleasedButton(&g_sContext, &SpinnerMinutesButtondown);
                 }
             }



    }while(!Graphics_isButtonSelected(&backButton, g_sTouchContext.x, g_sTouchContext.y));
    /*backButton*/
    Graphics_drawSelectedButton(&g_sContext, &backButton);
    button_selectedback = false;
    Graphics_drawReleasedButton(&g_sContext, &backButton);
    drawMainMenu();
}

/* Función que recibe comandos de la interfaz y actualiza alarmas y hora. Función a utilizar para futura implementación de recepción de
*  configuraciones de alarma y tiempo ejecutándose a cada segundo.
*/
/*
int refreshCmds()
{
   //uint32_t * pAlarms[8];

   //getPointers(*pAlarms);
   int rfshStatus=0;
   int recvdConfigs=0;
   // Recibo respuesta de comando enviado en interrupción anterior
   if(Error_Transm)        //Chequeo una vez. Asumo que se mantiene
   {
       //Gestion de error de transmision
       rfshStatus=-1;
   }
   else
   {
       int tries=0;
       while(recvdConfigs<=4 && tries<1000)
       {
           RecvDataWifi(&BufferRx);
           switch (BufferRx[0])
           {
               case 'E':
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[0])=BufferRx[0];
                   AlarmECG_display.lo=BufferRx[0];
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[1])=BufferRx[0];
                   AlarmECG_display.hi=BufferRx[0];
                   recvdConfigs++;
               break;
               case 'I':
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[2])=BufferRx[0];
                   AlarmIBP_display.lo=BufferRx[0];
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[3])=BufferRx[0];
                   AlarmIBP_display.hi=BufferRx[0];
                   recvdConfigs++;
               break;
               case 'S':
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[4])=BufferRx[0];
                   AlarmSPO2_display.lo=BufferRx[0];
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[5])=BufferRx[0];
                   AlarmSPO2_display.hi=BufferRx[0];
                   recvdConfigs++;
               break;
               case 'R':
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[6])=BufferRx[0];
                   display.hours=BufferRx[0];
                   RecvDataWifi(&BufferRx);
                   //*(pAlarms[7])=BufferRx[0];
                   display.minutes=BufferRx[0];
                   recvdConfigs++;
               break;
               default:
                   //error de transmisión
                   rfshStatus=-1;
           }   //switch
           tries++;
       }   //while
   }   //else

   if (recvdConfigs==4)
       rfshStatus=0;
   else rfshStatus=-1;

   BufferTx[0] = 'S';          //Envio comando para recibir en
   SendDataWifi(&BufferTx,1);   //próxima interrupción

   return rfshStatus;
}

*/

//Función que se ejecuta al inicio del programa y recibe las configuraciones de alarma y tiempo.
int16_t SetInterfaz()
{

    int16_t Status = 100;
    BufferTx[0] = 'S';  //envio S , se apreto set

    SendDataWifi(&BufferTx[0],1);
    Delay (100);

    RecvDataWifi(&BufferRx);

    if(BufferRx[0] != 0 )
    {
        BufferTx[0] = 'F';
        CLI_Write((_u8 *) "Configuracion recibida \n\r");
        SendDataWifi(&BufferTx[0],1);
        alarmsupdate();

    } else {
            CLI_Write((_u8 *) "Configuracion NO recibida \n\r");
            Status = -1;
           }

 return Status;
}

// actualización de alarmas y tiempo
void alarmsupdate(void)
{
    AlarmECG_display.lo = BufferRx[0];
    display_loAlarmECG(AlarmECG_display);
    AlarmECG_display.hi = BufferRx[1];
    display_hiAlarmECG(AlarmECG_display);
    AlarmIBP_display.lo = BufferRx[2];
    display_loAlarmIBP(AlarmIBP_display);
    AlarmIBP_display.hi = BufferRx[3];
    display_hiAlarmIBP(AlarmIBP_display);
    AlarmSPO2_display.lo = BufferRx[4];
    display_loAlarmSPO2(AlarmSPO2_display);
    AlarmSPO2_display.hi = BufferRx[5];
    display_hiAlarmSPO2(AlarmSPO2_display);
    display.hours = BufferRx[6];
    display_hour(display);
    display.minutes = BufferRx[7];
    display_minutes(display);

    AlarmECG_set = AlarmECG_display;
    AlarmIBP_set = AlarmIBP_display;
    AlarmSPO2_set = AlarmSPO2_display;
    time_set.hours = display.hours;
    time_set.minutes = display.minutes;
    time_set.seconds = 00;
    time_set.miliseconds = 000;
    set_time(time_set);

}


void display_loAlarmECG(alarm_t value)
{
    itoa(value.lo, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &BuffauxloECG ,3, 65, 55, TRANSPARENT_TEXT);
    if (value.lo >= 0 && value.lo <= 9)
        {
            BuffauxloECG[0] = ' ';
            BuffauxloECG[1] = Buffitoa[0];
        } else
            {
                BuffauxloECG[0] = Buffitoa[0];
                BuffauxloECG[1] = Buffitoa[1];
            }
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &BuffauxloECG ,3, 65, 55, TRANSPARENT_TEXT);
}

void display_hiAlarmECG(alarm_t value)
{
    itoa(value.hi, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &BuffauxhiECG ,3, 144, 55, TRANSPARENT_TEXT);
    if (value.hi >= 10 && value.hi <= 99)
        {
            BuffauxhiECG[0] = ' ';
            BuffauxhiECG[1] = Buffitoa[0];
            BuffauxhiECG[2] = Buffitoa[1];
        } else
            {
                BuffauxhiECG[0] = Buffitoa[0];
                BuffauxhiECG[1] = Buffitoa[1];
                BuffauxhiECG[2] = Buffitoa[2];
            }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &BuffauxhiECG ,3, 144, 55, TRANSPARENT_TEXT);

}

void display_loAlarmIBP(alarm_t value)
{
    itoa(value.lo, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &BuffauxloIBP ,3, 65, 123, TRANSPARENT_TEXT);
    if (value.lo >= 0 && value.lo <= 9)
        {
            BuffauxloIBP[0] = ' ';
            BuffauxloIBP[1] = Buffitoa[0];
        } else
            {
                BuffauxloIBP[0] = Buffitoa[0];
                BuffauxloIBP[1] = Buffitoa[1];
            }


    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &BuffauxloIBP ,3, 65, 123, TRANSPARENT_TEXT);

}

void display_hiAlarmIBP(alarm_t value)
{
    itoa(value.hi, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &BuffauxhiIBP ,3, 144, 123, TRANSPARENT_TEXT);

        BuffauxhiIBP[0] = Buffitoa[0];
        BuffauxhiIBP[1] = Buffitoa[1];
        BuffauxhiIBP[2] = Buffitoa[2];
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &BuffauxhiIBP ,3, 144, 123, TRANSPARENT_TEXT);

}

void display_loAlarmSPO2(alarm_t value)
{
    itoa(value.lo, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &BuffauxloSPO2 ,3, 65, 191, TRANSPARENT_TEXT);
    if (value.lo >= 0 && value.lo <= 9)
        {
            BuffauxloSPO2[0] = ' ';
            BuffauxloSPO2[1] = Buffitoa[0];
        } else
            {
                BuffauxloSPO2[0] = Buffitoa[0];
                BuffauxloSPO2[1] = Buffitoa[1];
            }
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &BuffauxloSPO2 ,3, 65, 191, TRANSPARENT_TEXT);

}

void display_hiAlarmSPO2(alarm_t value)
{
    itoa(value.hi, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &BuffauxhiSPO2 ,3, 144, 191, TRANSPARENT_TEXT);
    if (value.hi >= 10 && value.hi <= 99)
        {
            BuffauxhiSPO2[0] = ' ';
            BuffauxhiSPO2[1] = Buffitoa[0];
            BuffauxhiSPO2[2] = Buffitoa[1];
        } else
            {
                BuffauxhiSPO2[0] = Buffitoa[0];
                BuffauxhiSPO2[1] = Buffitoa[1];
                BuffauxhiSPO2[2] = Buffitoa[2];
            }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &BuffauxhiSPO2 ,3, 144, 191, TRANSPARENT_TEXT);
}

void display_hour(timedisplay_t value)
{
    itoa(value.hours, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &Buffauxhour ,3, 260, 55, TRANSPARENT_TEXT);
    if (value.hours >= 0 && value.hours <= 9)
    {
        Buffauxhour[0] = '0';
        Buffauxhour[1] = Buffitoa[0];
    } else
        {
            Buffauxhour[0] = Buffitoa[0];
            Buffauxhour[1] = Buffitoa[1];
        }
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &Buffauxhour ,3, 260, 55, TRANSPARENT_TEXT);
}

void display_minutes(timedisplay_t value)
{
    itoa(value.minutes, Buffitoa);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GRAY);
    Graphics_drawString(&g_sContext, &Buffauxminutes ,3, 260, 123, TRANSPARENT_TEXT);
    if (value.minutes >= 0 && value.minutes <= 9)
        {
            Buffauxminutes[0] = '0';
            Buffauxminutes[1] = Buffitoa[0];
        } else
            {
                Buffauxminutes[0] = Buffitoa[0];
                Buffauxminutes[1] = Buffitoa[1];
            }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, &Buffauxminutes ,3, 260, 123, TRANSPARENT_TEXT);
}



void initializeButtons(void)
{
    //Initiliaze Setup Button
    setupButton.xMin = 250;
    setupButton.xMax = 320;
    setupButton.yMin = 215;
    setupButton.yMax = 240;
    setupButton.borderWidth = 1;
    setupButton.selected = false;
    setupButton.fillColor = GRAPHICS_COLOR_LIGHT_BLUE;
    setupButton.borderColor = GRAPHICS_COLOR_LIGHT_BLUE;
    setupButton.selectedColor = GRAPHICS_COLOR_BLACK;
    setupButton.textColor = GRAPHICS_COLOR_BLACK;
    setupButton.selectedTextColor = GRAPHICS_COLOR_LIGHT_BLUE;
    setupButton.textXPos = 258;
    setupButton.textYPos = 219;
    setupButton.text = "SETUP";
    setupButton.font = &g_sFontCmss18b;

    //Initiliaze Stop Alarm Button
    stopAlarmButton.xMin = 172;
    stopAlarmButton.xMax = 245;
    stopAlarmButton.yMin = 215;
    stopAlarmButton.yMax = 240;
    stopAlarmButton.borderWidth = 1;
    stopAlarmButton.selected = false;
    stopAlarmButton.fillColor = GRAPHICS_COLOR_RED;
    stopAlarmButton.borderColor = GRAPHICS_COLOR_RED;
    stopAlarmButton.selectedColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButton.textColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButton.selectedTextColor = GRAPHICS_COLOR_RED;
    stopAlarmButton.textXPos = 175;
    stopAlarmButton.textYPos = 219;
    stopAlarmButton.text = "stopALARM";
    stopAlarmButton.font = &g_sFontCmss16;

    //Initiliaze Stop Alarm Button
    stopAlarmButtonoff.xMin = 172;
    stopAlarmButtonoff.xMax = 245;
    stopAlarmButtonoff.yMin = 215;
    stopAlarmButtonoff.yMax = 240;
    stopAlarmButtonoff.borderWidth = 1;
    stopAlarmButtonoff.selected = false;
    stopAlarmButtonoff.fillColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButtonoff.borderColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButtonoff.selectedColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButtonoff.textColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButtonoff.selectedTextColor = GRAPHICS_COLOR_BLACK;
    stopAlarmButtonoff.textXPos = 175;
    stopAlarmButtonoff.textYPos = 219;
    stopAlarmButtonoff.text = "stopALARM";
    stopAlarmButtonoff.font = &g_sFontCmss16;


    //Initiliaze Start Button
    startButton.xMin = 0;
    startButton.xMax = 70;
    startButton.yMin = 215;
    startButton.yMax = 240;
    startButton.borderWidth = 1;
    startButton.selected = false;
    startButton.fillColor = GRAPHICS_COLOR_LIGHT_GREEN;
    startButton.borderColor = GRAPHICS_COLOR_LIGHT_GREEN;
    startButton.selectedColor = GRAPHICS_COLOR_BLACK;
    startButton.textColor = GRAPHICS_COLOR_BLACK;
    startButton.selectedTextColor = GRAPHICS_COLOR_LIGHT_GREEN;
    startButton.textXPos = 5;
    startButton.textYPos = 218;
    startButton.text = "START";
    startButton.font = &g_sFontCmss18b;

    //Initiliaze Set Button
    setButton.xMin = 250;
    setButton.xMax = 320;
    setButton.yMin = 179;
    setButton.yMax = 205;
    setButton.borderWidth = 1;
    setButton.selected = false;
    setButton.fillColor = GRAPHICS_COLOR_BLUE;
    setButton.borderColor = GRAPHICS_COLOR_BLUE;
    setButton.selectedColor = GRAPHICS_COLOR_BLACK;
    setButton.textColor = GRAPHICS_COLOR_BLACK;
    setButton.selectedTextColor = GRAPHICS_COLOR_BLUE;
    setButton.textXPos = 270;
    setButton.textYPos = 183;
    setButton.text = "SET";
    setButton.font = &g_sFontCmss18b;

    //Initiliaze Back Button
    backButton.xMin = 250;
    backButton.xMax = 320;
    backButton.yMin = 214;
    backButton.yMax = 240;
    backButton.borderWidth = 1;
    backButton.selected = false;
    backButton.fillColor = GRAPHICS_COLOR_BLUE;
    backButton.borderColor = GRAPHICS_COLOR_BLUE;
    backButton.selectedColor = GRAPHICS_COLOR_BLACK;
    backButton.textColor = GRAPHICS_COLOR_BLACK;
    backButton.selectedTextColor = GRAPHICS_COLOR_BLUE;
    backButton.textXPos = 260;
    backButton.textYPos = 218;
    backButton.text = "BACK";
    backButton.font = &g_sFontCmss18b;



    //Initialize loSpinnerECGup Button
    loSpinnerECGButtonup.xMin = 105;
    loSpinnerECGButtonup.xMax = 125;
    loSpinnerECGButtonup.yMin = 36;
    loSpinnerECGButtonup.yMax = 56;
    loSpinnerECGButtonup.borderWidth = 1;
    loSpinnerECGButtonup.selected = false;
    loSpinnerECGButtonup.fillColor = GRAPHICS_COLOR_GRAY;
    loSpinnerECGButtonup.borderColor = GRAPHICS_COLOR_GRAY;
    loSpinnerECGButtonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    loSpinnerECGButtonup.textColor = GRAPHICS_COLOR_WHITE;
    loSpinnerECGButtonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    loSpinnerECGButtonup.textXPos = 111;
    loSpinnerECGButtonup.textYPos = 39;
    loSpinnerECGButtonup.text = "+";
    loSpinnerECGButtonup.font = &g_sFontCm14;

    //Initialize loSpinnerECGdown Button
    loSpinnerECGButtondown.xMin = 105;
    loSpinnerECGButtondown.xMax = 125;
    loSpinnerECGButtondown.yMin = 61;
    loSpinnerECGButtondown.yMax = 81;
    loSpinnerECGButtondown.borderWidth = 1;
    loSpinnerECGButtondown.selected = false;
    loSpinnerECGButtondown.fillColor = GRAPHICS_COLOR_GRAY;
    loSpinnerECGButtondown.borderColor = GRAPHICS_COLOR_GRAY;
    loSpinnerECGButtondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    loSpinnerECGButtondown.textColor = GRAPHICS_COLOR_WHITE;
    loSpinnerECGButtondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    loSpinnerECGButtondown.textXPos = 111;
    loSpinnerECGButtondown.textYPos = 55;
    loSpinnerECGButtondown.text = "-";
    loSpinnerECGButtondown.font = &g_sFontCm30;

    //Initialize loSpinnerIBPup Button
    loSpinnerIBPButtonup.xMin = 105;
    loSpinnerIBPButtonup.xMax = 125;
    loSpinnerIBPButtonup.yMin = 105;
    loSpinnerIBPButtonup.yMax = 125;
    loSpinnerIBPButtonup.borderWidth = 1;
    loSpinnerIBPButtonup.selected = false;
    loSpinnerIBPButtonup.fillColor = GRAPHICS_COLOR_GRAY;
    loSpinnerIBPButtonup.borderColor = GRAPHICS_COLOR_GRAY;
    loSpinnerIBPButtonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    loSpinnerIBPButtonup.textColor = GRAPHICS_COLOR_WHITE;
    loSpinnerIBPButtonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    loSpinnerIBPButtonup.textXPos = 111;
    loSpinnerIBPButtonup.textYPos = 108;
    loSpinnerIBPButtonup.text = "+";
    loSpinnerIBPButtonup.font = &g_sFontCm14;

    //Initialize loSpinnerIBPdown Button
    loSpinnerIBPButtondown.xMin = 105;
    loSpinnerIBPButtondown.xMax = 125;
    loSpinnerIBPButtondown.yMin = 130;
    loSpinnerIBPButtondown.yMax = 150;
    loSpinnerIBPButtondown.borderWidth = 1;
    loSpinnerIBPButtondown.selected = false;
    loSpinnerIBPButtondown.fillColor = GRAPHICS_COLOR_GRAY;
    loSpinnerIBPButtondown.borderColor = GRAPHICS_COLOR_GRAY;
    loSpinnerIBPButtondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    loSpinnerIBPButtondown.textColor = GRAPHICS_COLOR_WHITE;
    loSpinnerIBPButtondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    loSpinnerIBPButtondown.textXPos = 111;
    loSpinnerIBPButtondown.textYPos = 124;
    loSpinnerIBPButtondown.text = "-";
    loSpinnerIBPButtondown.font = &g_sFontCm30;

    //Initialize loSpinnerSPO2up Button
    loSpinnerSPO2Buttonup.xMin = 105;
    loSpinnerSPO2Buttonup.xMax = 125;
    loSpinnerSPO2Buttonup.yMin = 174;
    loSpinnerSPO2Buttonup.yMax = 194;
    loSpinnerSPO2Buttonup.borderWidth = 1;
    loSpinnerSPO2Buttonup.selected = false;
    loSpinnerSPO2Buttonup.fillColor = GRAPHICS_COLOR_GRAY;
    loSpinnerSPO2Buttonup.borderColor = GRAPHICS_COLOR_GRAY;
    loSpinnerSPO2Buttonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    loSpinnerSPO2Buttonup.textColor = GRAPHICS_COLOR_WHITE;
    loSpinnerSPO2Buttonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    loSpinnerSPO2Buttonup.textXPos = 111;
    loSpinnerSPO2Buttonup.textYPos = 177;
    loSpinnerSPO2Buttonup.text = "+";
    loSpinnerSPO2Buttonup.font = &g_sFontCm14;

    //Initialize loSpinnerIBPdown Button
    loSpinnerSPO2Buttondown.xMin = 105;
    loSpinnerSPO2Buttondown.xMax = 125;
    loSpinnerSPO2Buttondown.yMin = 199;
    loSpinnerSPO2Buttondown.yMax = 219;
    loSpinnerSPO2Buttondown.borderWidth = 1;
    loSpinnerSPO2Buttondown.selected = false;
    loSpinnerSPO2Buttondown.fillColor = GRAPHICS_COLOR_GRAY;
    loSpinnerSPO2Buttondown.borderColor = GRAPHICS_COLOR_GRAY;
    loSpinnerSPO2Buttondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    loSpinnerSPO2Buttondown.textColor = GRAPHICS_COLOR_WHITE;
    loSpinnerSPO2Buttondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    loSpinnerSPO2Buttondown.textXPos = 111;
    loSpinnerSPO2Buttondown.textYPos = 193;
    loSpinnerSPO2Buttondown.text = "-";
    loSpinnerSPO2Buttondown.font = &g_sFontCm30;

    //Initialize hiSpinnerECGup Button
    hiSpinnerECGButtonup.xMin = 184;
    hiSpinnerECGButtonup.xMax = 204;
    hiSpinnerECGButtonup.yMin = 36;
    hiSpinnerECGButtonup.yMax = 56;
    hiSpinnerECGButtonup.borderWidth = 1;
    hiSpinnerECGButtonup.selected = false;
    hiSpinnerECGButtonup.fillColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerECGButtonup.borderColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerECGButtonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    hiSpinnerECGButtonup.textColor = GRAPHICS_COLOR_WHITE;
    hiSpinnerECGButtonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerECGButtonup.textXPos = 190;
    hiSpinnerECGButtonup.textYPos = 39;
    hiSpinnerECGButtonup.text = "+";
    hiSpinnerECGButtonup.font = &g_sFontCm14;

    //Initialize hiSpinnerECGdown Button
    hiSpinnerECGButtondown.xMin = 184;
    hiSpinnerECGButtondown.xMax = 204;
    hiSpinnerECGButtondown.yMin = 61;
    hiSpinnerECGButtondown.yMax = 81;
    hiSpinnerECGButtondown.borderWidth = 1;
    hiSpinnerECGButtondown.selected = false;
    hiSpinnerECGButtondown.fillColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerECGButtondown.borderColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerECGButtondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    hiSpinnerECGButtondown.textColor = GRAPHICS_COLOR_WHITE;
    hiSpinnerECGButtondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerECGButtondown.textXPos = 190;
    hiSpinnerECGButtondown.textYPos = 55;
    hiSpinnerECGButtondown.text = "-";
    hiSpinnerECGButtondown.font = &g_sFontCm30;

    //Initialize hiSpinnerIBPup Button
    hiSpinnerIBPButtonup.xMin = 184;
    hiSpinnerIBPButtonup.xMax = 204;
    hiSpinnerIBPButtonup.yMin = 105;
    hiSpinnerIBPButtonup.yMax = 125;
    hiSpinnerIBPButtonup.borderWidth = 1;
    hiSpinnerIBPButtonup.selected = false;
    hiSpinnerIBPButtonup.fillColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerIBPButtonup.borderColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerIBPButtonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    hiSpinnerIBPButtonup.textColor = GRAPHICS_COLOR_WHITE;
    hiSpinnerIBPButtonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerIBPButtonup.textXPos = 190;
    hiSpinnerIBPButtonup.textYPos = 108;
    hiSpinnerIBPButtonup.text = "+";
    hiSpinnerIBPButtonup.font = &g_sFontCm14;

    //Initialize hiSpinnerIBPdown Button
    hiSpinnerIBPButtondown.xMin = 184;
    hiSpinnerIBPButtondown.xMax = 204;
    hiSpinnerIBPButtondown.yMin = 130;
    hiSpinnerIBPButtondown.yMax = 150;
    hiSpinnerIBPButtondown.borderWidth = 1;
    hiSpinnerIBPButtondown.selected = false;
    hiSpinnerIBPButtondown.fillColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerIBPButtondown.borderColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerIBPButtondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    hiSpinnerIBPButtondown.textColor = GRAPHICS_COLOR_WHITE;
    hiSpinnerIBPButtondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerIBPButtondown.textXPos = 190;
    hiSpinnerIBPButtondown.textYPos = 124;
    hiSpinnerIBPButtondown.text = "-";
    hiSpinnerIBPButtondown.font = &g_sFontCm30;

    //Initialize hiSpinnerSPO2up Button
    hiSpinnerSPO2Buttonup.xMin = 184;
    hiSpinnerSPO2Buttonup.xMax = 204;
    hiSpinnerSPO2Buttonup.yMin = 174;
    hiSpinnerSPO2Buttonup.yMax = 194;
    hiSpinnerSPO2Buttonup.borderWidth = 1;
    hiSpinnerSPO2Buttonup.selected = false;
    hiSpinnerSPO2Buttonup.fillColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerSPO2Buttonup.borderColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerSPO2Buttonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    hiSpinnerSPO2Buttonup.textColor = GRAPHICS_COLOR_WHITE;
    hiSpinnerSPO2Buttonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerSPO2Buttonup.textXPos = 190;
    hiSpinnerSPO2Buttonup.textYPos = 177;
    hiSpinnerSPO2Buttonup.text = "+";
    hiSpinnerSPO2Buttonup.font = &g_sFontCm14;

    //Initialize hiSpinnerIBPdown Button
    hiSpinnerSPO2Buttondown.xMin = 184;
    hiSpinnerSPO2Buttondown.xMax = 204;
    hiSpinnerSPO2Buttondown.yMin = 199;
    hiSpinnerSPO2Buttondown.yMax = 219;
    hiSpinnerSPO2Buttondown.borderWidth = 1;
    hiSpinnerSPO2Buttondown.selected = false;
    hiSpinnerSPO2Buttondown.fillColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerSPO2Buttondown.borderColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerSPO2Buttondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    hiSpinnerSPO2Buttondown.textColor = GRAPHICS_COLOR_WHITE;
    hiSpinnerSPO2Buttondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    hiSpinnerSPO2Buttondown.textXPos = 190;
    hiSpinnerSPO2Buttondown.textYPos = 193;
    hiSpinnerSPO2Buttondown.text = "-";
    hiSpinnerSPO2Buttondown.font = &g_sFontCm30;

    //Initialize Spinner Hour Button Up Button
    SpinnerHourButtonup.xMin = 300;
    SpinnerHourButtonup.xMax = 320;
    SpinnerHourButtonup.yMin = 36;
    SpinnerHourButtonup.yMax = 56;
    SpinnerHourButtonup.borderWidth = 1;
    SpinnerHourButtonup.selected = false;
    SpinnerHourButtonup.fillColor = GRAPHICS_COLOR_GRAY;
    SpinnerHourButtonup.borderColor = GRAPHICS_COLOR_GRAY;
    SpinnerHourButtonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    SpinnerHourButtonup.textColor = GRAPHICS_COLOR_WHITE;
    SpinnerHourButtonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    SpinnerHourButtonup.textXPos = 306;
    SpinnerHourButtonup.textYPos = 39;
    SpinnerHourButtonup.text = "+";
    SpinnerHourButtonup.font = &g_sFontCm14;

    //Initialize Spinner Hour Down Button
    SpinnerHourButtondown.xMin = 300;
    SpinnerHourButtondown.xMax = 320;
    SpinnerHourButtondown.yMin = 61;
    SpinnerHourButtondown.yMax = 81;
    SpinnerHourButtondown.borderWidth = 1;
    SpinnerHourButtondown.selected = false;
    SpinnerHourButtondown.fillColor = GRAPHICS_COLOR_GRAY;
    SpinnerHourButtondown.borderColor = GRAPHICS_COLOR_GRAY;
    SpinnerHourButtondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    SpinnerHourButtondown.textColor = GRAPHICS_COLOR_WHITE;
    SpinnerHourButtondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    SpinnerHourButtondown.textXPos = 306;
    SpinnerHourButtondown.textYPos = 55;
    SpinnerHourButtondown.text = "-";
    SpinnerHourButtondown.font = &g_sFontCm30;

    //Initialize SpinnerMinutesButtonrup Button
    SpinnerMinutesButtonup.xMin = 300;
    SpinnerMinutesButtonup.xMax = 320;
    SpinnerMinutesButtonup.yMin = 105;
    SpinnerMinutesButtonup.yMax = 125;
    SpinnerMinutesButtonup.borderWidth = 1;
    SpinnerMinutesButtonup.selected = false;
    SpinnerMinutesButtonup.fillColor = GRAPHICS_COLOR_GRAY;
    SpinnerMinutesButtonup.borderColor = GRAPHICS_COLOR_GRAY;
    SpinnerMinutesButtonup.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    SpinnerMinutesButtonup.textColor = GRAPHICS_COLOR_WHITE;
    SpinnerMinutesButtonup.selectedTextColor = GRAPHICS_COLOR_GRAY;
    SpinnerMinutesButtonup.textXPos = 306;
    SpinnerMinutesButtonup.textYPos = 108;
    SpinnerMinutesButtonup.text = "+";
    SpinnerMinutesButtonup.font = &g_sFontCm14;

    //Initialize maxSpinnerMinutesdown Button
    SpinnerMinutesButtondown.xMin = 300;
    SpinnerMinutesButtondown.xMax = 320;
    SpinnerMinutesButtondown.yMin = 130;
    SpinnerMinutesButtondown.yMax = 150;
    SpinnerMinutesButtondown.borderWidth = 1;
    SpinnerMinutesButtondown.selected = false;
    SpinnerMinutesButtondown.fillColor = GRAPHICS_COLOR_GRAY;
    SpinnerMinutesButtondown.borderColor = GRAPHICS_COLOR_GRAY;
    SpinnerMinutesButtondown.selectedColor = GRAPHICS_COLOR_LIGHT_GRAY;
    SpinnerMinutesButtondown.textColor = GRAPHICS_COLOR_WHITE;
    SpinnerMinutesButtondown.selectedTextColor = GRAPHICS_COLOR_GRAY;
    SpinnerMinutesButtondown.textXPos = 306;
    SpinnerMinutesButtondown.textYPos = 124;
    SpinnerMinutesButtondown.text = "-";
    SpinnerMinutesButtondown.font = &g_sFontCm30;
}

void MessageAlarmECG()
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "Alarm ECG", AUTO_STRING_LENGTH,
                                      0, 1, TRANSPARENT_TEXT);
}

void MessageAlarmIBP()
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "Alarm IBP", AUTO_STRING_LENGTH,
                                      0, 71, TRANSPARENT_TEXT);
}

void MessageAlarmSPO2()
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "Alarm SPO2", AUTO_STRING_LENGTH,
                                      0, 142, TRANSPARENT_TEXT);
}

void EraseMessageAlarm()
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "Alarm ECG", AUTO_STRING_LENGTH,
                                      0, 1, TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "Alarm IBP", AUTO_STRING_LENGTH,
                                      0, 71, TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "Alarm SPO2", AUTO_STRING_LENGTH,
                                      0, 142, TRANSPARENT_TEXT);
}


void boardInit()
{
    FPU_enableModule();
}



void DelayLcd(uint16_t msec){
    uint32_t i = 0;
    uint32_t time = (msec / 1000) * (SYSTEM_CLOCK_SPEED / 15);

    for(i = 0; i < time; i++)
    {
        ;
    }
}





