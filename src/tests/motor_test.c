#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "motor_driver.h"

#define TAG "MOTOR_TEST"

// Define left and right motors
motor_t left_motor = {
    .in1_pin = GPIO_NUM_25,
    .in2_pin = GPIO_NUM_26,
    .pwm_pin = GPIO_NUM_27,
    .pwm_channel = LEDC_CHANNEL_0
};

motor_t right_motor = {
    .in1_pin = GPIO_NUM_32,
    .in2_pin = GPIO_NUM_33,
    .pwm_pin = GPIO_NUM_14,
    .pwm_channel = LEDC_CHANNEL_1
};

void app_main(void) {
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
