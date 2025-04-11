#include "alarm.h"
#include "deep_sleep.h"

#include "drivers/gpio.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include <stdio.h>
#include "drivers/motor_driver.h"

void app_main(void) {
  printf("Runaway Alarm Clock Booted!\n");
  ESP_LOGI(TAG, "Starting motor driver test...");

  motor_init(&left_motor);
  motor_init(&right_motor);

  ESP_LOGI(TAG, "Moving forward");
  motor_forward(&left_motor, 255);
  motor_forward(&right_motor, 255);
  vTaskDelay(pdMS_TO_TICKS(2000));

  ESP_LOGI(TAG, "Reversing");
  motor_backward(&left_motor, 255);
  motor_backward(&right_motor, 255);
  vTaskDelay(pdMS_TO_TICKS(2000));

  ESP_LOGI(TAG, "Stopping");
  motor_stop(&left_motor);
  motor_stop(&right_motor);

  ESP_LOGI(TAG, "Test complete");
}
