#include "pico/stdlib.h"
#include "src/app/state_machine.h"
#include "src/drivers/buzzer.h"
#include "src/drivers/motor_driver.h"
#include "src/drivers/push_button.h"
#include "src/drivers/ultrasonic_sensor.h"

// Pin definitions
#define LEFT_MOTOR_IN1_PIN 1
#define LEFT_MOTOR_IN2_PIN 2
#define LEFT_MOTOR_PWM_PIN 3
#define RIGHT_MOTOR_IN1_PIN 4
#define RIGHT_MOTOR_IN2_PIN 5
#define RIGHT_MOTOR_PWM_PIN 6

#define LEFT_ULTRASONIC_TRIGGER_PIN 16
#define LEFT_ULTRASONIC_ECHO_PIN 17
#define RIGHT_ULTRASONIC_TRIGGER_PIN 18
#define RIGHT_ULTRASONIC_ECHO_PIN 19

#define BUZZER_PIN 10
#define STOP_BUTTON_PIN 22

#define LED_PIN 25

#define ALARM_SLEEP_DURATION_MS 30000 // 30 seconds for testing

int main() {
  // Initialize standard I/O
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

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
  ultrasonic_config_t left_sensor = {
      .trigger_pin = LEFT_ULTRASONIC_TRIGGER_PIN,
      .echo_pin = LEFT_ULTRASONIC_ECHO_PIN,
  };
  ultrasonic_config_t right_sensor = {
      .trigger_pin = RIGHT_ULTRASONIC_TRIGGER_PIN,
      .echo_pin = RIGHT_ULTRASONIC_ECHO_PIN,
  };
  if (!ultrasonic_init(&left_sensor)) {
    return -1;
  }
  if (!ultrasonic_init(&right_sensor)) {
    return -1;
  }

  // Configure buzzer
  buzzer_t buzzer = {.pin = BUZZER_PIN};
  buzzer_init(&buzzer);

  button_t stop_button = {.pin = STOP_BUTTON_PIN};
  button_init(&stop_button);

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
  buzzer_beep(&buzzer, 500);
  sleep_ms(1000);
  buzzer_beep(&buzzer, 500);

  // Main loop
  while (true) {
    state_update(&machine);
  }
  return 0;
}
