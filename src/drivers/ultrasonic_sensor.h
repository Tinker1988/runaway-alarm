#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "driver/gpio.h"

typedef struct {
    gpio_num_t trigger_pin;
    gpio_num_t echo_pin;
} ultrasonic_sensor_t;

void ultrasonic_init(ultrasonic_sensor_t *sensor);
float ultrasonic_read_distance_cm(ultrasonic_sensor_t *sensor);

#endif // ULTRASONIC_H
