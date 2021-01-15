/**
 * @file timer_hw.c
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de inicializar el reloj del sistema y del TimerA0.
 *
 * @paragraph
 * El reloj del sistema se inicializa en 48 Mhz. El TimerA0 interrumpe cada 2,8ms y en la ISR
 * se calculan los tiempos de 50ms, 250ms y 1000ms.
 *
 *
 * @author Diego Alonso German Minetti Walter Barreiro
 * @version 1.7
 * @date 03 de Junio de 2020
 *
 * timer_hw.c
 *
 */
/*System Include*/
#include <msp432.h>
#include "timer.h"
/* DriverLib Includes */
#include <driverlib.h>


/* Application Defines  */
#define TIMER_PERIOD    0x823

static char* TimerA0flag1000;
static char* TimerA0flag250;
static char* TimerA0flag50;
static char* TimerA0flag28;
int32_t* cantticks;
int16_t contador_18 = 0; // 50ms
int16_t contador_5 = 0;  // 250ms
int16_t contador_4 = 0; //1000ms

void SetFlagTimerA0 (char* TimerA0flag_main1000, char*TimerA0flag_main28)
{
    TimerA0flag1000 = TimerA0flag_main1000;
    TimerA0flag28 = TimerA0flag_main28;
}




void SetCantTicks(int32_t * cantTicks_main){

    cantticks = cantTicks_main;
}

void clockInit(void)
{
    /* 2 flash wait states, VCORE = 1, running off DC-DC, 48 MHz */
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    PCM_setPowerState(PCM_AM_DCDC_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_setDCOFrequency(48000000);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, 1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, 1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, 1);

    return;
}

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/64
        TIMER_PERIOD,                           // 5000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};


void initTimer(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring Timer_A1 for Up Mode */
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    /* Enabling interrupts and starting the timer */
   // MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_TA1_0);
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    
    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();  


}

void TA1_0_IRQHandler(void)
{
    *TimerA0flag28 = 1;
    //18 ticks de 2.8ms = 50ms
    if (contador_18 < *cantticks) contador_18++;
    if (contador_18 >= *cantticks)
    {
            contador_18 = 0;
            *TimerA0flag50 = 1;
            contador_5++;  //para llegar a 250ms
     }

    if (contador_5 >= 5)
    {
        contador_5 = 0;
        *TimerA0flag250 = 1;
        inc_time();
        contador_4++;
    }
    if (contador_4 >= 4)
    {
        contador_4 = 0;
        *TimerA0flag1000 = 1;
    }


    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
