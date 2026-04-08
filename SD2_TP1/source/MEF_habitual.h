

#ifndef MEF_HABITUAL_H_
#define MEF_HABITUAL_H_

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void MEF_habitual_reset(void);
extern void MEF_habitual(void);
extern void MEF_habitual_task1ms(void);
extern bool habilitado_peatones_get(void);
extern void habilitado_peatones_set(bool);

#ifdef __cplusplus
}
#endif

#endif /* MEF_HABITUAL_H_ */
