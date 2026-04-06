/**
 * @file MEF_peatones.c
 * @author Federico Scheytt
 * @version 1.0
 * @date 04-2024
 *
 */

/*===============================[ Inclusions ]===============================*/
#include "MEF_peatones.h"
#include "SD2_board.h"

#define TIME_10_SEC 	1000	// Por defecto 10000 (10 segundos)
#define TIME_1_MIN	 	6000	// Por defecto 60000 (1 minuto)
#define TIME_200_MS 	100		// Por defecto 200   (20 ms)

/* Estados de la MEF */
typedef enum {
    EST_peatones_1 = 0,
	EST_peatones_2,
	EST_peatones_3,
} EST_MEF_peatones_enum;

static EST_MEF_peatones_enum EST_MEF_peatones;
static int32_t count_1ms_peatones;
static int32_t count_1ms_peatones_blink;


extern void MEF_peatones_reset(void)
{
	EST_MEF_peatones = EST_peatones_1;
	count_1ms_peatones = TIME_10_SEC;
	count_1ms_peatones_blink = TIME_200_MS;
}

/* Retorna true cuando termina su ejecucion */
extern bool MEF_peatones(void) {
	bool end = false;

	/* Maquina de estados finitos */
    switch(EST_MEF_peatones) {
    	/* Caso peatones 1 */
        case EST_peatones_1:
        	/* Ruta por desabilitarse */
			board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_OFF);

			/* Camino secundario deshabilitado */
			board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_ON);

			if(!count_1ms_peatones_blink && count_1ms_peatones) {
				/* Tiempos de blink */
				count_1ms_peatones_blink = TIME_200_MS;

				/* Ruta por desabilitarse */
				board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_TOGGLE);
			}

			if(!count_1ms_peatones) {
				/* Cargo los tiempos del siguiente estado */
				count_1ms_peatones = TIME_1_MIN;

				/* Transiciono al proximo estado */
				EST_MEF_peatones = EST_peatones_2;
			}
            break;

		/* Caso peatones 2 */
        case EST_peatones_2:
        	/* Ruta desabilitada */
			board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_ON);

			/* Camino secundario habilitado */
			board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_OFF);

			if(!count_1ms_peatones) {
				/* Cargo los tiempos del siguiente estado */
				count_1ms_peatones = TIME_10_SEC;
				count_1ms_peatones_blink = TIME_200_MS;

				/* Transiciono al proximo estado */
				EST_MEF_peatones = EST_peatones_3;
			}
            break;

		/* Caso peatones 3 */
        case EST_peatones_3:
        	/* Ruta por habilitarse */
        	board_setLed(BOARD_LED_ID_VERDE_RUTA, BOARD_LED_MSG_OFF);

        	/* Camino secundario habilitado */
        	board_setLed(BOARD_LED_ID_VERDE_SEC, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_ROJO_SEC, BOARD_LED_MSG_OFF);

			if(!count_1ms_peatones_blink && count_1ms_peatones) {
				/* Tiempos de blink */
				count_1ms_peatones_blink = TIME_200_MS;

            	/* Ruta por bilitarse */
				board_setLed(BOARD_LED_ID_ROJO_RUTA, BOARD_LED_MSG_TOGGLE);
			}

			if(!count_1ms_peatones) {
				end = true;
			}
            break;
    }
    return end;
}

/*===========================[ Function implementation ]======================*/

/* Funcion que decrementa los contadores */
extern void MEF_peatones_task1ms(void) {
	/* Decremento el contador */
    if(count_1ms_peatones) {
    	count_1ms_peatones--;
    }

    /* Decremento contadores solamente en los estados 2 y 4 donde hay blink */
    if(count_1ms_peatones_blink && ((EST_MEF_peatones == EST_peatones_1) || (EST_MEF_peatones == EST_peatones_3))){
    	count_1ms_peatones_blink--;
    }
}

