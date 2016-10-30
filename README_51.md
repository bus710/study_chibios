[서문으로 돌아가기](README.md#howwhat---어떻게-무엇을-개발하고-공유할까)

# study_chibios - understanding Vedderb-OpenESC  
   
### background
  
서문에서 적은 바와 같이 저희 팀이 지향하는 것은 ChibiOS를 이용하여 BLDC 모터를 제어하는 것 입니다. 이 프로젝트 자체가 Vedder Bejamin의 OpenESC 프로젝트에 영향을 많이 받았으므로 OpenESC를 분석함으로써 우리가 필요로 하는 기능을 구현하는 것은 자연스러운 일이라 할 것 입니다.   

OpenESC에 대한 자세한 내용은 Vedder의 블로그인 [vedder.se](http://vedder.se/2015/01/vesc-open-source-esc/)에 소개 되어 있습니다. 간단히 요약하자면 아래와 같이 구성 되어 있습니다.
  
- ChibiOS 3.0.2 기반 (2016년 10월 현재)
- STM32F40x를 콘트롤러로 사용
- MOSFET으로 출력 되는 3 phase 신호로 BLDC 모터를 제어 (톱니파, 삼각파, FOC 제어 가능)
- BLDC 모터에서 제공하는 hall sensor와 온도 센서를 통해 closed-loop 제어 가능
- MOSFET의 제어를 위해 TI의 DRV8302 IC를 사용하며, 해당 칩은 주 전원 입력 (20~60Vdc)을 강하하여 디지털 회로가 필요로 하는 전원 (5Vdc)을 제공함 (실제로 MCU에 공급 되는 전원은 TC2117을 통해 3.3V로 다시 강하됨)
- 다양한 통신 기능이 제공 됨 (UART, CAN, Nrf, USB..)
- 리눅스 상의 응용 프로그램이 제공 됨
  
회로도, PCB 아트웍, 펌웨어 등을 비롯한 모든 정보가 github에 공개 되어 있으므로([>>>](https://github.com/vedderb)), 평가 보드를 입수하지 못 하더라도 충분히 좋은 교재가 될 수 있으리라고 생각 합니다.  
  
![images/200.png](images/200.png)  
  
### downloading F/W
  
저희가 현재 교재로 삼고 있는 커밋 버전은 **"1655c11"**이며 제 계정에 fork 해 놓은 상태 입니다 ([>>>](https://github.com/bus710/bldc)). 따라서 해당 저장소를 클론하여 폴더를 열어보면 약 70개에 가까운 파일들(폴더, 헤더, c코드, makefile...)이 있음을 알 수 있습니다. 일단은 이클립스에서 해당 프로젝트를 열어 컴파일을 해보도록 하겠습니다. 
  
이를 위해서는 이클립스에서 **"File >> New >> Makefile Project with Existing Code"**를 실행합니다. 
  
![images/201.png](images/201.png)  
  
이후 표시 되는 다이얼로그에서 **Browse**를 클릭하고, 해당 프로젝트를 열어 줍니다. 
  
![images/202.png](images/202.png)  

다음으로는 여타 ChibiOS 프로젝트와 마찬가지로 컴파일(Project >> Build All (^B))을 수행 해주면, 정상적으로 컴파일 된 것을 확인할 수 있을 것 입니다.  
  
### anatomy OpenESC  
  
컴파일이 가능한 OpenESC 프로젝트를 입수하였으니, 이제 프로젝트의 구조를 파악 해야할 것 입니다.
  
먼저, 아래의 리스트는 ChibiOS의 어플리케이션을 위한 기본적인 파일들 입니다. 

| 번호 	| 파일명 		| 설명 | 
| :----	| :---- 		| :---- |
| 1		| .cproject		|	|
| 2		| .project		|	| 
| 3		| chconf.h		|	| 
| 4		| halconf.h		| 	|
| 5		| main.c		|	|
| 6		| Makefile		|	|
| 7		| mcuconf.h		| 	|
| 8		| readme.txt	|	|

아래의 리스트는 OpenESC에 속한 모든 파일들 입니다. 즉, 위의 파일들을 기본으로 삼아 OpenESC를 위해 작성 된 파일들인 것 입니다.  

| 번호	| 파일명 				| 설명 		| 
| :----	| :---- 				| :---- 	|
| 1		| appconf 				| 폴더,  	|
| 2		| application 			| 폴더,  	|
| 3		| ChibiOS_3.0.2			| 폴더,  	|
| 4		| docs 					| 폴더,  	|
| 5		| hwconf 				| 폴더,  	|
| 6		| mcconf				| 폴더,  	|
| 7		| nrf					| 폴더,  	|
| 8		| .cproject				| 			|
| 9		| .project				|			|
| 10	| buffer.c/h			|			|
| 11	| chconf.h				|			|
| 12	| comm_can.c/h			|			|
| 13	| comm_usb.c/h			|			|
| 14	| comm_usb_serial.c/h	|			|
| 15	| commands.c/h			|			|
| 16	| conf_general.c/h		|			|
| 17	| crc.c/h				|			|
| 18	| datatypes.h			|			|
| 19	| digital_filter.c/h	|			|
| 20	| eeprom.c/h			|			|
| 21	| encoder.c/h			|	|
| 22	| flash_helper.c/h		|	|
| 23	| halconf.h				|	|
| 24	| irq_hadlers.c			|	|
| 25	| isr_vector_table.h	|	|
| 26	| ld_eeprom_emu.ld		|	|
| 27	| led_external.c/h		|	|
| 28	| ledpwm.c/h			|	|
| 29	| main.c				|	|
| 30	| Makefile				|	|
| 31	| mc_interface.c/h		|	|
| 32	| mcpwm.c/h				|	|
| 33	| mcpwm_foc.c/h			|	|
| 34	| mcuconf.h				|	|
| 35	| packet.c/h			|	|
| 36	| servo.c/h				|	|
| 37	| servo_dec.c/h			|	|
| 38	| servo_simple.c/h		|	|
| 39	| stm32-bv_openocd.cfg	|	|
| 40	| stm32f4xx_conf.h		|	|
| 41	| terminal.c/h			|	|
| 42	| timeout.c/h			|	|
| 43	| utils.c/h				|	|
| 44	| ws2811.c/h			|	|

저 두번째 리스트에 속한 파일들을 복사하여 **projects_esc/p001**에 붙여 넣었습니다. 이제부터는 필요한 첫번째 기능을 p001에 넣고, 두번째 기능이 추가 되면 p002에 복사해 넣도록 하겠습니다. 즉, p001은 p002의 하위 호환이 되는 셈 입니다.  
