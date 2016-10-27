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

제일 첫 줄에서는 "ch.h" 라이브러리의 사용을 선언하고 있습니다. 해당 파일의 위치는 "C:\ChibiStudio\chibios161\os\rt\include" 입니다. 간단히 파일의 내용을 살펴 보자면 코어 헤더와 서브시스템 헤더가 있는 것을 확인 할 수 있습니다.  
공식 웹사이트에서 열람할 수 있는 "The ultimate guide"에 따르면, rt 디렉토리의 내용은 아키텍쳐와 컴파일러로부터 독립적인, 즉 OS의 로직을 담당하는 코드라는 내용의 해설이 있습니다 [>>>](http://www.chibios.org/dokuwiki/doku.php?id=chibios:book:architecture#the_big_picture). 이를 통해, "ch.h"는 OS에 관련 된 API를 호출하기 위해서 사용하는 것임을 유추할 수 있겠습니다. 또한 예제 코드 내용 중 "ch"라는 접두어로 시작하는 API들이 저 "ch.h"를 통해 선언되고 사용될 수 있음을 짐작할 수 있습니다.   

다음 줄에서는 "hal.h" 라이브러리의 사용을 선언하고 있습니다. 해당 파일의 위치는 "C:\ChibiStudio\chibios161\os\hal\include" 입니다. 역시 간단히 파일의 내용을 살펴 보자면 MCU와 주변 장치에 관련 된 내용을 담고 있는 것을 확인할 수 있습니다.   
HAL은 "Hardware Abstract Layer"의 약자인 것... OS에 관심 있는 분들은 다들 아시리라 생각 합니다. 즉, 다양한 종류의 하드웨어를 일관적인 API로 접근 할 수 있게 하는 레이어에 속한 API를 호출하기 위해 사용하는 것임을 유추할 수 있습니다. 예제 코드 내용 중 "hal"이라는 접두어로 시작하는 API들이 저 "hal.h"를 통해 선언되고 사용됭 수 있음을 짐작할 수 있습니다.   

셋째 줄에서는 "test.h" 라이브러리의 사용을 선언하고 있습니다. 해당 파일의 위치는 "C:\ChibiStudio\chibios161\test\rt" 입니다. 사실, 저 위의 예제에서 "test.h"를 쓰는 부분은 수퍼루프 내에서 선언 된 "TestThread" 뿐 입니다. 해당 함수가 선언된 부분을 확인하면, "test_println" 함수를 통해 RTOS의 환경 변수와 테스크 케이스를 출력하는 것임을 알 수 있습니다. 예제에서는 적극적으로 test 관련 함수를 사용하지 않았지만, 아마도 "test.h"의 용도는 assert 등을 통해 실행 중 로직의 조건을 검사하는 등의 용도로 디버깅을 위해 사용되는 것이라고 여겨집니다. (많은 언어와 플랫폼에서 그렇듯이 말이죠 ^^;)
  
```C
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
```
  
그 다음은 쓰레드를 선언한 부분 입니다. 본 예제의 주석에서는 주기적으로 LED를 반짝이는 용도로만 사용 되었다고 합니다. 말은 간단하지만, 앞으로 쓰레드를 자유자재로 사용하기 위해서는 저 코드들의 내용을 반드시 짚고 넘어가야 하겠습니다.  




