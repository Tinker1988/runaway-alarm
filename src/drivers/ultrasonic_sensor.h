#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <sys/types.h>

typedef struct {
  uint trigger_pin;
  uint echo_pin;
  uint timeout_us;
} ultrasonic_sensor_t;

void ultrasonic_init(ultrasonic_sensor_t *sensor);
float ultrasonic_read_distance_cm(ultrasonic_sensor_t *sensor);

#endif // !ULTRASONIC_SENSOR_H
