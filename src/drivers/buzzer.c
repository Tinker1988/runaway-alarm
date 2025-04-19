#include "buzzer.h"

void buzzer_init(buzzer_t *buzzer) {
  // Configure the pin as an output
  gpio_init(buzzer->pin);
  gpio_set_dir(buzzer->pin, GPIO_OUT);

  // Initially off
  buzzer_off(buzzer);
}

void buzzer_on(buzzer_t *buzzer) { gpio_put(buzzer->pin, 1); }

void buzzer_off(buzzer_t *buzzer) { gpio_put(buzzer->pin, 0); }

void buzzer_beep(buzzer_t *buzzer, uint duration_ms) {
  buzzer_on(buzzer);
  sleep_ms(duration_ms);
  buzzer_off(buzzer);
}
