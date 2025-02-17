# Code under test
../../../libdirs.b

# drivers
src/Driver/DIO/STM32 < Pwm.cpp In.cpp

# Application Specific OSAL
src/Cpl/System/FreeRTOS/_fatalerror
src/Cpl/System/_trace
src/Cpl/Io/Stdio/_ansi

# Output stream for the trace engine (and the debug console)
src/Cpl/Io/Serial/ST/M32F4

# CPL OSAL
src/Cpl/System
src/Cpl/System/FreeRTOS
src/Cpl/System/_assert


# BSP
src/Bsp/Initech/alpha1/trace
src/Bsp/Initech/alpha1 
src/Bsp/Initech/alpha1/MX 
src/Bsp/Initech/alpha1/MX/Core/Src > freertos.c
src/Bsp/Initech/alpha1/console

# SEGGER SysVIEW
src/Bsp/Initech/alpha1/SeggerSysView

# SDK
xsrc/stm32F4-SDK/Drivers/STM32F4xx_HAL_Driver/Src > stm32f4xx_hal_timebase_rtc_alarm_template.c stm32f4xx_hal_timebase_rtc_wakeup_template.c stm32f4xx_hal_timebase_tim_template.c

# FreeRTOS
xsrc/freertos
xsrc/freertos/portable/GCC/ARM_CM4F
