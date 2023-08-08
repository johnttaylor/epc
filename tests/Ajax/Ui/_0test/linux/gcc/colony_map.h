#ifndef COLONY_MAP_H_
#define COLONY_MAP_H_

// Cpl::System mappings
#if defined(BUILD_VARIANT_POSIX) || defined(BUILD_VARIANT_POSIX64)
#include "Cpl/System/Posix/mappings_.h"
#endif
#ifdef BUILD_VARIANT_CPP11
#include "Cpl/System/Cpp11/_posix/mappings_.h"
#endif

// strapi mapping
#include "Cpl/Text/_mappings/_posix/strapi.h"

#include "Ajax/Ui/Events.h"
#define AjaxScreenMgrEvent_T_MAP        AjaxUiEvent_T

#include <stdint.h>
#define Driver_Button_Pin_Hal_T_MAP     uint32_t  

/// Unit test mock impl
#define Driver_Button_Hal_getRawPressedState_MAP    mocked_getRawPressState
bool mocked_getRawPressState( uint32_t pinHandle );




#endif

