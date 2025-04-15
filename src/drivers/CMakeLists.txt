add_library(drivers
    motor_driver.c
    ultrasonic_sensor.c
    push_button.c
    buzzer.c
)

target_include_directories(drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against pico libraries
target_link_libraries(drivers
    pico_stdlib
    hardware_pwm
    hardware_gpio
    hardware_timer
)
