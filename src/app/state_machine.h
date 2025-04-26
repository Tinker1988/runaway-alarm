#ifndef STATE_H
#define STATE_H

#include "../drivers/buzzer.h"
#include "../drivers/motor_driver.h"
#include "../drivers/push_button.h"
#include "../drivers/ultrasonic_sensor.h"
#include "pico/stdlib.h"

// bot states
typedef enum {
  STATE_SLEEP,  // Waiting for alarm time
  STATE_RUNNING // Alarm active (buzzer on, robot moving)
} state_t;

typedef struct {
  uint32_t sleep_duration_ms;      // Duration of sleep state (for testing)
  uint64_t last_state_change_time; // Timestamp of last state change
  uint64_t next_alarm_time;        // Timestamp when alarm should trigger
  state_t state;                   // Current state

  // Components
  buzzer_t *buzzer;
  motor_t *left_motor;
  motor_t *right_motor;
  ultrasonic_config_t *left_sensor;
  ultrasonic_config_t *right_sensor;
  button_t *stop_button;
  bool last_button_state;
  uint64_t last_debounce_time;
} state_machine_t;

void state_machine_init(state_machine_t *state);
void state_update(state_machine_t *state);

#endif // STATE_H
