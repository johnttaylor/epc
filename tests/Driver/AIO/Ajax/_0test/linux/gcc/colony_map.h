#ifndef COLONY_MAP_H_
#define COLONY_MAP_H_

/// ADC Unit Test mappings
#include "Driver/AIO/Ajax/_0test/halsingleinput_mappings_.h"


// Cpl::System mappings
#if defined(BUILD_VARIANT_POSIX) || defined(BUILD_VARIANT_POSIX64)
#include "Cpl/System/Posix/mappings_.h"
#endif
#ifdef BUILD_VARIANT_CPP11
#include "Cpl/System/Cpp11/_posix/mappings_.h"
#endif

// strapi mapping
#include "Cpl/Text/_mappings/_posix/strapi.h"


#endif

