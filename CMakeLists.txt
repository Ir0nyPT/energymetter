# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/external/pico_sdk_import.cmake)
set(PICO_BOARD pico_w)

project(energymetter C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add libraries
add_subdirectory(lib)

# Add executable. Default name is the project name, version 0.1
add_executable(energymetter main.cc)
pico_set_program_name(energymetter "energymetter")
pico_set_program_version(energymetter "0.1")

# Enable USB as uart
pico_enable_stdio_uart(energymetter 0)
pico_enable_stdio_usb(energymetter 1)

# Add the standard library to the build
target_link_libraries(energymetter 
  pico_stdlib
  pico_multicore
  hardware_adc
  hardware_gpio
  hardware_irq
  hardware_pwm
  hardware_rtc
  hardware_timer
  hardware_uart
  pico_cyw43_arch_none
)

# Add any user requested libraries
target_link_libraries(energymetter lib)


target_compile_options(energymetter PRIVATE
  -O3
  -Wall
  -Werror
)

pico_add_extra_outputs(energymetter)
