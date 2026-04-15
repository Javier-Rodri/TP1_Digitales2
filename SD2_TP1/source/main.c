

/*===============================[ Inclusions ]===============================*/
#include "SD2_board.h"
#include "board.h"
#include "MEF_semaforos.h"
#include "MEF_Luminaria.h"
#include "sensor_luz.h"
#include "key.h"
#include "cars.h"

/*===================================[ Main ]=================================*/
int main(void) {
    /* Init board hardware */
	board_init();
	key_init();
	sensorLuz_init();
	/* Reset count of cars */
	count_of_cars_reset();

    /* Init FSL debug console */
    BOARD_InitDebugConsole();

    /* SysTick configuration for 1 ms interruptions */
    SysTick_Config(SystemCoreClock / 1000U);

    /* Init MEF */
    MEF_semaforos_init();
    MEF_Luminaria_Init();

    /* Enter an infinite loop */
    while(1) {
    	MEF_semaforos();
    	MEF_Luminaria_Task();
    }

    return 0 ;
}

void SysTick_Handler(void) {

	// Este contador es 'static' para que no se borre su valor
	// cada vez que termina la función.
	static uint16_t divisor_adc = 100;

	/* Se leen las entradas de los switches */
	key_periodicTask1ms();

	/* Se descuentan los contadores correspondientes */
	MEF_semaforos_task1ms();

	// Tick para el sensor (dispara la lectura cada 100ms)
	//sensorLuz_tick1ms();

	// Tick para la MEF (descuenta los 5 segundos de chequeo)
	MEF_Luminaria_tick();


	// 2. Esto se hace CADA 100ms
	    if (divisor_adc > 0) {
	        // Si no llegamos a 100, solo restamos 1
	        divisor_adc--;
	    } else {
	        // ¡Llegamos! Pasaron 100 entradas de 1ms = 100ms
	        divisor_adc = 100; // Reseteamos el contador

	        // ACÁ damos la orden al ADC.
	        // Solo ocurre 1 vez cada 100ms.
	        sensorLuz_dispararLectura();
	    }
}
