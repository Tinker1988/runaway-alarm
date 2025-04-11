#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "motor_driver.h"

#define IN1 GPIO_NUM_25
#define IN2 GPIO_NUM_26
#define PWM GPIO_NUM_27

void app_main(void) {
  motor_t motor = {.in1_pin = IN1,
                   .in2_pin = IN2,
                   .pwm_pin = PWM,
                   .pwm_channel = LEDC_CHANNEL_0};

  motor_init(&motor);

  while (1) {
    motor_forward(&motor, 200);
    ESP_LOGI("MOTOR", "motor moving forward");
    vTaskDelay(pdMS_TO_TICKS(2000));
    motor_backward(&motor, 200);
    ESP_LOGI("MOTOR", "motor moving backward");
    vTaskDelay(pdMS_TO_TICKS(2000));
    motor_stop(&motor);
    ESP_LOGI("MOTOR", "motor stopped");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
