#include "buzzer.h"
#include "motor_driver.h"
#include "pico/stdlib.h"
#include "push_button.h"
#include "state_machine.h"
#include "ultrasonic_sensor.h"
#include <stdio.h>

// Pin definitions
#define LEFT_MOTOR_IN1_PIN 2
#define LEFT_MOTOR_IN2_PIN 3
#define LEFT_MOTOR_PWM_PIN 4
#define RIGHT_MOTOR_IN1_PIN 5
#define RIGHT_MOTOR_IN2_PIN 6
#define RIGHT_MOTOR_PWM_PIN 7

#define LEFT_ULTRASONIC_TRIGGER_PIN 8
#define LEFT_ULTRASONIC_ECHO_PIN 9
#define RIGHT_ULTRASONIC_TRIGGER_PIN 10
#define RIGHT_ULTRASONIC_ECHO_PIN 11

#define BUZZER_PIN 12
#define STOP_BUTTON_PIN 13

#define ALARM_SLEEP_DURATION_MS 30000 // 30 seconds for testing

int main() {
  // Initialize standard I/O
  stdio_init_all();
  printf("Alarm Robot Starting...\n");

  // Configure motors
  motor_t left_motor = {.in1_pin = LEFT_MOTOR_IN1_PIN,
                        .in2_pin = LEFT_MOTOR_IN2_PIN,
                        .pwm_pin = LEFT_MOTOR_PWM_PIN};

  motor_t right_motor = {.in1_pin = RIGHT_MOTOR_IN1_PIN,
                         .in2_pin = RIGHT_MOTOR_IN2_PIN,
                         .pwm_pin = RIGHT_MOTOR_PWM_PIN};

  motor_init(&left_motor);
  motor_init(&right_motor);

  // Configure ultrasonic sensors
  ultrasonic_sensor_t left_sensor = {
      .trigger_pin = LEFT_ULTRASONIC_TRIGGER_PIN,
      .echo_pin = LEFT_ULTRASONIC_ECHO_PIN,
      .timeout_us = 30000 // 30ms timeout (~5m range)
  };

  ultrasonic_sensor_t right_sensor = {
      .trigger_pin = RIGHT_ULTRASONIC_TRIGGER_PIN,
      .echo_pin = RIGHT_ULTRASONIC_ECHO_PIN,
      .timeout_us = 30000 // 30ms timeout (~5m range)
  };

  ultrasonic_init(&left_sensor);
  ultrasonic_init(&right_sensor);

  // Configure buzzer
  buzzer_t buzzer = {.pin = BUZZER_PIN};

  buzzer_init(&buzzer);

  // Configure stop button (assuming active low with pull-up)
  push_button_t stop_button = {
      .pin = STOP_BUTTON_PIN, .is_active_low = true, .debounce_delay_ms = 50};

  push_button_init(&stop_button);

  // Configure alarm
  state_machine_t machine = {.sleep_duration_ms = ALARM_SLEEP_DURATION_MS,
                             .buzzer = &buzzer,
                             .left_motor = &left_motor,
                             .right_motor = &right_motor,
                             .left_sensor = &left_sensor,
                             .right_sensor = &right_sensor,
                             .stop_button = &stop_button};

  state_machine_init(&machine);

  // Beep to indicate startup complete
  buzzer_beep(&buzzer, 1000, 200);
  sleep_ms(100);
  buzzer_beep(&buzzer, 1500, 200);

  printf("System initialized. First alarm will trigger in %lu seconds\n",
         ALARM_SLEEP_DURATION_MS / 1000);

  // Main loop
  while (true) {
    state_update(&alarm);
  }

  return 0;
}
