#include "state_machine.h"
#include "motion_logic.h"
#include <stdio.h>

#define LED_PIN 25

void state_machine_init(state_machine_t *machine) {
  machine->state = STATE_SLEEP;
  machine->last_state_change_time = time_us_64() / 1000;
  machine->next_alarm_time =
      machine->last_state_change_time + machine->sleep_duration_ms;
  // Initialize button tracking variables
  machine->last_button_state = false;
  machine->last_debounce_time = 0;
  printf("Alarm initialized. Sleep duration: %lu ms\n",
         machine->sleep_duration_ms);
}

void state_update(state_machine_t *machine) {
  uint64_t current_time = time_us_64() / 1000;
  const uint64_t debounce_delay = 50; // 50ms debounce window

  // === Kill Switch Check (Global Button Press Handler) ===
  bool current_button_state = button_is_pressed(machine->stop_button);

  if (current_button_state && !machine->last_button_state) {

    // If we're running, go to sleep
    if (machine->state == STATE_RUNNING) {
      // going to sleep

      // Stop everything
      motor_stop(machine->left_motor);
      motor_stop(machine->right_motor);
      buzzer_off(machine->buzzer);

      // Set next alarm time
      machine->next_alarm_time = current_time + machine->sleep_duration_ms;

      // State transition
      machine->state = STATE_SLEEP;
      machine->last_state_change_time = current_time;

      // Confirm with beep
      buzzer_beep(machine->buzzer, 200);
    } else if (machine->state == STATE_SLEEP) {
      // waking up and starting

      // Transition to running state
      machine->state = STATE_RUNNING;
      machine->last_state_change_time = current_time;

      // Start the alarm
      buzzer_on(machine->buzzer);
    }
  }

  machine->last_button_state = current_button_state;
  sleep_ms(debounce_delay);

  // normal state handling
  switch (machine->state) {
  case STATE_SLEEP:
    // check if its time to wake up based on alarm
    if (current_time >= machine->next_alarm_time) {
      machine->state = STATE_RUNNING;
      machine->last_state_change_time = current_time;
      buzzer_on(machine->buzzer);
    } else {
      sleep_ms(100);
    }
    break;
  case STATE_RUNNING:
    motion_logic_update(machine->left_sensor, machine->right_sensor,
                        machine->left_motor, machine->right_motor);
    break;
  }
}
