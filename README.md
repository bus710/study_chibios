# study_chibios
This repo shares my experience in using ChibiOS for personal projects.
본 리포지토리는 ChibiOS를 개인 프로젝트를 적용하며 겪은 내용을 공유 합니다.

Since I am not a RTOS expert, the contents can be wrong. Also it won't be written in English for now.
제가 RTOS 전문가가 아니기 때문에 내용에 오류가 있을 수 있습니다. 또한 당분간은 영어가 아닌 한글로 내용을 작성하고자 합니다.

h3. Why - 왜 ChibiOS를 쓰게 되었나?
관심 있는 분은 없으시겠지만, 저는 현재 작은 프로젝트를 진행 중에 있습니다. 파워 휠체어 프로젝트는 지역 로봇 커뮤니티에서 만난 다른 개발자들과 진행 되고 있으며, 그 중 한 명의 가족에게 제공할 전동 휠체어를 개발하는 것이 목적 입니다. [관련 링크](https://github.com/robomakery/pvc-powerchair)
처음에는 ROS 위주로 관심이 모였으나, 프로젝트의 특성 상, 로봇 하드웨어를 직접 개발해야 하는 상황이 되었으며, 그 과정에서 OROCA와 Vedderb를 많이 참고하게 되었습니다. 그 중 Vedderb의 OpenESC 프로젝트가 바로 ChibiOS를 사용하고 있었기에 지난 여름 동안 (2016) 여러가지로 검토하게 되었고, ChibiOS가 stand alone 로봇을 구동하기에 적절함을 확인 할 수 있었습니다. 여러 장단점이 있겠습니다만, 저렴하게 하드웨어와 소프트웨어를 입수하여 진행 할 수 있는 것은 오픈소스 로봇 프로젝트에 있어 큰 메리트가 아닐 수 없을 것 입니다.
물론 다른 좋은 RTOS가 있을 수 있습니다. 오픈소스와 상용 양 편에서. 그러나 저희 프로젝트가 요구하는 것을 최대한 빠르고 쉽게 진행 할 수 있다는 점에서 큰 반대 없이 적용하기로 결정이 되었습니다. 또한 본 저장소에서 RTOS의 장단점이나 ChibiOS의 장단점은 논의하지 않을 것이며, OS를 평가하는 것도 논외의 영역임을 양해 부탁 드립니다. 
결론적으로, ChibiOS는 기초적인 로봇 하드웨어를 제어함에 있어서 내부적으로는 효율적인 멀티 쓰레드와 API를 제공하고, 외부적으로는 다양한 신호의 입출력을 제공하므로, 채용하게 되었습니다.

h3. How/What - 어떻게 무엇을 개발하고 공유할까?
