#ifndef COLONY_MAP_H_
#define COLONY_MAP_H_

/// ADC Unit Test mappings
#include "Driver/AIO/Ajax/_0test/halsingleinput_mappings_.h"

/// Null BSP
#include "Bsp/UserSpace/Api.h"


// Cpl::System mappings
#ifdef BUILD_VARIANT_WIN32
#include "Cpl/System/Win32/mappings_.h"
#endif
#ifdef BUILD_VARIANT_CPP11
#include "Cpl/System/Cpp11/_win32/mappings_.h"
#endif

// strapi mapping
#include "Cpl/Text/_mappings/_vc12/strapi.h"



#endif
