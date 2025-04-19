#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H

#include "pico/stdlib.h"

typedef struct {
  uint pin;
  bool active_low; // true if button pulls to GND when pressed
} button_t;

void button_init(button_t *button);
bool button_is_pressed(button_t *button);

#endif // !PUSH_BUTTON_H
