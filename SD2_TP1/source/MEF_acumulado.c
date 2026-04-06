/**
 * @file MEF_acumulado.c
 * @author Federico Scheytt
 * @version 1.0
 * @date 04-2024
 *
 */

/*===============================[ Inclusions ]===============================*/
#include "MEF_acumulado.h"
#include "SD2_board.h"
#include "key.h"
#include "cars.h"

#define TIME_5_SEC	 	1000	// Por defecto 5000 (5 segundos)
#define TIME_200_MS 	100		// Por defecto 200  (200 ms)

/* Estados de la MEF */
typedef enum {
    EST_acumulado_1 = 0,
	EST_acumulado_2,
	EST_acumulado_3,
} EST_MEF_acumulado_enum;

static EST_MEF_acumulado_enum EST_MEF_acumulado;
static int32_t count_1ms_acumulado;
static int32_t count_1ms_acumulado_blink;


extern void MEF_acumulado_reset(void) {
	EST_MEF_acumulado = EST_acumulado_1;
	count_1ms_acumulado = TIME_5_SEC;
	count_1ms_acumulado_blink = TIME_200_MS;
}

/* Retorna true cuando termina su ejecucion */
extern bool MEF_acumulado(void) {
	bool end = 0;

	/* Maquina de estados finitos */
    switch(EST_MEF_acumulado) {
    	/* Caso acumulado 1 */
        case EST_acumulado_1:
        	/* Ruta por desabilitarse */
			board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_OFF);

			/* Camino secundario deshabilitado */
			board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_ON);

			if(!count_1ms_acumulado_blink && count_1ms_acumulado) {
				/* Tiempos de blink */
				count_1ms_acumulado_blink = TIME_200_MS;

				/* Ruta por desabilitarse */
				board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_TOGGLE);
			}

			if(!count_1ms_acumulado) {
				/* Transiciono al proximo estado */
				EST_MEF_acumulado = EST_acumulado_2;
			}

			if(key_getPressEv(BOARD_SW_ID_3)) {
				/* En este estado se puede seguir incrementando el contador */
				count_of_cars_increase();
			}
            break;

		/* Caso acumulado 1 */
        case EST_acumulado_2:
        	/* Ruta desabilitada */
			board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_ON);

			/* Camino secundario habilitado */
			board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_OFF);

			if(key_getPressEv(BOARD_SW_ID_3)) {
				/* En cada pulsada dal sw3 se simula que la cantidad de
				 * vehiculos acumulados disminuye */
				count_of_cars_decrease();
			}

			if(!count_of_cars_get()) {
				/* Cargo los tiempos del siguiente estado */
				count_1ms_acumulado = TIME_5_SEC;
				count_1ms_acumulado_blink = TIME_200_MS;

				/* Transiciono al proximo estado */
				EST_MEF_acumulado = EST_acumulado_3;
			}
            break;

		/* Caso acumulado 3 */
        case EST_acumulado_3:
        	/* Ruta desabilitada */
			board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_ON);

			/* Camino or desabilitarse */
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_OFF);

			if(!count_1ms_acumulado_blink && count_1ms_acumulado) {
				/* Tiempos de blink */
				count_1ms_acumulado_blink = TIME_200_MS;

				/* Camino por desabilitarse */
				board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_TOGGLE);
			}

			if(!count_1ms_acumulado) {
				end = 1;
			}
            break;
    }
    return end;
}

/*===========================[ Function implementation ]======================*/

/* Funcion que decrementa los contadores */
extern void MEF_acumulado_task1ms(void) {
    if(count_1ms_acumulado) {
    	count_1ms_acumulado--;
    }

    if(count_1ms_acumulado_blink && ((EST_MEF_acumulado == EST_acumulado_1) || (EST_MEF_acumulado == EST_acumulado_3))) {
    	count_1ms_acumulado_blink--;
	}
}
