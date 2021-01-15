/**
 * @file timer_hw.h
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este m�dulo se encarga de inicializar el reloj del sistema y del TimerA0.
 *
 *
 * @author Diego Alonso German Minetti Walter Barreiro
 * @version 1.7
 * @date 03 de Junio de 2020
 *
 * timer_hw.h
 *
 */

#ifndef TIMER_TIMER_HW_H_
#define TIMER_TIMER_HW_H_
/**
 * @brief Funci�n que indica posici�n de memoria de las banderas de 1000ms y 2,8ms.
 */

void SetFlagTimerA0 (char* TimerA0flag_main1000, char*TimerA0flag_main28);
/**
 * @brief Funci�n que indica posici�n de memoria la cantidad de ticks.
 */
void SetCantTicks(int32_t * cantTicks_main);
/**
 * @brief Funci�n que inicializa el reloj del sistema a 48MHz.
 */
void clockInit(void);
/**
 * @brief Funci�n que inicializa el TimerA0.
 */
void initTimer();

#endif /* TIMER_TIMER_HW_H_ */
