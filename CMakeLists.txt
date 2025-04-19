cmake_minimum_required(VERSION 3.13)

# Initialize the Pico SDK
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Project name and version
project(alarmbot C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

set(SOURCES
  main.c
  src/drivers/buzzer.c
  src/drivers/push_button.c
  src/drivers/motor_driver.c
  src/drivers/ultrasonic_sensor.c
  src/app/motion_logic.c
  src/app/state_machine.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers
  ${CMAKE_CURRENT_LIST_DIR}/src/app
)

# link to libs
target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  hardware_pwm
  hardware_gpio
  pico_time
)

pico_add_extra_outputs(${PROJECT_NAME})
