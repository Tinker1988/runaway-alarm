#include "ultrasonic_sensor.h"
#include "hardware/timer.h"
#include <stdint.h>

#define ULTRASONIC_SPEED_OF_SOUND 0.0343 // cm/us

void ultrasonic_init(ultrasonic_sensor_t *sensor) {
  gpio_init(sensor->trigger_pin);
  gpio_init(sensor->echo_pin);

  gpio_set_dir(sensor->trigger_pin, GPIO_OUT);
  gpio_set_dir(sensor->echo_pin, GPIO_IN);

  gpio_put(sensor->trigger_pin, 0); // initialize trig pin to 0
}

float ultrasonic_read_distance_cm(ultrasonic_sensor_t *sensor) {
  // send 10us pulse to trigger
  gpio_put(sensor->trigger_pin, 0);
  sleep_us(2);
  gpio_put(sensor->trigger_pin, 1);
  sleep_us(10);
  gpio_put(sensor->trigger_pin, 0);

  // measure echo pulse duration
  uint64_t timeout = time_us_64() + sensor->timeout_us;

  // wait for echo to go high
  while (!gpio_get(sensor->echo_pin)) {
    if (time_us_64() > timeout) {
      return -1.0f; // timeout occurred
    }
  }

  uint64_t echo_start = time_us_64();

  // wait for echo to go low
  while (gpio_get(sensor->echo_pin)) {
    if (time_us_64() > timeout) {
      return -1.0f; // timeout occurred
    }
  }

  uint64_t echo_end = time_us_64();
  uint64_t echo_duration_us = echo_end - echo_start;

  // calcluate dist in cm
  float distance_cm =
      (float)echo_duration_us * ULTRASONIC_SPEED_OF_SOUND / 2.0f;

  return distance_cm;
}
