#include "state_machine.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "alarm.h"
#include "deep_sleep.h"
#include "motion_logic.h"
#include "motor_driver.h"
#include "ultrasonic_sensor.h"

#define BUTTON_GPIO 12
#define CONFIG_TIMEOUT_MS 300000  // 5 minutes

extern motor_t left_motor;
extern motor_t right_motor;
extern ultrasonic_sensor_t left_sensor;
extern ultrasonic_sensor_t right_sensor;

static system_state_t current_state = STATE_IDLE;

static void handle_idle_state(void) {
    printf("Entering IDLE state\n");

    motor_stop(&left_motor);
    motor_stop(&right_motor);

    uint32_t start_time = to_ms_since_boot(get_absolute_time());

    while (true) {
        if ((to_ms_since_boot(get_absolute_time()) - start_time) > CONFIG_TIMEOUT_MS) {
            printf("Configuration timeout reached\n");
            break;
        }

        alarm_config_t *config = alarm_get_config();
        if (config->enabled) {
            printf("Alarm configured for %02d:%02d\n", config->hour, config->minute);
            current_state = STATE_SLEEPING;
            return;
        }

        sleep_ms(1000);
    }

    printf("No alarm configured, staying in IDLE state\n");
}

static void handle_sleeping_state(void) {
    printf("Entering SLEEPING state\n");

    alarm_config_t *config = alarm_get_config();
    if (!config->enabled) {
        printf("No alarm configured, returning to IDLE state\n");
        current_state = STATE_IDLE;
        return;
    }

    printf("Setting deep sleep until %02d:%02d\n", config->hour, config->minute);
    deep_sleep_enable_button_wake(BUTTON_GPIO, false);  // Active LOW
    deep_sleep_for_ms(0);  // RTC alarm setup would be handled inside

    int reason = deep_sleep_get_wake_reason();
    if (reason == 0) {
        printf("Woke up due to RTC alarm\n");
        current_state = STATE_ALARM_ACTIVE;
    } else {
        printf("Woke up due to button press\n");
        current_state = STATE_IDLE;
    }
}

static void handle_alarm_active_state(void) {
    printf("Entering ALARM_ACTIVE state\n");

    while (alarm_is_triggered()) {
        // 1 second ON
        alarm_start();
        sleep_ms(1000);

        // 29 seconds OFF with obstacle updates
        alarm_stop();
        for (int i = 0; i < 290; i++) {
            if (!alarm_is_triggered()) {
                break;
            }

            motion_logic_update(&left_sensor, &right_sensor, &left_motor, &right_motor);
            sleep_ms(100);
        }
    }

    printf("Alarm stopped, returning to IDLE state\n");
    motor_stop(&left_motor);
    motor_stop(&right_motor);
    current_state = STATE_IDLE;
}

void state_machine_init(void) {
    if (deep_sleep_get_wake_reason() == 0) {
        current_state = STATE_ALARM_ACTIVE;
    } else {
        current_state = STATE_IDLE;
    }
}

void state_machine_run(void) {
    while (true) {
        switch (current_state) {
            case STATE_IDLE:
                handle_idle_state();
                break;
            case STATE_SLEEPING:
                handle_sleeping_state();
                break;
            case STATE_ALARM_ACTIVE:
                handle_alarm_active_state();
                break;
            default:
                printf("Unknown state: %d\n", current_state);
                current_state = STATE_IDLE;
                break;
        }
        sleep_ms(100);
    }
}

system_state_t state_machine_get_state(void) {
    return current_state;
}
