/*
	Copyright 2012-2015 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "ch.h"
#include "hal.h"
#include "stm32f4xx_conf.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "hw.h"

#define LED_GREEN			0
#define LED_RED				1

static const uint8_t message[] = "aa";

/*
 * Timers used:
 * TIM1: mcpwm
 * TIM2: mcpwm
 * TIM12: mcpwm
 * TIM8: mcpwm
 *
 * DMA/stream	Device		Function
 * 1, 1			UART3		HW_R2
 * 1, 3			UART3		HW_R2
 * 2, 2			UART6		Other HW
 * 2, 7			UART6		Other HW
 * 2, 4			ADC			mcpwm
 *
 */

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
  //palClearPad(GPIOD, GPIOD_LED5);
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
  //palSetPad(GPIOD, GPIOD_LED5);
}

static PWMConfig pwmcfg = {
  8000000,                           /* 2MHz PWM clock frequency.   */
  40,                                /* Initial PWM period 0.000005S.       */
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};


static THD_WORKING_AREA(led_thread_wa, 128);
static THD_WORKING_AREA(pwm_thread_wa, 128);

static void run(void){
  int mdelay = 5;

  for(int i=0; i<30; i++){
    // step 1
    palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(1));  //H1
    palSetPadMode(GPIOA,  9, PAL_MODE_UNCONNECTED);   //H2
    palSetPadMode(GPIOA,  8, PAL_MODE_UNCONNECTED);   //H3
    palSetPadMode(GPIOB, 15, PAL_MODE_UNCONNECTED);   //L1
    palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(1));  //L2
    palSetPadMode(GPIOB, 13, PAL_MODE_UNCONNECTED);   //L3
    chThdSleepMilliseconds(mdelay);

    // step 6
    palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(1));  //H1
    palSetPadMode(GPIOA,  9, PAL_MODE_UNCONNECTED);   //H2
    palSetPadMode(GPIOA,  8, PAL_MODE_UNCONNECTED);   //H3
    palSetPadMode(GPIOB, 15, PAL_MODE_UNCONNECTED);   //L1
    palSetPadMode(GPIOB, 14, PAL_MODE_UNCONNECTED);   //L2
    palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(1));  //L3
    chThdSleepMilliseconds(mdelay);

    // step 5
    palSetPadMode(GPIOA, 10, PAL_MODE_UNCONNECTED);   //H1
    palSetPadMode(GPIOA,  9, PAL_MODE_ALTERNATE(1));  //H2
    palSetPadMode(GPIOA,  8, PAL_MODE_UNCONNECTED);   //H3
    palSetPadMode(GPIOB, 15, PAL_MODE_UNCONNECTED);   //L1
    palSetPadMode(GPIOB, 14, PAL_MODE_UNCONNECTED);   //L2
    palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(1));  //L3
    chThdSleepMilliseconds(mdelay);

    // step 4
    palSetPadMode(GPIOA, 10, PAL_MODE_UNCONNECTED);   //H1
    palSetPadMode(GPIOA,  9, PAL_MODE_ALTERNATE(1));  //H2
    palSetPadMode(GPIOA,  8, PAL_MODE_UNCONNECTED);   //H3
    palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(1));  //L1
    palSetPadMode(GPIOB, 14, PAL_MODE_UNCONNECTED);   //L2
    palSetPadMode(GPIOB, 13, PAL_MODE_UNCONNECTED);   //L3
    chThdSleepMilliseconds(mdelay);

    // step 3
    palSetPadMode(GPIOA, 10, PAL_MODE_UNCONNECTED);   //H1
    palSetPadMode(GPIOA,  9, PAL_MODE_UNCONNECTED);   //H2
    palSetPadMode(GPIOA,  8, PAL_MODE_ALTERNATE(1));  //H3
    palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(1));  //L1
    palSetPadMode(GPIOB, 14, PAL_MODE_UNCONNECTED);   //L2
    palSetPadMode(GPIOB, 13, PAL_MODE_UNCONNECTED);   //L3
    chThdSleepMilliseconds(mdelay);

    // step 2
    palSetPadMode(GPIOA, 10, PAL_MODE_UNCONNECTED);   //H1
    palSetPadMode(GPIOA,  9, PAL_MODE_UNCONNECTED);   //H2
    palSetPadMode(GPIOA,  8, PAL_MODE_ALTERNATE(1));  //H3
    palSetPadMode(GPIOB, 15, PAL_MODE_UNCONNECTED);   //L1
    palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(1));  //L2
    palSetPadMode(GPIOB, 13, PAL_MODE_UNCONNECTED);   //L3
    chThdSleepMilliseconds(mdelay);
  }

  // stop
  palSetPadMode(GPIOA, 10, PAL_MODE_UNCONNECTED);   //H1
  palSetPadMode(GPIOA,  9, PAL_MODE_UNCONNECTED);   //H2
  palSetPadMode(GPIOA,  8, PAL_MODE_UNCONNECTED);   //H3
  palSetPadMode(GPIOB, 15, PAL_MODE_UNCONNECTED);   //L1
  palSetPadMode(GPIOB, 14, PAL_MODE_UNCONNECTED);   //L2
  palSetPadMode(GPIOB, 13, PAL_MODE_UNCONNECTED);   //L3
  chThdSleepMilliseconds(50);
}

