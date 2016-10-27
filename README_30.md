[서문으로 돌아가기](README.md#howwhat---어떻게-무엇을-개발하고-공유할까)

# study_chibios - understanding the basic structure.

### reading the example
   
이제 프로젝트도 **import** 및 compile이 잘 되고 있으니 조금씩 코드를 분석해야할 차례일 것 입니다.   
현재로써 제일 좋은 예제는 역시 import한 프로젝트에 속해 있는 [main.c](projects/RT-STM32F407-DISCOVERY-01/main.c) 일 것 입니다.  
여기서는 먼저 전체 소스 코드를 살펴 본 후 각 부분에 대해 해설을 적어 보도록 하겠습니다.   
   
```C 
 #include "ch.h"
 #include "hal.h"
 #include "test.h"

 /*
  * This is a periodic thread that does absolutely nothing except flashing
  * a LED.
 */
 static THD_WORKING_AREA(waThread1, 128);
 static THD_FUNCTION(Thread1, arg) {
 
  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */
    chThdSleepMilliseconds(500);
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
    chThdSleepMilliseconds(500);
  }
 }

 /*
  * Application entry point.
 */
 int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      TestThread(&SD2);
    chThdSleepMilliseconds(500);
  }
}
```
  
  
### anatomy of the example
  
먼저 include 부분부터 보도록 하겠습니다.

```C
 #include "ch.h"
 #include "hal.h"
 #include "test.h"
```
  
