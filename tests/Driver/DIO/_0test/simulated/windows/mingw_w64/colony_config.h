#ifndef COLONY_CONFIG_H_
#define COLONY_CONFIG_H_

// Enable Trace (but NOT on code coverage builds)
#ifndef BUILD_VARIANT_WIN32
#define USE_CPL_SYSTEM_TRACE
#endif

#define OPTION_DRIVER_DIO_SIMUALTED_PWM_MAX_DUTY_CYCLE  100

#endif
