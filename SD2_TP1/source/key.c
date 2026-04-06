#include "key.h"

typedef enum {
    ESPERANDO_ACTIVACION = 0,
    ESPERANDO_DESACTIVACION,
} estPul_enum;

static estPul_enum estSW[BOARD_SW_ID_TOTAL];
static bool eventSW[BOARD_SW_ID_TOTAL];

void key_init(void) {
    int32_t i;

    for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++)
    {
        estSW[i] = ESPERANDO_ACTIVACION;
        eventSW[i] = 0;
    }
}

bool key_getPressEv(board_swId_enum id) {
    bool ret = false;

    if (eventSW[id]) {
        eventSW[id] = 0;
        ret = true;
    }

    return ret;
}

/*	EJEMPLO DE CÓDIGO INCORRECTO PARA ESTA FUNCIÓN

bool key_getPressEv(board_swId_enum id) {
	bool ret = estSW[i];

	 // Si supongo que cuando se llamó a esta función estSW[i] = 0, entonces en
	 // la linea anterior se asigno a ret = 0.
	 // Ahora, si justo donde está este comentario, se lanzara una interrupcion
	 // del systick llamando a key_periodicTask1ms(), y se diera que justo hay
	 // una pulsacion y por ende estSW[i] = 1, al volver de la interrupcion y
	 // ejecutarse la linea siguiente, a estSW[i] se le asignaría un 0, y se
	 // retornaría ret = 0, perdiendo la pulsada

	estSW[i] = 0;
	return ret;
}

*/

void key_periodicTask1ms(void) {
    int32_t i;

    for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++) {
        switch (estSW[i]) {
            case ESPERANDO_ACTIVACION:
                if (board_getSw(i)) {
                    eventSW[i] = 1;
                    estSW[i] = ESPERANDO_DESACTIVACION;
                }
                break;

            case ESPERANDO_DESACTIVACION:
                if (!board_getSw(i)) {
                    estSW[i] = ESPERANDO_ACTIVACION;
                }
                break;

            default:
                estSW[i] = ESPERANDO_ACTIVACION;
                break;
        }
    }
}
