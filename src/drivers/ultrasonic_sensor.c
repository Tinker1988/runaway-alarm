#include "ultrasonic_sensor.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "Ultrasonic";

void ultrasonic_init(ultrasonic_sensor_t *sensor) {
  gpio_set_direction(sensor->trigger_pin, GPIO_MODE_OUTPUT);
  gpio_set_direction(sensor->echo_pin, GPIO_MODE_INPUT);
}

float ultrasonic_read_distance_cm(ultrasonic_sensor_t *sensor) {
  uint64_t start_time, end_time;
  uint32_t pulse_duration;

  // Send trigger pulse (10 microseconds HIGH)
  gpio_set_level(sensor->trigger_pin, 0);
  esp_rom_delay_us(2);
  gpio_set_level(sensor->trigger_pin, 1);
  esp_rom_delay_us(10);
  gpio_set_level(sensor->trigger_pin, 0);

  // Wait for echo start
  int timeout = 10000;
  while (gpio_get_level(sensor->echo_pin) == 0 && timeout > 0) {
    esp_rom_delay_us(1);
    timeout--;
  }
  if (timeout == 0) {
    ESP_LOGW(TAG, "Timeout waiting for echo to start");
    return -1.0f;
  }
  start_time = esp_timer_get_time();

  // Wait for echo end
  timeout = 30000;
  while (gpio_get_level(sensor->echo_pin) == 1 && timeout > 0) {
    esp_rom_delay_us(1);
    timeout--;
  }
  if (timeout == 0) {
    ESP_LOGW(TAG, "Timeout waiting for echo to end");
    return -1.0f;
  }
  end_time = esp_timer_get_time();

  pulse_duration = (uint32_t)(end_time - start_time);

  // Speed of sound = 34300 cm/s
  // Distance = (pulse_duration in us) * 0.0343 / 2
  float distance_cm = (pulse_duration * 0.0343f) / 2.0f;
  return distance_cm;
}
