// deep_sleep.c
#include "deep_sleep.h"
#include <stdio.h>
#include "hardware/clocks.h"
#include "hardware/watchdog.h"

static sleep_state_t current_sleep_state = SLEEP_DISABLED;
static bool was_woken_from_sleep = false;
static int wake_reason = -1;

static void wake_callback(void) {
    was_woken_from_sleep = true;
    wake_reason = 1; // GPIO wake
    current_sleep_state = SLEEP_ENABLED;
}

void deep_sleep_init(void) {
    datetime_t t = {
        .year  = 2023,
        .month = 1,
        .day   = 1,
        .dotw  = 0,
        .hour  = 0,
        .min   = 0,
        .sec   = 0
    };

    rtc_init();
    rtc_set_datetime(&t);

    if (watchdog_caused_reboot()) {
        was_woken_from_sleep = true;
        wake_reason = 0; // Timer wake
    }

    current_sleep_state = SLEEP_ENABLED;
    printf("Deep sleep initialized\n");
}

void deep_sleep_for_ms(uint32_t sleep_ms) {
    if (current_sleep_state == SLEEP_DISABLED)
        return;

    printf("Entering deep sleep for %lu ms\n", sleep_ms);
    current_sleep_state = SLEEP_ACTIVE;

    if (sleep_ms >= 1000) {
        datetime_t t;
        rtc_get_datetime(&t);
        uint32_t seconds = sleep_ms / 1000;
        t.sec += seconds;

        while (t.sec >= 60) {
            t.sec -= 60;
            t.min++;
            if (t.min >= 60) {
                t.min = 0;
                t.hour++;
                if (t.hour >= 24) {
                    t.hour = 0;
                }
            }
        }

        sleep_goto_sleep_until(&t, NULL);
    } else {
        sleep_us(sleep_ms * 1000);
    }

    was_woken_from_sleep = true;
    wake_reason = 0;
    current_sleep_state = SLEEP_ENABLED;
    printf("Woken from deep sleep\n");
}

void deep_sleep_enable_button_wake(uint gpio_pin, bool trigger_level) {
    if (current_sleep_state == SLEEP_DISABLED)
        return;

    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);

    if (trigger_level)
        gpio_pull_down(gpio_pin);
    else
        gpio_pull_up(gpio_pin);

    sleep_run_from_xosc();
    sleep_gpio_enable_wake_callback(gpio_pin, trigger_level, &wake_callback);

    printf("GPIO %d wake configured (level: %d)\n", gpio_pin, trigger_level);
}

sleep_state_t deep_sleep_get_state(void) {
    return current_sleep_state;
}

bool deep_sleep_was_woken(void) {
    return was_woken_from_sleep;
}

int deep_sleep_get_wake_reason(void) {
    return wake_reason;
}
