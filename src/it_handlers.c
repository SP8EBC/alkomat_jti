/*
 * it_handlers.c
 *
 *  Created on: 25.06.2017
 *      Author: mateusz
 */

#include <stm32f10x.h>
#include "main.h"
#include "diag/Trace.h"
#include "drivers/dallas.h"
#include "drivers/ms5611.h"
#include "drivers/serial.h"
#include "drivers/tx20.h"
//#include "drivers/umb-slave.h"
#include "KissCommunication.h"
#include <stdio.h>

static char s = -1;
static char wx_freq = 3, wxi = 2;

//extern UmbMeteoData u;

void TIM3_IRQHandler(void) {


	TIM3->SR &= ~(1<<0);
	ds_t = DallasQuery();
	//u.temperature = (char)ds_t;
}

void TIM4_IRQHandler(void) {
	TIM4->SR &= ~(1<<0);
	if (wxi++ == wx_freq){


		wxi = 1;
	}
	else;

}
