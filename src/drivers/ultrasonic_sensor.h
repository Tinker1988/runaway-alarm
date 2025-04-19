#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "pico/stdlib.h"

typedef struct {
  uint8_t trigger_pin; // Output pin to trigger measurement
  uint8_t echo_pin;    // Input pin to receive echo
} ultrasonic_config_t;

bool ultrasonic_init(const ultrasonic_config_t *config);
float ultrasonic_measure_distance_cm(const ultrasonic_config_t *config,
                                     uint32_t timeout_us);

#endif /* ULTRASONIC_SENSOR_H */
