#include "motor_driver.h"

#define PWM_RANGE 255 // 8-bit PWM range
#define PWM_CLOCK_DIV 100
#define PWM_WRAP 12500

void motor_init(motor_t *motor) {
  gpio_init(motor->in1_pin);
  gpio_init(motor->in2_pin);
  gpio_set_dir(motor->in1_pin, GPIO_OUT);
  gpio_set_dir(motor->in2_pin, GPIO_OUT);

  // Initialize PWM for speed control
  gpio_set_function(motor->pwm_pin, GPIO_FUNC_PWM);
  motor->pwm_slice = pwm_gpio_to_slice_num(motor->pwm_pin);
  motor->pwm_channel = pwm_gpio_to_channel(motor->pwm_pin);

  // Configure PWM
  pwm_set_clkdiv(motor->pwm_slice, PWM_CLOCK_DIV);
  pwm_set_wrap(motor->pwm_slice, PWM_WRAP);
  pwm_set_chan_level(motor->pwm_slice, motor->pwm_channel, 0);
  pwm_set_enabled(motor->pwm_slice, true);

  // Ensure motors are stopped at initialization
  motor_stop(motor);
}

void motor_forward(motor_t *motor, uint8_t speed) {
  gpio_put(motor->in1_pin, 1);
  gpio_put(motor->in2_pin, 0);

  // Set PWM duty cycle based on speed (0-255)
  uint16_t duty = (speed * PWM_WRAP) / PWM_RANGE;
  pwm_set_chan_level(motor->pwm_slice, motor->pwm_channel, duty);
}

void motor_backward(motor_t *motor, uint8_t speed) {
  gpio_put(motor->in1_pin, 0);
  gpio_put(motor->in2_pin, 1);

  // Set PWM duty cycle based on speed (0-255)
  uint16_t duty = (speed * PWM_WRAP) / PWM_RANGE;
  pwm_set_chan_level(motor->pwm_slice, motor->pwm_channel, duty);
}

void motor_stop(motor_t *motor) {
  gpio_put(motor->in1_pin, 0);
  gpio_put(motor->in2_pin, 0);
  pwm_set_chan_level(motor->pwm_slice, motor->pwm_channel, 0);
}
