#include "buzzer.h"
#include <stdint.h>

#define BUZZER_CLOCK_DIV 100

void buzzer_init(buzzer_t *buzzer) {
  gpio_set_function(buzzer->pin, GPIO_FUNC_PWM);
  buzzer->slice = pwm_gpio_to_slice_num(buzzer->pin);
  buzzer->channel = pwm_gpio_to_channel(buzzer->pin);

  // Configure PWM
  pwm_set_clkdiv(buzzer->slice, BUZZER_CLOCK_DIV);
  pwm_set_enabled(buzzer->slice, true);

  // Initially off
  buzzer_off(buzzer);
}

void buzzer_on(buzzer_t *buzzer, uint frequency) {
  // calculate PWM parameters for the desired frequency
  uint32_t clock_freq = 125000000; // 125 MHz
  uint32_t pwm_freq = clock_freq / BUZZER_CLOCK_DIV;
  uint wrap = pwm_freq / frequency;

  pwm_set_wrap(buzzer->slice, wrap);
  pwm_set_chan_level(buzzer->slice, buzzer->channel,
                     wrap / 2); // 50% duty cycle
}

void buzzer_off(buzzer_t *buzzer) {
  pwm_set_chan_level(buzzer->slice, buzzer->channel, 0);
}

void buzzer_beep(buzzer_t *buzzer, uint frequency, uint duration_ms) {
  buzzer_on(buzzer, frequency);
  sleep_ms(duration_ms);
  buzzer_off(buzzer);
}
