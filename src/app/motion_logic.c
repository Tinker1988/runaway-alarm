#include "motion_logic.h"
#include "stdio.h"
#include "pico/stdlib.h"
#include "motor_driver.h"

#define OBSTACLE_THRESHOLD_CM 20.0

void motion_logic_update(ultrasonic_sensor_t *left_sensor,
                         ultrasonic_sensor_t *right_sensor, motor_t *left_motor,
                         motor_t *right_motor) {
  // read distances
  float left_distance = ultrasonic_read_distance_cm(left_sensor);
  float right_distance = ultrasonic_read_distance_cm(right_sensor);

  printf("Left sensor: %.1f cm, Right sensor: %.1f cm\n", left_distance, right_distance);

  // determine if there's an obstacle within threshold dist
  bool obstacle_left = (left_distance > 0 && left_distance < OBSTACLE_THRESHOLD_CM);
  bool obstacle_right = (right_distance > 0 && right_distance < OBSTACLE_THRESHOLD_CM);

  if (obstacle_left && obstacle_right) {
    printf("Obstacles on both sides. Reversing.\n");
    motor_backward(left_motor, 200);
    motor_backward(right_motor, 200);
  } else if (obstacle_left) {
    printf("Obstacle on left. Turning right.\n");
    motor_forward(left_motor, 200);
    motor_backward(right_motor, 200);
  } else if (obstacle_right) {
    printf("Obstacle on right. Turning left.\n");
    motor_forward(right_motor, 200);
    motor_backward(left_motor, 200);
  } else {
    printf("Path is clear. Moving forward.\n");
    motor_forward(left_motor, 200);
    motor_forward(right_motor, 200);
  }

  sleep_ms(300); // 300 ms delay
}
