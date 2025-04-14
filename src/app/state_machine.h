#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_IDLE,
    STATE_SLEEPING,
    STATE_ALARM_ACTIVE
} system_state_t;

void state_machine_init(void);
void state_machine_run(void);
system_state_t state_machine_get_state(void);

#endif // STATE_MACHINE_H
