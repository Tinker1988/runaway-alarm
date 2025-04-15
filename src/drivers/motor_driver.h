#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <sys/types.h>

typedef struct {
  uint in1_pin;
  uint in2_pin;
  uint pwm_pin;
  uint pwm_slice;
  uint pwm_channel;
} motor_t;

void motor_init(motor_t *motor);
void motor_forward(motor_t *motor, uint8_t speed);
void motor_backward(motor_t *motor, uint8_t speed);
void motor_stop(motor_t *motor);

#endif // MOTOR_DRIVER_H
