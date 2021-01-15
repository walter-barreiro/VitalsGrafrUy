/**
 * @file main.c
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de inicializar los periféricos, consultar las banderas, consultar los botones touchscreen de la pantalla principal
 * y de ejecutar los handlers.
 *
 *
 * @author Diego Alonso Germán Minetti Walter Barreiro
 * @version 1.7
 * @date 27 de Mayo de 2020
 *
 * main.c
 *
 */


/* System Includes*/
#include <msp432.h>
#include "display_interfaz.h"
#include "display_vitals.h"
#include "procesamiento.h"
#include "timer.h"
#include "timer_hw.h"
#include "wifi.h"
#include "sl_common.h"

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>




int32_t cantTicks = 18; // 18 ticks 50 ms
time_t time_init = {10, 45, 53, 500};
int32_t i=0;
int32_t count28 = 0;
int32_t InitWifiOK;
volatile int8_t time_disp[7];
volatile time_t time_main;
int32_t startgraph = 0;
static char TimerA0Flag1000;
static char TimerA0Flag250;
static char TimerA0Flag50;
static char TimerA0Flag28;
static char AlarmECGFlag;
static char AlarmIBPFlag;
static char AlarmSPO2Flag;

bool button_selectedsetup = false;
bool button_selectedstart = false;
bool button_selectedstopAlarm = false;
bool firsttime_graph = true;
bool alarm_on = false;
bool alarm_on_firsttime = false;
bool configs_recibida = false;

const uint32_t cantpoints = 3599;


const eUSCI_UART_Config uartConfig =
  {
          EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
          78,                                     // BRDIV = 78
          2,                                       // UCxBRF = 2
          0,                                       // UCxBRS = 0
          EUSCI_A_UART_NO_PARITY,                  // No Parity
          EUSCI_A_UART_LSB_FIRST,                  // LSB First
          EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
          EUSCI_A_UART_MODE,                       // UART mode
          EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
  };

//----------------------------------------------------------
/** Programa principal

*/

