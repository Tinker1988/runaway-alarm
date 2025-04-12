#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// System states
typedef enum {
  STATE_IDLE,        // System is idle, can enter configuration mode
  STATE_SLEEPING,    // System is in deep sleep until alarm time
  STATE_ALARM_ACTIVE // Alarm is active, device is running away
} system_state_t;

// Initialize the state machine
void state_machine_init(void);

// Run the state machine (doesn't return)
void state_machine_run(void);

// Get current state
system_state_t state_machine_get_state(void);

#endif // STATE_MACHINE_H
