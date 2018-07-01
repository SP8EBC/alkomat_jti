
/*
 * adc.c
 *
 *  Created on: 29.11.2017
 *      Author: mateusz
 */

#include "drivers/adc.h"
#include "drivers/gpio_conf.h"
#include "aprs/antilib_adc.h"
#include <stm32f10x.h>

void AdcConf(void) {
	Configure_GPIO(GPIOA, GPIO_Pin_1, ANALOG_MODE);

//	ADC1->CR2 |= ADC_CR2_ADON;
//	ADC1->CR2 |= ADC_CR2_CAL;
//	ADC1->SMPR2 |= (ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_2);
//	ADC1->SQR3 |= 0x02;	// ch2
//	ADC1->CR2 |= ADC_CR2_CONT;
//
//	ADC1->CR2 |= ADC_CR2_SWSTART;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_RSTCAL;       				// Reset calibration
    while(ADC1->CR2 & ADC_CR2_RSTCAL);  			        // Wait for reset
    ADC1->CR2 |= ADC_CR2_CAL;          				// Start calibration
    while(ADC1->CR2 & ADC_CR2_CAL);
	ADC1->SQR1 = ADC_SEQUENCE_LENGTH(0);		// odczyt tylko jednego kana�u
	ADC1->SQR3 =  ADC_SEQ1(2);				// wyb�r kana�u ADC -- 11 - napi�cie zasilania
	ADC1->SMPR1 = ADC_SAMPLE_TIME0(SAMPLE_TIME_239_5);	// czas pr�bkowania
//	ADC1->CR1 = ADC_CR1_EOCIE;			/// przerwanie na zako�czenie konwersji
//	NVIC_EnableIRQ(ADC1_2_IRQn);
//	NVIC_SetPriority(ADC1_2_IRQn, 3);
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->DR;
}



