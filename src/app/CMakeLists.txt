cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(runaway_alarm C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files here
add_executable(runaway_alarm
    main.c
    state_machine.c
    alarm.c
    motion_logic.c
    motor_driver.c
    ultrasonic_sensor.c
    deep_sleep.c
)

# Include directories if needed
target_include_directories(runaway_alarm PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link with the Pico SDK
target_link_libraries(runaway_alarm pico_stdlib)

# Enable USB output, UART output, etc.
pico_enable_stdio_usb(runaway_alarm 1)
pico_enable_stdio_uart(runaway_alarm 0)

# Create UF2 file
pico_add_extra_outputs(runaway_alarm)
