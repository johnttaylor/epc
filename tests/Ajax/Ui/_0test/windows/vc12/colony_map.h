#ifndef COLONY_MAP_H_
#define COLONY_MAP_H_

// Cpl::System mappings
#ifdef BUILD_VARIANT_WIN32
#include "Cpl/System/Win32/mappings_.h"
#endif
#ifdef BUILD_VARIANT_CPP11
#include "Cpl/System/Cpp11/_win32/mappings_.h"
#endif

// strapi mapping
#include "Cpl/Text/_mappings/_vc12/strapi.h"

#include "Ajax/Ui/Events.h"
#define AjaxScreenMgrEvent_T_MAP        AjaxUiEvent_T

#define Driver_Button_Pin_Hal_T_MAP     uint32_t  

/// Unit test mock impl
#define Driver_Button_Hal_getRawPressedState_MAP    mocked_getRawPressState
bool mocked_getRawPressState( uint32_t pinHandle );


#endif
