#ifndef ACTIVE_BUZZER_H
#define ACTIVE_BUZZER_H

#include "pico/stdlib.h"

typedef struct {
  uint pin;
} buzzer_t;

void buzzer_init(buzzer_t *buzzer);
void buzzer_on(buzzer_t *buzzer);
void buzzer_off(buzzer_t *buzzer);
void buzzer_beep(buzzer_t *buzzer, uint duration_ms);

#endif // !ACTIVE_BUZZER_H
