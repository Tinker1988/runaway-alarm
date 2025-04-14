#include "motor_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

// Motor pin definitions for Pico
#define LEFT_MOTOR_IN1_PIN 1
#define LEFT_MOTOR_IN2_PIN 2
#define LEFT_MOTOR_PWM_PIN 3

#define RIGHT_MOTOR_IN1_PIN 4
#define RIGHT_MOTOR_IN2_PIN 5
#define RIGHT_MOTOR_PWM_PIN 6

void test_speed_ramping(motor_t *motor) {
  printf("Testing speed ramping...\n");
  for (uint8_t speed = 0; speed <= 250; speed += 5) {
    printf("Speed: %d\n", speed);
    motor_forward(motor, speed);
    sleep_ms(100);
  }
  motor_stop(motor);
}

void test_direction_changes(motor_t *motor) {
  printf("Testing direction changes...\n");
  for (int i = 0; i < 5; i++) {
    printf("Cycle %d: Forward\n", i + 1);
    motor_forward(motor, 128);
    sleep_ms(500);
    printf("Cycle %d: Backward\n", i + 1);
    motor_backward(motor, 128);
    sleep_ms(500);
  }
  motor_stop(motor);
}

int main() {
  stdio_init_all();
  printf("Motor Driver Test for Raspberry Pi Pico\n");

  // Initialize motor structures
  motor_t left_motor = {.in1_pin = LEFT_MOTOR_IN1_PIN,
                        .in2_pin = LEFT_MOTOR_IN2_PIN,
                        .pwm_pin = LEFT_MOTOR_PWM_PIN};

  motor_t right_motor = {.in1_pin = RIGHT_MOTOR_IN1_PIN,
                         .in2_pin = RIGHT_MOTOR_IN2_PIN,
                         .pwm_pin = RIGHT_MOTOR_PWM_PIN};

  // Initialize motors
  motor_init(&left_motor);
  motor_init(&right_motor);

  // Test sequence
  printf("Moving forward...\n");
  motor_forward(&left_motor, 128); // Half speed
  motor_forward(&right_motor, 128);
  sleep_ms(2000);

  printf("Moving backward...\n");
  motor_backward(&left_motor, 128);
  motor_backward(&right_motor, 128);
  sleep_ms(2000);

  printf("Turning left...\n");
  motor_forward(&left_motor, 64);   // Slower
  motor_forward(&right_motor, 192); // Faster
  sleep_ms(2000);

  printf("Turning right...\n");
  motor_forward(&left_motor, 192); // Faster
  motor_forward(&right_motor, 64); // Slower
  sleep_ms(2000);

  printf("Stopping...\n");
  motor_stop(&left_motor);
  motor_stop(&right_motor);

  test_speed_ramping(&left_motor);

  test_direction_changes(&right_motor);

  while (true) {
    tight_loop_contents();
  }

  return 0;
}
