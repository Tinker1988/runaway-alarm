add_executable(alarm_robot
    main.c
    motion_logic.c
    state_machine.c
)

# Include directories
target_include_directories(alarm_robot PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/drivers
)

# Link against libraries
target_link_libraries(alarm_robot
    drivers
    pico_stdlib
    hardware_pwm
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(alarm_robot 1)
pico_enable_stdio_uart(alarm_robot 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(alarm_robot)
