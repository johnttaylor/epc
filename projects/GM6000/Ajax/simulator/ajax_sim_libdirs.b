# Application
src/Ajax/Main/_plat_simulator

# Comment out this line when building with the House Simulator
src/Ajax/Main/_plat_simulator/_app_platform < app_platform.cpp

# TO Build the simulator using the House simulator for closed
# loop feedback to the heating algorithm.
# 1. Comment out the '... /app_platform < app_platform.cpp' line above
# 2. Uncomment out the following lines
#src/Ajax/Main/_plat_simulator/_app_platform < app_platform_housesim.cpp
#src/Ajax/Heating/Simulated
#src/Ajax/SimHouse

# Simulated UI
src/Driver/TPipe
src/Driver/PicoDisplay/TPipe
src/Driver/Button/TPipe

# Drivers
src/Driver/DIO/Simulated

