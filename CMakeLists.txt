# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_CXX_ENABLE_RTTI 1)

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
pico_set_program_version(tr-pll "3.0")

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

pico_generate_pio_header(tr-pll ${CMAKE_CURRENT_LIST_DIR}/rotaryEncoder.pio)

add_subdirectory(pico-ssd1306) # OLED
target_link_libraries(tr-pll pico_ssd1306 hardware_i2c)

add_library(settings settings.cpp)
target_link_libraries(settings pico_stdlib hardware_i2c hardware_spi hardware_pio hardware_flash)

add_library(I2Cinput I2Cinput.cpp)
target_link_libraries(I2Cinput settings pico_stdlib hardware_i2c hardware_spi hardware_pio hardware_timer)

add_library(TrxState TrxState.cpp)
target_link_libraries(TrxState pico_stdlib hardware_i2c settings hardware_spi hardware_pio I2Cinput memory)

add_library(TrxStateVfo TrxStateVfo.cpp)
target_link_libraries(TrxStateVfo TrxState pico_stdlib hardware_i2c hardware_spi hardware_pio)

add_library(memory memory.cpp)
target_link_libraries(memory settings pico_stdlib hardware_flash hardware_sync)

add_library(GPIOoutput GPIOoutput.cpp)
target_link_libraries(GPIOoutput settings TrxState pico_stdlib hardware_timer hardware_pio hardware_clocks)
pico_generate_pio_header(GPIOoutput ${CMAKE_CURRENT_LIST_DIR}/beep.pio)
pico_generate_pio_header(tr-pll ${CMAKE_CURRENT_LIST_DIR}/ctcss.pio)

add_library(GPIOinput GPIOinput.cpp)
target_link_libraries(GPIOinput settings pico_stdlib hardware_spi hardware_pio GPIOoutput)

add_library(MCP4725 MCP4725.cpp)
target_link_libraries(MCP4725 settings pico_stdlib hardware_i2c hardware_spi hardware_pio)

add_library(ADF4351 ADF4351.cpp)
target_link_libraries(ADF4351 settings pico_stdlib hardware_spi hardware_pio)

add_library(TrxStateSpecialMemory TrxStateSpecialMemory.cpp)
target_link_libraries(TrxStateSpecialMemory TrxState)

add_library(TrxStateScanMin TrxStateScanMin.cpp)
target_link_libraries(TrxStateScanMin pico_stdlib TrxStateSpecialMemory settings)

add_library(TrxStateScanMax TrxStateScanMax.cpp)
target_link_libraries(TrxStateScanMax pico_stdlib TrxStateSpecialMemory settings)

add_library(TrxStateMemories TrxStateMemories.cpp)
target_link_libraries(TrxStateMemories pico_stdlib TrxState memory settings GPIOoutput)

add_library(Display Display.cpp)
target_link_libraries(Display pico_stdlib pico_ssd1306 hardware_i2c hardware_spi hardware_pio TrxStateVfo TrxStateSpecialMemory TrxStateScanMin TrxStateScanMax Scanner)

add_library(Scanner Scanner.cpp)
target_link_libraries(Scanner pico_stdlib TrxState GPIOinput GPIOoutput TrxStateScanMin TrxStateScanMax memory hardware_timer)

# Add any user requested libraries
target_link_libraries(tr-pll
        pico_stdlib
        hardware_i2c
        hardware_spi 
        hardware_pio
        TrxState
        TrxStateVfo
        Display
        GPIOinput
        GPIOoutput
        I2Cinput
        settings
        MCP4725
        ADF4351
        Scanner
        TrxStateScanMin
        )

pico_add_extra_outputs(tr-pll)

