#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "driver/gpio.h"
#include "driver/ledc.h"

// Motor structure
typedef struct {
  gpio_num_t in1_pin;
  gpio_num_t in2_pin;
  gpio_num_t pwm_pin;
  ledc_channel_t pwm_channel;
} motor_t;

// Motor driver functions
void motor_init(motor_t *motor);
void motor_forward(motor_t *motor, uint32_t speed);  // speed 0-255
void motor_backward(motor_t *motor, uint32_t speed); // speed 0-255
void motor_stop(motor_t *motor);

#endif // MOTOR_DRIVER_H
