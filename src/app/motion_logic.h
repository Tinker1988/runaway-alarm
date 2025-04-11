#ifndef MOTION_LOGIC_H
#define MOTION_LOGIC_H

#include "motor_driver.h"
#include "ultrasonic_sensor.h"

void motion_logic_update(ultrasonic_sensor_t *left_sensor,
                         ultrasonic_sensor_t *right_sensor, motor_t *left_motor,
                         motor_t *right_motor);

#endif // MOTION_LOGIC_H
