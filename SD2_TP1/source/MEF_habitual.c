/**
 * @file MEF_habitual.c
 * @author Federico Scheytt
 * @version 1.0
 * @date 04-2024
 *
 */

/*===============================[ Inclusions ]===============================*/
#include "MEF_habitual.h"
#include "SD2_board.h"
#include "key.h"
#include "cars.h"

#define TIME_2_MIN 		12000	// Por defecto 120000 	(2 min)
#define TIME_5_SEC 		2000	// Por defecto 5000 	(5 segundos)
#define TIME_30_SEC 	3000	// Por defecto 30000 	(30 segundos)
#define TIME_200_MS 	100		// Por defecto 200 		(200 ms)

/* Estados de la MEF */
typedef enum {
    EST_habitual_1 = 0,
	EST_habitual_2,
	EST_habitual_3,
	EST_habitual_4,
} EST_MEF_habitual_enum;

typedef enum {
    HABILITADO,
	NO_HABILITADO,
} habilitado_peatones_enum;

static EST_MEF_habitual_enum EST_MEF_habitual;
static habilitado_peatones_enum habilitado_peatones;
static int32_t count_1ms_habitual;
static int32_t count_1ms_habitual_blink;

extern void MEF_habitual_reset(void) {
	EST_MEF_habitual = EST_habitual_1;
	count_1ms_habitual = TIME_2_MIN;
    habilitado_peatones = HABILITADO;
}

extern void MEF_habitual(void) {
	/* Maquina de estados finitos */
    switch(EST_MEF_habitual) {
    	/* Caso habitual 1 */
        case EST_habitual_1:
        	/* Ruta habilitada */
            board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_OFF);

			/* Camino secundario deshabilitado */
            board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_ON);

            if(!count_1ms_habitual) {
            	/* Desabilito el paso de peatones en este ciclo */
            	habilitado_peatones = NO_HABILITADO;

            	/* Cargo los tiempos del siguiente estado */
            	count_1ms_habitual = TIME_5_SEC;
            	count_1ms_habitual_blink = TIME_200_MS;

            	/* Reseteo el contador de vehiculos dado que se acumularon
				 * menos de 3 */
				count_of_cars_reset();

            	/* Transiciono al proximo estado */
            	EST_MEF_habitual = EST_habitual_2;
            }

            /* Al presionarse sw3 se acumula un vehiculo en el camino
             * secundario */
            if(key_getPressEv(BOARD_SW_ID_3)) {
            	count_of_cars_increase();
            }

            break;

		/* Caso habitual 2 */
        case EST_habitual_2:
        	/* Ruta por desabilitarse */
        	board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_OFF);

			/* Camino secundario deshabilitado */
			board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_ON);

            if(!count_1ms_habitual_blink && count_1ms_habitual) {
            	/* Tiempos de blink */
            	count_1ms_habitual_blink = TIME_200_MS;

            	/* Ruta por desabilitarse */
            	board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_TOGGLE);
            }

			if(!count_1ms_habitual) {
				/* Cargo los tiempos del siguiente estado */
				count_1ms_habitual = TIME_30_SEC;

				/* Transiciono al proximo estado */
                EST_MEF_habitual = EST_habitual_3;
            }
            break;

		/* Caso habitual 3 */
        case EST_habitual_3:
        	/* Ruta desabilitada */
        	board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_ON);

			/* Camino secundario habilitado */
			board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_OFF);

			if (!count_1ms_habitual) {
				/* Cargo los tiempos del siguiente estado */
				count_1ms_habitual = TIME_5_SEC;
				count_1ms_habitual_blink = TIME_200_MS;

				/* Transiciono al proximo estado */
				EST_MEF_habitual = EST_habitual_4;
			}
            break;

		/* Caso habitual 4 */
        case EST_habitual_4:
        	/* Ruta desabilitada */
        	board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_OFF);
        	board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_ON);

        	/* Camino por desabilitarse */
        	board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_OFF);

			if (!count_1ms_habitual_blink && count_1ms_habitual) {
				/* Tiempos de blink */
				count_1ms_habitual_blink = TIME_200_MS;

				/* Camino por desabilitarse */
				board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_TOGGLE);
			}

			if (!count_1ms_habitual) {
				/* Habilito el paso de peatones dado que termino el ciclo */
				habilitado_peatones = HABILITADO;

				/* Cargo los tiempos del siguiente estado */
				count_1ms_habitual = TIME_2_MIN;

				/* Limpio el estado de los switches */
				key_getPressEv(BOARD_SW_ID_1);
				key_getPressEv(BOARD_SW_ID_3);

				/* Transiciono al proximo estado */
				EST_MEF_habitual = EST_habitual_1;
			}
			break;
    }
}

/*===========================[ Function implementation ]======================*/

/* Funcion que decrementa los contadores */
extern void MEF_habitual_task1ms(void) {
	/* Decremento el contador */
    if (count_1ms_habitual) {
    	count_1ms_habitual--;
    }

    /* Decremento contadores solamente en los estados 2 y 4 donde hay blink */
    if (count_1ms_habitual_blink && ((EST_MEF_habitual == EST_habitual_2) || (EST_MEF_habitual == EST_habitual_4))) {
    	count_1ms_habitual_blink--;
    }
}

/* Funcion que retorna el estado de la habilitacion del paso peatonal */
extern bool habilitado_peatones_get(void) {
	bool value = false;

	if(habilitado_peatones == HABILITADO){
		value = true;
	}
	return value;
}

/* Funcion que setea el estado de la habilitacion del paso peatonal */
extern void habilitado_peatones_set(bool value) {
	if (value) {
		habilitado_peatones = HABILITADO;
	} else {
		habilitado_peatones = NO_HABILITADO;
	}
}

