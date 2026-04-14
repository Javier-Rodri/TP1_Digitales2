

/*===============================[ Inclusions ]===============================*/
#include "MEF_semaforos.h"
#include "MEF_habitual.h"
#include "MEF_peatones.h"
#include "MEF_acumulado.h"
#include "cars.h"
#include "SD2_board.h"
#include "key.h"

/* Cantidad maxima de vehiculos a acumular en el camino secundario */
#define MAX_NUMBER_OF_CARS 3

/* Estados de la MEF */
typedef enum {
	EST_habitual = 0,
	EST_peatones,
	EST_acumulado,
} EST_MEF_semaforos_enum;

static EST_MEF_semaforos_enum EST_MEF_semaforos;

extern void MEF_semaforos_init() {
	/* Al iniciar el sistema comienza en el modo habitual */
	EST_MEF_semaforos = EST_habitual;

	/* Se resetea la MEF habitual */
	MEF_habitual_reset();
}

extern void MEF_semaforos() {
	/* Maquina de estados finitos */
	switch (EST_MEF_semaforos) {
		/* Caso habitual */
		case EST_habitual:
			MEF_habitual();

			if(key_getPressEv(BOARD_SW_ID_1) && habilitado_peatones_get()) {
				/* Transicion a MEF_peatones con reset */
				MEF_peatones_reset();
				EST_MEF_semaforos = EST_peatones;
			}

			if(count_of_cars_get() == MAX_NUMBER_OF_CARS) {
				/* Transicion a MEF_acumulado con reset */
				MEF_acumulado_reset();
				EST_MEF_semaforos = EST_acumulado;
			}
		break;

		/* Caso peatones */
		case EST_peatones:
			/* Esta MEF retorna true cuando termina su ejecucion */
			if(MEF_peatones()) {
				/* Limpio el estado de sw1 teniendo en cuenta que se hubiera
				 * pulsado por error */
				key_getPressEv(BOARD_SW_ID_1);

				/* Limpio el estado de sw3 teniendo en cuenta que durante el
				 * funcionamiento de la MEF_peatones el camino secundario
				 * estuvo habilitado por lo que no deberia haber autos
				 * acumulados */
				key_getPressEv(BOARD_SW_ID_3);

				/* Desabilito las posibles transiciones a MEF_peatones dado que
				 * la transicion hacia MEF_habitual se da sin reset */
				habilitado_peatones_set(false);

				/* Transicion a MEF_habitual sin reset */
				EST_MEF_semaforos = EST_habitual;
			}

		break;

		/* Caso acumulado */
		case EST_acumulado:
			/* Esta MEF retorna true cuando termina su ejecucion */
			if(MEF_acumulado()) {
				/* Limpio el estado de sw1 teniendo en cuenta que se hubiera
				 * pulsado por error */
				key_getPressEv(BOARD_SW_ID_1);

				/* Limpio el estado de sw3 teniendo en cuenta que durante el
				 * funcionamiento de la MEF_acumulado el camino secundario
				 * estuvo habilitado por lo que no deberia haber autos
				 * acumulados */
				key_getPressEv(BOARD_SW_ID_3);

				/* Transicion a MEF_habitual con reset */
				MEF_habitual_reset();
				EST_MEF_semaforos = EST_habitual;
			}

		break;
	}
}

/*===========================[ Function implementation ]======================*/

extern void MEF_semaforos_task1ms(void) {
    if(EST_MEF_semaforos == EST_habitual){
        MEF_habitual_task1ms();
    }

    if(EST_MEF_semaforos == EST_peatones){
    	MEF_peatones_task1ms();
    }

    if(EST_MEF_semaforos == EST_acumulado){
    	MEF_acumulado_task1ms();
	}
}
