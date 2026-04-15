/*
 * MEF_Luminaria.h
 *
 *  Created on: 14 abr. 2026
 *      Author: Javier
 */

#ifndef MEF_LUMINARIA_H_
#define MEF_LUMINARIA_H_

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void MEF_Luminaria_Init(void);
extern void MEF_Luminaria_Task(void);
extern void MEF_Luminaria_tick(void);

#ifdef __cplusplus
}
#endif


#endif /* MEF_LUMINARIA_H_ */
