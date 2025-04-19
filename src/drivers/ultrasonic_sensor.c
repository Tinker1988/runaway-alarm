#include "ultrasonic_sensor.h"
#include "hardware/gpio.h"
#include "pico/time.h"

// Speed of sound in air (approx. 343 meters/second at room temperature)
#define SPEED_OF_SOUND_CM_US 0.0343 // cm per microsecond

bool ultrasonic_init(const ultrasonic_config_t *config) {
  if (!config) {
    return false;
  }

  // Initialize trigger pin as output
  gpio_init(config->trigger_pin);
  gpio_set_dir(config->trigger_pin, GPIO_OUT);
  gpio_put(config->trigger_pin, 0); // Start with trigger low

  // Initialize echo pin as input
  gpio_init(config->echo_pin);
  gpio_set_dir(config->echo_pin, GPIO_IN);

  return true;
}

float ultrasonic_measure_distance_cm(const ultrasonic_config_t *config,
                                     uint32_t timeout_us) {
  // Reset the trigger pin to ensure a clean pulse
  gpio_put(config->trigger_pin, 0);
  sleep_us(2);

  // Send a 10us pulse on the trigger pin
  gpio_put(config->trigger_pin, 1);
  sleep_us(10);
  gpio_put(config->trigger_pin, 0);

  // Wait for echo pin to go high (start of echo pulse)
  absolute_time_t timeout_time = make_timeout_time_us(timeout_us);
  while (!gpio_get(config->echo_pin)) {
    if (absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
      return 0; // Timeout waiting for echo to start
    }
  }

  // Measure echo pulse duration
  absolute_time_t start_time = get_absolute_time();
  timeout_time = make_timeout_time_us(timeout_us);

  while (gpio_get(config->echo_pin)) {
    if (absolute_time_diff_us(get_absolute_time(), timeout_time) <= 0) {
      return 0; // Timeout waiting for echo to end
    }
  }

  absolute_time_t end_time = get_absolute_time();

  // Calculate pulse duration in microseconds
  uint64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

  // Convert pulse duration to distance in centimeters
  // Distance = (Time × Speed of Sound) / 2
  // We divide by 2 because sound travels to the object and back
  float distance_cm = (pulse_duration * SPEED_OF_SOUND_CM_US) / 2.0f;

  return distance_cm;
}
