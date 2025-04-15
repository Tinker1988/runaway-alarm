#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <sys/types.h>

typedef struct {
  uint pin;
  bool is_active_low;
  bool last_state;
  uint64_t last_debounce_time;
  uint debounce_delay_ms;
} push_button_t;

void push_button_init(push_button_t *button);
bool push_button_is_pressed(push_button_t *button);
bool push_button_was_pressed(
    push_button_t *button); // Detects button press events

#endif // PUSH_BUTTON_H
