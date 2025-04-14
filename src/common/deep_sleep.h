// deep_sleep.h
#ifndef DEEP_SLEEP_H
#define DEEP_SLEEP_H

#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "hardware/gpio.h"
#include "pico/sleep.h"

typedef enum {
    SLEEP_DISABLED,
    SLEEP_ENABLED,
    SLEEP_ACTIVE
} sleep_state_t;

void deep_sleep_init(void);
void deep_sleep_for_ms(uint32_t sleep_ms);
void deep_sleep_enable_button_wake(uint gpio_pin, bool trigger_level);
sleep_state_t deep_sleep_get_state(void);
bool deep_sleep_was_woken(void);
int deep_sleep_get_wake_reason(void);

#endif // DEEP_SLEEP_H
