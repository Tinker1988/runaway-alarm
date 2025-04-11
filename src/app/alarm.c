#include "alarm.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUZZER_GPIO GPIO_NUM_13
static bool alarm_active = false;

void alarm_init(void) {
  gpio_reset_pin(BUZZER_GPIO);
  gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(BUZZER_GPIO, 0);
}

void alarm_start(void) {
  alarm_active = true;
  gpio_set_level(BUZZER_GPIO, 1);
}

void alarm_stop(void) {
  gpio_set_level(BUZZER_GPIO, 0);
  alarm_active = false;
}

bool alarm_triggered(void) { return alarm_active; }
