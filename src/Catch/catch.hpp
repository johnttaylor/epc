/* This file is use to redirect the Catch2 include to 'external' Catch2 
   package.  This is to simpfly the transition from Catch2 v2 (which was a header
   only framework) to Catch2 v3 (which is a static library). 
*/
#include "catch2/catch_test_macros.hpp"   // Note: Only include the minimal feature set
#include "catch2/catch_session.hpp"

