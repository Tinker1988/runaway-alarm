#ifndef BUZZER_H
#define BUZZER_H

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <sys/types.h>

typedef struct {
  uint pin;
  uint slice;
  uint channel;
} buzzer_t;

void buzzer_init(buzzer_t *buzzer);
void buzzer_on(buzzer_t *buzzer, uint frequency);
void buzzer_off(buzzer_t *buzzer);
void buzzer_beep(buzzer_t *buzzer, uint frequency, uint duration_ms);

#endif // !BUZZER_H
