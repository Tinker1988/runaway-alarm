add_executable(test_motor_driver
    test_motor_driver.c
    ${PROJECT_SOURCE_DIR}/drivers/motor_driver.c
)

target_include_directories(test_motor_driver PRIVATE
    ${PROJECT_SOURCE_DIR}/drivers
)

target_link_libraries(test_motor_driver pico_stdlib)

pico_add_extra_outputs(test_motor_driver)
