# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/michael/pico/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(tr-pll C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(tr-pll main.cpp )

pico_set_program_name(tr-pll "tr-pll")
pico_set_program_version(tr-pll "0.1")

#pico_enable_stdio_uart(tr-pll 0)
#pico_enable_stdio_usb(tr-pll 0)

# Add the standard library to the build
target_link_libraries(tr-pll
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(tr-pll PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)


# OLED
add_subdirectory(pico-ssd1306)
target_link_libraries(tr-pll pico_ssd1306 hardware_i2c)

add_library(TrxState TrxState.cpp)
target_link_libraries(TrxState pico_stdlib)


# Add any user requested libraries
target_link_libraries(tr-pll
        hardware_i2c
        hardware_pio
        hardware_timer
        TrxState
        )

pico_add_extra_outputs(tr-pll)

