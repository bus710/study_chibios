# study_chibios - preface
    
This repo shares my experience in using ChibiOS for personal projects.  
본 리포지토리는 ChibiOS를 개인 프로젝트를 적용하며 겪은 내용을 공유 하고자 합니다.  
  
Since I am not an RTOS expert, the contents can be wrong.  
제가 RTOS 전문가가 아니기 때문에 내용에 오류가 있을 수 있습니다.   
  
Readers should be careful and take responsibility about results in following this guide.  
독자분들은 본 가이드를 적용함에 있어 주의를 요하며 결과들에 대한 책임이 있습니다.   
  
Also it won't be written in English for now.  
또한 당분간은 영어가 아닌 한글로 내용을 작성하고자 합니다.  
  
### Why - 왜 ChibiOS를 쓰게 되었나?  
  
ChibiOS는 ARM의 마이크로 콘트롤러 제품군인 M 시리즈 상에서 사용 가능한 RTOS 입니다.  
  
저는 현재 작은 프로젝트를 진행 중에 있습니다. 파워 휠체어 프로젝트는 지역 로봇 커뮤니티에서 만난 다른 개발자들과 진행 되고 있으며, 그 중 한 명의 가족에게 제공할 전동 휠체어를 개발하는 것이 목적 입니다. ([관련 링크](https://github.com/robomakery/pvc-powerchair))  
처음에는 ROS 위주로 관심이 모였으나, 프로젝트의 특성 상, 로봇 하드웨어를 직접 개발해야 하는 상황이 되었으며, 그 과정에서 [OROCA](http://cafe.naver.com/openrt)와 [Vedderb](http://vedder.se/)를 많이 참고하게 되었습니다. 그 중 Vedderb의 OpenESC 프로젝트가 바로 ChibiOS를 사용하고 있었기에 지난 여름 동안 (2016) 여러가지로 검토하게 되었고, ChibiOS가 stand alone 로봇을 구동하기에 적절함을 확인 할 수 있었습니다. 여러 장단점이 있겠습니다만, 저렴하게 하드웨어와 소프트웨어를 입수하여 진행 할 수 있는 것은 오픈소스 로봇 프로젝트에 있어 큰 메리트가 아닐 수 없을 것 입니다.  
물론 다른 좋은 RTOS가 있을 수 있습니다. 오픈소스와 상용 양 편에서. 그러나 저희 프로젝트가 요구하는 것을 최대한 빠르고 쉽게 진행 할 수 있다는 점에서 큰 반대 없이 적용하기로 결정이 되었습니다. 그렇기에 본 저장소에서 RTOS와 ChibiOS의 장단점은 논의하지 않을 것 입니다. 즉, OS를 평가하는 것은 논외의 영역임을 양해 부탁 드립니다.   
결론적으로, ChibiOS는 기초적인 로봇 하드웨어를 제어함에 있어서 내부적으로는 효율적인 멀티 쓰레드와 API를 제공하고, 외부적으로는 다양한 신호의 입출력을 제공하므로, 채용하게 되었습니다.  
  
### Who - 누구를 위하여?  
  
감히 독자가 생기리라고는 생각지 않습니다만, ChibiOS에 관심을 가지고 도전하고자 하는 분들이 헤매는 일 없이 작업을 진행하는데 도움이 되면 좋겠습니다. 대체로 엔지니어, 메이커, 학생분들이 읽어주시지 않을까 싶으며, 조금 더 특정하자면, baremetal 환경 펌웨어 또는 RTOS를 통해 하드웨어/로봇을 제어하고자 하는 분들을 대상으로 합니다. 특히 ARM의 M 제품군과 전자 회로에 대한 기본적인 이해가 있으시면 좋습니다. (저도 하수 입니다만...)    
비판과 질문은 본 저장소의 issue에 남겨 주시면 최대한 응답하도록 하겠습니다.  
  
### How/What - 어떻게 무엇을 개발하고 공유할까?  
  
일단은 ChibiOS를 입수하고, 컴파일 및 퓨징하는 것부터 시작하고자 합니다. 다음은 기본적인 입출력과 쓰레드를 사용하기 위한 API들을 살펴 본 후, 프로젝트와 관련된 내용을 통해 점차 수준을 높여가보고자 합니다. 프로젝트의 수준이 높아지면, 프로파일링과 함께 상위 소프트웨어와 연동하는 것도 알아 봐야겠습니다.  

| 제목 | 바로가기 |
| :---- | :---- |
| 1. ChibiOS & ChibiStudio    	| [>>>](README_10.md) |  
| 2. 새 프로젝트의 생성       	| [>>>](README_20.md) |  
| 3. 프로젝트 기본 구조의 이해  | [>>>](README_30.md) |  
| 4. 기본 API의 사용          	| [>>>](README_40.md) |  
| 5. 주변 장치를 위한 응용    	| [>>>](README_50.md) |  
| 6. 상위 프로젝트와의 연동   	| [>>>](README_60.md) |  
   
### Requirement - 준비물들  
  
- 먼저 적절한 사양의 PC가 필요 합니다. 저는 윈도우즈 10을 사용하고 있습니다.   
- ChibiOS는 ST의 마이크로 콘트롤러인 STM32F4xxx을 기반으로하는 평가 보드인 STM32F4-Discovery를 기반으로 합니다. 해당 제품은 대략 $20 정도의 비교적 저렴한 가격에 마이크로 콘트롤러, 기본 주변 장치 (전원, 발진 회로, 입출력), 퓨징을 위한 stlink v2를 제공함으로써 빠르고 간편한 시작을 위한 환경을 제공 합니다.   
- FTDI 모듈은 Discovery 평가 보드와 PC를 연결하여 통신을 주고 받기 위해 사용 합니다. 보드 측과 3.3V로 통신하므로, 스펙을 반드시 확인해야 합니다. 
- DC 파워 서플라이, 오실로 스코프, 테스터, 납땜 기구도 필요 합니다.  
- 관련 소프트웨어는 추후에 다루도록 하겠습니다.  
  
### Maintainer - 관리자  

김성준 - [bus710.net](http://bus710.net) <<bus710@gmail.com>>  

### References - 참고자료
  
[1] http://www.chibios.org/dokuwiki/doku.php  
[2] http://vedder.se/2015/01/vesc-open-source-esc/  
[3] http://cafe.naver.com/openrt/8952 (네이버 로그인 필요)   

