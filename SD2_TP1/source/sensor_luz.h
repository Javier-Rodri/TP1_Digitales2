/*
 * sensor_luz.h
 *
 *  Created on: 15 abr. 2026
 *      Author: Javier
 */


#ifndef SENSOR_LUZ_H_
#define SENSOR_LUZ_H_

#include <stdint.h>

/**
 * Inicializa el periférico ADC0, calibra el hardware y habilita
 * las interrupciones en el NVIC para el ADC.
 */
void sensorLuz_init(void);

/**
 * Inicia una nueva conversión en el canal del sensor de luz.
 * Esta función no bloquea; el resultado se procesará en la ISR.
 * Debe ser llamada desde el SysTick_Handler cada 100ms.
 */
void sensorLuz_dispararLectura(void);

/**
 * Retorna el último porcentaje de luz calculado por la interrupción.
 * @return float Valor de 0.0 a 100.0 representativo del nivel de luz.
 */
float sensorLuz_obtenerValor(void);

#endif /* SENSOR_LUZ_H_ */



