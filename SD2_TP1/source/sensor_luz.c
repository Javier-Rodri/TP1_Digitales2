#include "fsl_adc16.h"
#include "sensor_luz.h"
#include "MKL46Z4.h"
#include "SD2_board.h"

static volatile uint32_t Valor_visualizar = 0;
static volatile uint32_t acumulador = 0;
static volatile uint8_t contador = 0;
static volatile float Valor_promedio = 0.0f;

void sensorLuz_init(void) {

		SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

//	    // Pin 18: Salida fuerte para el LED
//	    PORTE->PCR[18] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
//	    PTE->PDDR |= (1 << 18);

	    // Pin 22: Entrada pura para el ADC (Mux 0 es analógico)
	    PORTE->PCR[22] = PORT_PCR_MUX(0);


    // 1. Habilitar relojes para Puerto E y Puerto D
	    //    SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK);
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

//    // 2. Configurar LED Rojo (PTE18) como salida con Drive Strength (DSE)
//    // Esto es lo que saca el LED del estado "tenue"
//    PORTE->PCR[18] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
//    PTE->PDDR |= (1 << 18);  // Dirección: Salida
//    PTE->PSOR = (1 << 18);   // Arranca apagado (Lógica negativa: High = Off)

    // 3. Configuración del ADC (la que ya te funcionaba)
    adc16_config_t config;
    ADC16_GetDefaultConfig(&config);
    config.clockSource = kADC16_ClockSourceAsynchronousClock;
    config.resolution = kADC16_ResolutionSE12Bit;

    ADC16_Init(ADC0, &config);
    ADC16_DoAutoCalibration(ADC0);

    // 4. Habilitar interrupción en el micro
    EnableIRQ(ADC0_IRQn);
}

void sensorLuz_dispararLectura(void) {
    adc16_channel_config_t chConfig;
    chConfig.channelNumber = 3U; // Canal 3 = Sensor de luz en KL46
    chConfig.enableInterruptOnConversionCompleted = true;

    ADC16_SetChannelConfig(ADC0, 0U, &chConfig);
}

void ADC0_IRQHandler(void) {
    uint32_t valor = ADC16_GetChannelConversionValue(ADC0, 0U);
    Valor_visualizar= valor;
    acumulador += valor;
    contador++;

    if (contador >= 5) {
    	Valor_promedio = (((float)acumulador / 65000.0f) / 5.0f)* 100.0f;
        //Valor_promedio = (((float)acumulador / 5.0f) * 100.0f) / 4095.0f;
        acumulador = 0;
        contador = 0;
    }
}

float sensorLuz_obtenerValor(void) {
    return Valor_promedio;
}
