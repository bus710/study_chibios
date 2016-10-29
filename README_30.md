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
  
먼저 헤더파일들의 사용을 선언하는 include 블럭부터 보도록 하겠습니다.

```C
 #include "ch.h"
 #include "hal.h"
 #include "test.h"
```

제일 첫 줄에서는 **"ch.h"** 라이브러리의 사용을 선언하고 있습니다. 해당 파일의 위치는 **"C:\ChibiStudio\chibios161\os\rt\include"** 입니다. 간단히 파일의 내용을 살펴 보자면 코어 헤더와 서브시스템 헤더가 있는 것을 확인 할 수 있습니다.  
공식 웹사이트에서 열람할 수 있는 **"The ultimate guide"**에 따르면, rt 디렉토리의 내용은 아키텍쳐와 컴파일러로부터 독립적인, 즉 OS의 로직을 담당하는 코드라는 내용의 해설이 있습니다 ([>>>](http://www.chibios.org/dokuwiki/doku.php?id=chibios:book:architecture#the_big_picture)). 이를 통해, **"ch.h"**는 OS에 관련 된 API를 호출하기 위해서 사용하는 것임을 유추할 수 있겠습니다. 또한 예제 코드 내용 중 **"ch"**라는 접두어로 시작하는 API들이 저 **"ch.h"**를 통해 선언되고 사용될 수 있음을 짐작할 수 있습니다.   

다음 줄에서는 "hal.h" 라이브러리의 사용을 선언하고 있습니다. 해당 파일의 위치는 "C:\ChibiStudio\chibios161\os\hal\include" 입니다. 역시 간단히 파일의 내용을 살펴 보자면 MCU와 주변 장치에 관련 된 내용을 담고 있는 것을 확인할 수 있습니다.   
HAL은 "Hardware Abstract Layer"의 약자인 것... OS에 관심 있는 분들은 다들 아시리라 생각 합니다. 즉, 다양한 종류의 하드웨어를 일관적인 API로 접근 할 수 있게 하는 레이어이며, 해당 레이어에 속한 API를 호출하기 위해 사용하는 것임을 유추할 수 있습니다. 예제 코드 내용 중 "hal"이라는 접두어로 시작하는 API들이 저 "hal.h"를 통해 선언되고 사용됭 수 있음을 짐작할 수 있습니다.   

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
  
그 다음은 쓰레드를 선언한 부분 입니다. 본 예제의 주석에서는 주기적으로 LED를 반짝이는 용도로만 사용 되었다고 합니다. 말은 간단하지만, 앞으로의 프로젝트들에서 쓰레드를 자유자재로 사용하기 위해서는 저 코드들의 내용을 반드시 짚고 넘어가야 하겠습니다.  

먼저 "static THD_WORKING_AREA(waThread1, 128);" 입니다. chibios의 쓰레드는 두가지 종류가 있습니다. static과 dynamic으로 분류되며, 이름과 같이 쓰레드의 메모리 영역을 정적 메모리 공간에 만들지 동적 메모리 공간에 만들지에 따라 나뉘어 집니다. 다만, 여기서는 우선 static 쓰레드만을 다루도록 하겠습니다.  
해당 매크로 "THD_WORKING_AREA"는 두가지 인자를 받아 들입니다. "waThread1"은 앞으로 선언 및 실행 될 쓰레드의 이름 입니다. 또한 128은 해당 쓰레드가 사용할 수 있는 메모리의 크기 입니다.   
  
그 아래는 실제 쓰레드 함수의 선언 입니다. "THD_FUNCTION" 매크로의 괄호 안에는 "Thread1"가 적혀 있는데, 이것은 앞으로 이 쓰레드 함수를 지칭하기 위해서는 "Thread1"라는 이름으로 부르라는 것 입니다. 여기서 주의할 점은, 아직은 저 위에서 나온 "waThread1"로 이름붙여진 메모리 공간과 "Threa1"로 이름붙여진 실제 함수의 내용이 연결 되지 않았다는 것 입니다. 

쓰레드 함수의 첫 줄은 "(void)arg;" 입니다. 이것은 사실 c 언어를 사용하며 발생하는 컴파일러의 워닝을 지우기 위한 일종의 꼼수 입니다. 즉, "THD_FUNCTION" 매크로를 사용하면서 반드시 "arg"를 통해 인자를 건네 받아야 하는 규칙이 있는데, 쓰레드 내에서 "arg"를 한번도 사용하지 않는다면 컴파일러가 주의를 줍니다. "너 변수 선언하고 안 쓴거 맞냐?"라고 말 입니다. 이런 워닝을 줄이기 위한 부분 입니다. 만약 나중에 쓰레드 함수를 작성하면서 "arg"를 사용할 일이 생긴다면 저 부분은 지워야 하겠습니다. 

이하의 부분은 쓰레드의 수퍼루프와 GPIO 제어, 그리고 쓰레드의 sleep을 통해 LED를 제어하는 것으로 여겨 집니다. (이 부분은 회로도와 함께 봐야할 부분이지만, 일단은 간단히 넘어가도록 하겠습니다.) GPIO를 제어하기 위해서 "palSetPad" 함수와 "GPIOD", "GPIOD_LED3" 예약어들을 사용했습니다. 해당 함수는 "pal.h" 내에 선언 되어 있습니다. 이클립스의 선언 추적 기능(Open Declaration F3)을 사용하여 확인해 보면, 저 함수가 HAL에 속하며 I/O 포트를 제어하기 위해 사용 되었음을 알 수 있습니다. 첫 인자는 포트명이고 MCU에 종속적인 값 입니다. 두번째 인자는 핀의 이름이고 보드에 종속적인 값 입니다. 각각의 값을 확인하기 위해서는 헤더 파일을 살펴 봐야 하겠지만, 타겟 보드에 따라 파일이 달라지므로 가볍게 패스~~~. 여담이지만 이후에 MCU를 바꾸거나 보드를 새로 개발할 때면 이러한 하드웨어 종속적인 부분을 바꿔주는, 이른 바 "포팅"을 수행해 주어야 할 것 입니다. 
  
  