void main()
{
    //Inicialization
    stopWDT();

   // 48 Mhz Clock Module
   clockInit();
   //Display
    boardInit();
    initializeButtons();

   // LCD setup using Graphics Library API calls
   Kitronix320x240x16_SSD2119Init();
   Graphics_initContext(&g_sContext, &g_sKitronix320x240x16_SSD2119);
   Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
   Graphics_setFont(&g_sContext, &g_sFontCmss20b);

   Graphics_clearDisplay(&g_sContext);
   //Inicio de la funcionalidad touchscreen
   touch_initInterface();

   // Initialize Wifi
   Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
   Graphics_drawString(&g_sContext, "Inicializando Wifi y TCP Server", AUTO_STRING_LENGTH,
                                   20, 30, TRANSPARENT_TEXT);

   if (InitWifi() > 0)
   {
      if (SetInterfaz() > 0)
          configs_recibida = true;

      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
      Graphics_drawString(&g_sContext, "Inicializando Wifi y TCP Server", AUTO_STRING_LENGTH,
                                              20, 30, TRANSPARENT_TEXT);
      Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
      Graphics_drawString(&g_sContext, "Conectado a", AUTO_STRING_LENGTH,
                                              20, 30, TRANSPARENT_TEXT);
      Graphics_drawString(&g_sContext, SSID_AP_MODE, AUTO_STRING_LENGTH,
                                              150, 30, TRANSPARENT_TEXT);
      Graphics_drawString(&g_sContext, "Servidor:", AUTO_STRING_LENGTH,
                                                 20, 80 , TRANSPARENT_TEXT);
      Graphics_drawString(&g_sContext, SERVERNAME, AUTO_STRING_LENGTH,
                                                 20, 120, TRANSPARENT_TEXT);
      if (configs_recibida) Graphics_drawString(&g_sContext, "Configuracion recibida", AUTO_STRING_LENGTH,
                                             20, 160, TRANSPARENT_TEXT);
          else Graphics_drawString(&g_sContext, "Configuracion NO recibida", AUTO_STRING_LENGTH,
                            20, 160, TRANSPARENT_TEXT);

   } else  Graphics_drawString(&g_sContext, "Conexion fallida", AUTO_STRING_LENGTH,
                               20, 80 , TRANSPARENT_TEXT);


   DelayLcd(3000);
   //Pantalla bienvenida
   Graphics_drawImage(&g_sContext, &ImageSisem8BPP_UNCOMP, 0, 0);
   DelayLcd(3000);
   //Initialize Timer
   if (!configs_recibida)
   set_time(time_init);
   //Set flags
   SetFlagTimerA0 (&TimerA0Flag1000, &TimerA0Flag28);
   SetFlagAlarms (&AlarmECGFlag, &AlarmIBPFlag, &AlarmSPO2Flag);
   //Set cantidad de ticks
   SetCantTicks(&cantTicks);

   initTimer();
   // Main screen

   drawMainMenu();

   while(1)
   {

       if (TimerA0Flag1000)
       {
           TimerA0Flag1000 = 0;
           if (g_runMainMenu)
           display_time();
           if (startgraph)
           {
              display_value_PR();
              display_value_IBP();
              display_value_SPO2();
           }
           if (alarm_on)
           MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
           // Para usar con wifi durante todo el programa en una futura implementación
           /*if (refreshCmds()>0)
                       CLI_Write('Configuración recibida correctamente desde PC \n\r');
             else CLI_Write('Configuración NO recibida correctamente desde PC \n\r');
           */


       }  //flag1000

       //por si se precisa flag 250
       if (TimerA0Flag250)
       {
           TimerA0Flag250 = 0;
       }
       if (TimerA0Flag28)
       {
           TimerA0Flag28 = 0;
           if (g_runMainMenu)
           {
               if (startgraph)
               {
                   if (count28 < cantpoints)
                   display_graph_ECG();
                   display_graph_IBP();
                   display_graph_SPO2();
                   count28++;
                   if (count28 > cantpoints)
                       {
                         startgraph = 0;
                         count28 = 0;
                         firsttime_graph = true;
                         initvariables();
                       }
               }

           }
       }//flag28
       if (AlarmECGFlag)
       {
           AlarmECGFlag = 0;
           if (!alarm_on)
           {
               alarm_on = true;
               alarm_on_firsttime = true;
           }
           MessageAlarmECG();

       }//alarmECG

       if (AlarmIBPFlag)
       {
           AlarmIBPFlag = 0;
           if (!alarm_on)
           {
               alarm_on = true;
               alarm_on_firsttime = true;
           }
           MessageAlarmIBP();

       }//alarmIBP
       if (AlarmSPO2Flag)
       {
           AlarmSPO2Flag = 0;
           if (!alarm_on)
           {
               alarm_on = true;
               alarm_on_firsttime = true;
           }
           MessageAlarmSPO2();

       }//alarmSPO2


       touch_updateCurrentTouch(&g_sTouchContext);
       /*muestra boton de alarma*/
       if(alarm_on_firsttime)
       {
           alarm_on_firsttime = false;
           Graphics_setFont(&g_sContext, &g_sFontCmss12);
           Graphics_drawButton(&g_sContext, &stopAlarmButton);
       } //alarmonfirst

      if(alarm_on)
      {
       if(Graphics_isButtonSelected(&stopAlarmButton, g_sTouchContext.x, g_sTouchContext.y))
       {
           //Graphics_drawSelectedButton(&g_sContext, &stopAlarmButton);
           button_selectedstopAlarm = true;
       }else
            {
                if (button_selectedstopAlarm)
                {
                    button_selectedstopAlarm = false;
                    Graphics_drawButton(&g_sContext, &stopAlarmButtonoff);
                    alarm_on = false;
                    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    EraseMessageAlarm();
                }
            }
          }
      if (!alarm_on)
      {

          if(Graphics_isButtonSelected(&setupButton, g_sTouchContext.x, g_sTouchContext.y))
          {
           Graphics_drawSelectedButton(&g_sContext, &setupButton);
           button_selectedsetup = true;
          }else
           {
               if (button_selectedsetup)
               {
                   button_selectedsetup = false;
                   Graphics_drawReleasedButton(&g_sContext, &setupButton);
                   drawSetupMenu();
               }
           }

          if(Graphics_isButtonSelected(&startButton, g_sTouchContext.x, g_sTouchContext.y))
          {
           Graphics_drawSelectedButton(&g_sContext, &startButton);
           button_selectedstart = true;
          }else
           {
               if (button_selectedstart)
               {
                   button_selectedstart = false;
                   Graphics_drawReleasedButton(&g_sContext, &startButton);
                   if(!startgraph && firsttime_graph)  //si no comenzo grafica y es la primera vez
                   {
                       scale_adaptECG();
                       scale_adaptIBP();
                       scale_adaptSPO2();
                       initvariables();
                       receivearrayECG(); // recibo primer array
                       receivearrayIBP();
                       receivearraySPO2();
                       firsttime_graph = false;
                   }
                   startgraph = !startgraph;   //comienzo o detengo grafica
               }
           }
      }

   } //if (!alarm_on)
}//main

  /* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
  {
      uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

      MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

      if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
      {
          MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
      }

  }


