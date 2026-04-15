/*
 * MEF_Luminaria.c
 *
 *  Created on: 14 abr. 2026
 *      Author: Javier
 */

#include "MKL46Z4.h"
#include "MEF_Luminaria.h"
#include <stdbool.h>
#include "SD2_board.h"
#include "ADC.h"



typedef enum { ESTADO_CLARIDAD, ESTADO_OSCURIDAD, ESTADO_CHEQUEO } State_t;

// Valores medidos en el ambiente
#define LUX_AMBIENTE_MIN    100U   // Valor ADC con el sensor tapado
#define LUX_AMBIENTE_MAX    3800U  // Valor ADC con mucha luz

// Definiciones de umbrales con respecto a la luz ambiente (Histéresis)
#define UMBRAL_ENCENDIDO 25.0f
#define UMBRAL_APAGADO   65.0f

#define TIME_5_SEC 		5000	// Por defecto 5000 	(5 segundos)
#define TIME_100_MS 	100		// Por defecto 100 		(100 ms)

// Variables para el manejo de la MEF
volatile bool flanco_oscuro_detectado = false;
static int32_t count_1ms_Luminaria=0;
static State_t estado_actual = ESTADO_CLARIDAD;

// Variables para el promedio del ADC
static int32_t timer_100ms = TIME_100_MS; // Contador para el muestreo
static uint32_t Valor_ADC_acum = 0;
static uint8_t Cantidad_muestras = 0;
static float porcentaje_promedio = 50.0f; // Valor inicial seguro


// Esta función se llama UNA VEZ en el main
void MEF_Luminaria_init(void) {
	ADC_Init(); // Configura el ADC
    estado_actual = ESTADO_CLARIDAD;
}

// Esta función traduce el valor RAW a porcentaje
float obtener_luz_porcentaje(void) {
    uint32_t raw = ADC_Leer_Luz(); // Llama al driver del ADC
    return (raw * 100.0f) / 4095.0f;
}


// Esta función ahora se encarga de recolectar muestras
static float actualizar_promedio_luz(void) {
    // Verificamos si pasaron los 100ms
    if (timer_100ms <= 0) {
        timer_100ms = 100; // Recargamos el timer

        // Tomamos una muestra y la sumamos al montón
        Valor_ADC_acum += ADC_Leer_Luz();
        Cantidad_muestras++;

        // Si ya tenemos 5 muestras (pasaron 500ms en total)
        if (Cantidad_muestras == 5) {
            // Calculamos el promedio: (Suma / 5) y luego a porcentaje
            float promedio_raw = (float)Valor_ADC_acum / 5.0f;
            porcentaje_promedio = (promedio_raw * 100.0f) / 4095.0f;

            // Limpiamos para la próxima tanda
            Valor_ADC_acum = 0;
            Cantidad_muestras = 0;
        }
    }
    return porcentaje_promedio;
}


extern bool MEF_Luminaria(void) {

	actualizar_promedio_luz();

	switch (estado_actual) {

		case ESTADO_CLARIDAD:
			board_setLed(BOARD_LED_ID_Luminaria, BOARD_LED_MSG_OFF);
			// Umbral bajo para entrar en oscuridad
			if (porcentaje_promedio < UMBRAL_ENCENDIDO) {
				estado_actual = ESTADO_OSCURIDAD;
			}
			break;

		case ESTADO_OSCURIDAD:
			board_setLed(BOARD_LED_ID_Luminaria, BOARD_LED_MSG_ON);
			// Umbral alto (histéresis) para intentar salir
			if (porcentaje_promedio >= UMBRAL_APAGADO) {
				estado_actual = ESTADO_CHEQUEO;
				count_1ms_Luminaria = TIME_5_SEC;// Seteo del cronómetro
				flanco_oscuro_detectado = false;
			}
			break;

		case ESTADO_CHEQUEO:
			// Verificación continua durante 5000ms
			if (porcentaje_promedio < UMBRAL_APAGADO) {
				flanco_oscuro_detectado = true;
			}

			// Si detectó oscuridad en el proceso, aborta y vuelve a Oscuridad
			if (flanco_oscuro_detectado) {
				estado_actual = ESTADO_OSCURIDAD;
			}
			// Si pasaron los 5 segundos sin detectar oscuridad
			////else if (count_1ms_Luminaria <= 0) {
			else if ((count_1ms_Luminaria) >= TIME_5_SEC) {
				estado_actual = ESTADO_CLARIDAD;
			}
			break;
	}

}


extern void MEF_Luminaria_task1ms(void) {
	 if (count_1ms_Luminaria) {
		 count_1ms_Luminaria--;
	    }
	 if (timer_100ms) {
		 timer_100ms--;
	 	}

}

