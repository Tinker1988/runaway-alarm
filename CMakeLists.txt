cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(runaway_alarm_clock C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add all source directories
add_subdirectory(src/app)
add_subdirectory(src/common)
add_subdirectory(src/drivers)
add_subdirectory(src/tests)

# Add external dependencies here (if any)
# add_subdirectory(src/external/...)

# Create executable (main is in src/app)
add_executable(runaway_alarm_clock
    src/app/main.c
)

# Link additional modules
target_link_libraries(runaway_alarm_clock
    pico_stdlib
    motor_driver
    deep_sleep
    hardware_pwm
    hardware_gpio
    hardware_timer
)

# Enable USB output, disable UART
pico_enable_stdio_usb(runaway_alarm_clock 1)
pico_enable_stdio_uart(runaway_alarm_clock 0)

# Create UF2
pico_add_extra_outputs(runaway_alarm_clock)
