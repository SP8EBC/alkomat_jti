//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stm32f10x_rcc.h>
#include "stm32f10x_iwdg.h"
#include "diag/Trace.h"
#include "drivers/serial.h"
//#include "drivers/umb-slave.h"
//#include "drivers/tm_stm32f1_onewire.h"
#include "drivers/dallas.h"
#include "drivers/ms5611.h"
#include "drivers/tx20.h"
#include "drivers/i2c.h"
#include "drivers/adc.h"
#include "TimerConfig.h"
#include "LedConfig.h"

#include "Timer.h"
#include "BlinkLed.h"

#include "alco.h"

#include "aprs/ax25.h"
#include "KissCommunication.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the 'C/C++ General' ->
// 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8 MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//

// Definitions visible only within this translation unit.
namespace
{
  // ----- Timing definitions -------------------------------------------------

  // Keep the LED on for 2/3 of a second.
  constexpr Timer::ticks_t BLINK_ON_TICKS = Timer::FREQUENCY_HZ * 3 / 4;
  constexpr Timer::ticks_t BLINK_OFF_TICKS = Timer::FREQUENCY_HZ
      - BLINK_ON_TICKS;
}

float ds_t = 0.0;
float ms_t = 0.0f;
double ms_p = 0.0;

uint8_t aprs_msg_len;
char aprs_msg[128];

alco Alco;

volatile int i = 0;

uint16_t adc = 0;

uint16_t thr = 2000;

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wempty-body"

int
main(int argc, char* argv[])
{

  NVIC_SetPriorityGrouping(0);

  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %u Hz\n", SystemCoreClock);

  char rsoutput[20];

  RCC->APB1ENR |= (RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM7EN | RCC_APB1ENR_TIM4EN);
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN |
		  	  	  RCC_APB2ENR_IOPBEN |
				  RCC_APB2ENR_IOPCEN |
				  RCC_APB2ENR_IOPDEN |
				  RCC_APB2ENR_AFIOEN |
				  RCC_APB2ENR_TIM1EN |
				  RCC_APB2ENR_TIM1EN |
				  RCC_APB2ENR_ADC1EN );

  memset(aprs_msg, 0x00, 128);


  LedConfig();
  TimerConfig();
  EventTimerConfig();
  DallasInit(GPIOD, GPIO_Pin_2, GPIO_PinSource2);
  SrlConfig();
  AdcConf();	//PA2

  ds_t = DallasQuery();

  ds_t = DallasQuery();

  GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);

  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_128);
  IWDG_SetReload(0xFFF);
  IWDG_Enable();

  thr = 3000;

  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == Bit_RESET) {
	  thr = 1700;
  }
  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) {
	  thr = 2000;
  }
  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) {
	  thr = 2200;
  }
  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_RESET) {
	  thr = 2600;
  }
  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == Bit_RESET) {
	  thr = 2800;
  }

  while (1)
    {
	  ds_t = DallasQuery();
	  adc = ADC1->DR;
	  Alco.push_back(adc);
	  Alco.correction_temperature(ds_t);
	  Alco.calculate_mean();
	  trace_printf("ADC value: %d Scaled Correction: %d \r\n", Alco.getMeanValue(), Alco.getScaledCorrection());

	  if (Alco.getMeanValue() > thr) {
		  GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		  GPIO_SetBits(GPIOC, GPIO_Pin_9);
	  }
	  else {
		  GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		  GPIO_SetBits(GPIOC, GPIO_Pin_8);
	  }


	  IWDG_ReloadCounter();

    }
  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
