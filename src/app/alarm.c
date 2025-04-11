#include "alarm.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUZZER_GPIO 18

void trigger_alarm() {
  gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);

  for (int i = 0; i < 5; i++) {
    gpio_set_level(BUZZER_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(BUZZER_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
