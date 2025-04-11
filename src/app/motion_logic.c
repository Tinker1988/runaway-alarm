#include "motion_logic.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "motor_driver.h"

#define TAG "MOTION_LOGIC"
#define OBSTACLE_THRESHOLD_CM 20.0

void motion_logic_update(ultrasonic_sensor_t *left_sensor,
                         ultrasonic_sensor_t *right_sensor, motor_t *left_motor,
                         motor_t *right_motor) {
  // read distances
  float left_distance = ultrasonic_sensor_read_distance(left_sensor);
  float right_distance = ultrasonic_sensor_read_distance(right_sensor);

  ESP_LOGI(TAG, "Left sensor: %.1f cm, Right sensor: %.1f cm", left_distance,
           right_distance);

  // determine if there's an obstacle within threshold dist
  bool obstacle_left =
      (left_distance > 0 && left_distance < OBSTACLE_THRESHOLD_CM);
  bool obstacle_right =
      (right_distance > 0 && right_distance < OBSTACLE_THRESHOLD_CM);

  if (obstacle_left && obstacle_right) {
    ESP_LOGI(TAG, "Obstacles on both sides. Reversing.");
    motor_backward(left_motor, 200);
    motor_backward(right_motor, 200);
  } else if (obstacle_left) {
    ESP_LOGI(TAG, "Obstacle on left. Turning right.");
    motor_forward(left_motor, 200);
    motor_backward(right_motor, 200);
  } else if (obstacle_right) {
    ESP_LOGI(TAG, "Obstacle on right. Turning left.");
    motor_forward(right_motor, 200);
    motor_backward(left_motor, 200);
  } else {
    ESP_LOGI(TAG, "Path is clear. Moving forward.");
    motor_forward(left_motor, 200);
    motor_forward(right_motor, 200);
  }

  vTaskDelay(pdMS_TO_TICKS(300));
}
