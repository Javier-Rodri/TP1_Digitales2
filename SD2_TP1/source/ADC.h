/*
 * ADC.h
 *
 *  Created on: 14 abr. 2026
 *      Author: Javier
 */

#ifndef ADC_LUZ_H_
#define ADC_LUZ_H_

#include <fsl_adc16.h>
#include <stdint.h>

// El sensor de luz en la FRDM-KL46Z suele estar en el canal 3
#define ADC_CANAL_SENSOR_LUZ 3U

 extern void ADC_Init(void);
 extern uint32_t ADC_Leer_Luz(void);

#endif
