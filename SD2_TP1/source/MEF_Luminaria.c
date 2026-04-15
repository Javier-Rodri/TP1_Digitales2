#include "MEF_Luminaria.h"
#include "sensor_luz.h"
#include "SD2_board.h"

typedef enum {
    ESTADO_CLARIDAD,   // LED APAGADO
    ESTADO_CHEQUEO,  // ESPERANDO 5 SEGUNDOS
    ESTADO_OSCURIDAD       // LED ENCENDIDO
} State_t;


/* --- CALIBRACIÓN DEL SENSOR --- */
// Valores típicos de ADC para el sensor de la KL46 (rango 0-4095)
#define ADC_MIN_LUZ        20.0f   // Valor con linterna (Mucha Luz)
#define ADC_MAX_OSCURIDAD  1500.0f  // Valor tapando el sensor (Oscuridad)

/* --- PARÁMETROS DEL ENUNCIADO --- */
#define UMBRAL_ENCENDIDO     0.25f  // 25%
#define UMBRAL_APAGADO         0.60f  // 60%
#define TIEMPO_5_SEG      5000

// Variables para el manejo de la MEF
volatile bool flanco_oscuro_detectado = false;
static State_t estado_actual = ESTADO_CLARIDAD;



static int32_t timer_chequeo;

void MEF_Luminaria_Init(void) {
    estado_actual = ESTADO_CLARIDAD;
    board_setLed(BOARD_LED_ID_Luminaria, BOARD_LED_MSG_OFF);
    //PTE->PSOR = (1 << 18);
}

void MEF_Luminaria_Task(void) {
    // Obtenemos el valor promedio del ADC (0 a 100)
    float luz_actual = sensorLuz_obtenerValor();



    switch (estado_actual) {

        case ESTADO_CLARIDAD:
            board_setLed(BOARD_LED_ID_Luminaria, BOARD_LED_MSG_OFF);
            //PTE->PSOR = (1 << 18);
            // Si la luz cae por debajo del 25% (hay oscuridad)
            if (luz_actual < (UMBRAL_ENCENDIDO*ADC_MAX_OSCURIDAD) ) {
                estado_actual = ESTADO_OSCURIDAD;
            }
            break;

        case ESTADO_OSCURIDAD:
            // Sigue apagado mientras espera
            board_setLed(BOARD_LED_ID_Luminaria, BOARD_LED_MSG_ON);
        	//PTE->PCOR = (1 << 18);
            if (luz_actual > (UMBRAL_APAGADO*ADC_MAX_OSCURIDAD)) {
                estado_actual = ESTADO_CHEQUEO;
                timer_chequeo = TIEMPO_5_SEG;
                flanco_oscuro_detectado = false;
            }
            break;

        case ESTADO_CHEQUEO:

            // Si supera el 60% de luz ambiente, se apaga al toque
            if (luz_actual < (UMBRAL_APAGADO*ADC_MAX_OSCURIDAD)) {
            	flanco_oscuro_detectado = true;
            }

			// Si detectó oscuridad en el proceso, aborta y vuelve a Oscuridad
			if (flanco_oscuro_detectado) {
				estado_actual = ESTADO_OSCURIDAD;
			}
			// Si pasaron los 5 segundos sin detectar oscuridad
			else if (timer_chequeo == 0) {
				// Pasaron 5s de oscuridad constante, ahora sí prendemos
				estado_actual = ESTADO_CLARIDAD;
			}
            break;

        default:
            MEF_Luminaria_Init();
            break;
    }
}

void MEF_Luminaria_tick(void) {
    if (timer_chequeo > 0) {
        timer_chequeo--;
    }
}
