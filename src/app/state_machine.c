#include "state_machine.h"
#include "motion_logic.h"
#include <stdio.h>

void state_machine_init(state_machine_t *machine) {
  machine->state = STATE_SLEEP;
  machine->last_state_change_time = time_us_64() / 1000;
  machine->next_alarm_time =
      machine->last_state_change_time + machine->sleep_duration_ms;

  printf("Alarm initialized. Sleep duration: %lu ms\n",
         machine->sleep_duration_ms);
}

void state_update(state_machine_t *machine) {
  uint64_t current_time = time_us_64() / 1000; // Current time in ms

  switch (machine->state) {
  case STATE_SLEEP:
    // Check if it's time to trigger the alarm
    if (current_time >= machine->next_alarm_time) {
      printf("ALARM TRIGGERED!\n");

      // Change state to running
      machine->state = STATE_RUNNING;
      machine->last_state_change_time = current_time;

      // Turn on buzzer
      buzzer_on(machine->buzzer, 2000); // 2kHz alarm sound
    } else {
      // Still in sleep state, could add a status indicator LED blink here
      sleep_ms(100); // Brief sleep to avoid hogging CPU
    }
    break;

  case ALARM_STATE_RUNNING:
    // Check if stop button was pressed
    if (push_button_was_pressed(machine->stop_button)) {
      printf("Alarm stopped by button press\n");

      // Stop motors and buzzer
      motor_stop(machine->left_motor);
      motor_stop(machine->right_motor);
      buzzer_off(machine->buzzer);

      // Set next alarm time (30 seconds from now)
      machine->next_alarm_time = current_time + alarm->sleep_duration_ms;

      // Change state back to sleep
      machine->state = ALARM_STATE_SLEEP;
      machine->last_state_change_time = current_time;

      // Brief beep to confirm alarm is off
      buzzer_beep(machine->buzzer, 1000, 100);
    } else {
      // Update robot movement based on sensors
      motion_logic_update(machine->left_sensor, machine->right_sensor,
                          machine->left_motor, machine->right_motor);
    }
    break;
  }
}
