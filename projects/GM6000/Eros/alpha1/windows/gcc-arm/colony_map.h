// Drivers
#include "Driver/DIO/STM32/mappings_.h"
#include "Driver/Button/STM32/mappings_.h"
#include "Driver/AIO/STM32/singleinput_mappings_.h"

// OSAL mapping
#include "Cpl/System/FreeRTOS/mappings_.h"

// strapi mapping
#include "Cpl/Text/_mappings/_arm_gcc_stm32/strapi.h"

#include "Ajax/Ui/Events.h"
#define AjaxScreenMgrEvent_T_MAP        AjaxUiEvent_T

// My BSP
#include "Bsp/Initech/alpha1/Api.h"

