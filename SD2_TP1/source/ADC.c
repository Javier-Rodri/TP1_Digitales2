#include "ADC.h"
#include "fsl_adc16.h"  // La librería del SDK

void ADC_Init(void) {
    adc16_config_t adc16ConfigStruct;
    ADC16_GetDefaultConfig(&adc16ConfigStruct);

    // 12 bits y reloj más lento para mayor estabilidad y evitar brillo tenue por ruido
    adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
    adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;

    ADC16_Init(ADC0, &adc16ConfigStruct);

    // Obligatorio para KL46Z: Calibración
    if (kStatus_Success != ADC16_DoAutoCalibration(ADC0)) {
        // Si falla la calibración, el ADC podría no funcionar bien
    }
}
uint32_t ADC_Leer_Luz(void) {
    adc16_channel_config_t adc16ChannelConfigStruct;

    /* Configurar el canal 3 (donde está el sensor de luz en la KL46) */
    adc16ChannelConfigStruct.channelNumber = 3U;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif

    /* Disparar la conversión */
    ADC16_SetChannelConfig(ADC0, 0U, &adc16ChannelConfigStruct);

    /* Esperar a que el hardware termine de leer (Polling) */
    while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, 0U)));

    /* Leer y devolver el resultado */
    return ADC16_GetChannelConversionValue(ADC0, 0U);
}
