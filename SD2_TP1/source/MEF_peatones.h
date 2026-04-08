

#ifndef MEF_PEATONES_H_
#define MEF_PEATONES_H_

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void MEF_peatones_reset(void);
extern bool MEF_peatones(void);
extern void MEF_peatones_task1ms(void);

#ifdef __cplusplus
}
#endif

#endif /* MEF_PEATONES_H_ */
