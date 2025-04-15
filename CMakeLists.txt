cmake_minimum_required(VERSION 3.13)

# Initialize the Pico SDK
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Project name and version
project(alarm_robot VERSION 1.0.0 LANGUAGES C CXX ASM)

# Initialize the Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src/drivers)
add_subdirectory(src/app)
