# STM32F4 STDPERIPH files.
STM32SRC = 	src/misc.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_adc.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_dma.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_exti.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_flash.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_rcc.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_syscfg.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_tim.c \
			${CHIBIOS}/ext/stdperiph_stm32f4/src/stm32f4xx_wwdg.c

#STM32INC = stdperiph_stm32f4/inc
STM32INC = inc/misc.h

