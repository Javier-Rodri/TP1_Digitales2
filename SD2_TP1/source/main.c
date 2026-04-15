

/*===============================[ Inclusions ]===============================*/
#include "SD2_board.h"
#include "board.h"
#include "MEF_semaforos.h"
#include "MEF_Luminaria.h"
#include "key.h"
#include "cars.h"

/*===================================[ Main ]=================================*/
int main(void) {
    /* Init board hardware */
	board_init();
	key_init();
	ADC_Init();
	/* Reset count of cars */
	count_of_cars_reset();

    /* Init FSL debug console */
    BOARD_InitDebugConsole();

    /* SysTick configuration for 1 ms interruptions */
    SysTick_Config(SystemCoreClock / 1000U);

    /* Init MEF */
    MEF_semaforos_init();
    MEF_Luminaria_init();

    /* Enter an infinite loop */
    while(1) {
    	MEF_semaforos();
    	MEF_Luminaria();
    }

    return 0 ;
}

void SysTick_Handler(void) {
	/* Se leen las entradas de los switches */
	key_periodicTask1ms();

	/* Se descuentan los contadores correspondientes */
	MEF_semaforos_task1ms();

	/* Se descuenta el contador de verificacion de falsas mediciones */
	MEF_Luminaria();
}