static THD_FUNCTION(pwm_thread, arg) {
  (void) arg;

  chRegSetThreadName("pwm_gen");

  //DISABLE_GATE();
  ENABLE_GATE();

  pwmStart(&PWMD1, &pwmcfg);
  pwmEnablePeriodicNotification(&PWMD1);

  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); // 25%
  pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); // 25%
  pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); // 25%
  //pwmEnableChannelNotification(&PWMD1, 0);

  // stop
  palSetPadMode(GPIOA, 10, PAL_MODE_UNCONNECTED);   //H1
  palSetPadMode(GPIOA,  9, PAL_MODE_UNCONNECTED);   //H2
  palSetPadMode(GPIOA,  8, PAL_MODE_UNCONNECTED);   //H3
  palSetPadMode(GPIOB, 15, PAL_MODE_UNCONNECTED);   //L1
  palSetPadMode(GPIOB, 14, PAL_MODE_UNCONNECTED);   //L2
  palSetPadMode(GPIOB, 13, PAL_MODE_UNCONNECTED);   //L3
  chThdSleepMilliseconds(50);

  for(int i=0; i<10; i++){
    run();
    chThdSleepMilliseconds(300);
  }

  for(;;){

  }
}

static THD_FUNCTION(led_thread, arg) {
	(void)arg;

	chRegSetThreadName("blinker");

	LED_GREEN_ON();

	for(;;) {
        LED_GREEN_OFF();
		chThdSleepMilliseconds(500);
        LED_GREEN_ON();
		chThdSleepMilliseconds(500);
	}
}

static void txend1(UARTDriver *uartp){
  (void)uartp;
}

static void txend2(UARTDriver *uartp){
  (void)uartp;
}

static void rxerr(UARTDriver *uartp, uartflags_t e){
  (void)uartp;
  (void)e;
}

static void rxchar(UARTDriver *uartp, uint16_t c){
  (void)uartp;
  (void)c;
}

static void rxend(UARTDriver *uartp){
  (void)uartp;
}

static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  19200,
  0,
  USART_CR2_LINEN,
  0
};

int main(void) {
	halInit();
	chSysInit();

	chThdSleepMilliseconds(100);

	hw_init_gpio();
	LED_RED_OFF();
	LED_GREEN_OFF();

	chThdCreateStatic(led_thread_wa, sizeof(led_thread_wa), NORMALPRIO, led_thread, NULL);
	chThdCreateStatic(pwm_thread_wa, sizeof(pwm_thread_wa), NORMALPRIO, pwm_thread, NULL);

	palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(8)); // UART TX
	palSetPadMode(GPIOC, 7, PAL_MODE_ALTERNATE(8)); // UART RX

	uartStart(&UARTD6, &uart_cfg_1);

	for(;;) {
	    uartStartSend(&UARTD6, 2, message);
		chThdSleepMilliseconds(500);
	}
}
