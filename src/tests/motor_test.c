#include "motor_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MOTOR_TEST";

// Motor configuration
motor_t left_motor = {
    .in1_pin = GPIO_NUM_25,
    .in2_pin = GPIO_NUM_26,
    .pwm_pin = GPIO_NUM_27,
    .pwm_channel = LEDC_CHANNEL_0,
};

motor_t right_motor = {
    .in1_pin = GPIO_NUM_14,
    .in2_pin = GPIO_NUM_12,
    .pwm_pin = GPIO_NUM_13,
    .pwm_channel = LEDC_CHANNEL_1,
};

void app_main(void) {
    ESP_LOGI(TAG, "Initializing motors...");
    motor_init(&left_motor);
    motor_init(&right_motor);

    while (1) {
        ESP_LOGI(TAG, "Moving Forward");
        motor_forward(&left_motor, 200);
        motor_forward(&right_motor, 200);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ESP_LOGI(TAG, "Stopping");
        motor_stop(&left_motor);
        motor_stop(&right_motor);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(TAG, "Moving Backward");
        motor_backward(&left_motor, 200);
        motor_backward(&right_motor, 200);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ESP_LOGI(TAG, "Stopping");
        motor_stop(&left_motor);
        motor_stop(&right_motor);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
