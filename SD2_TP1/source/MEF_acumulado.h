/*
 * MEF_acumulado.h
 *
 *      Author: Fede
 */

#ifndef MEF_ACUMULADO_H_
#define MEF_ACUMULADO_H_

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void MEF_acumulado_reset(void);
extern bool MEF_acumulado(void);
extern void MEF_acumulado_task1ms(void);

#ifdef __cplusplus
}
#endif

#endif /* MEF_ACUMULADO_H_ */
