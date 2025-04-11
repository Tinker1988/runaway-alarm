#include "motor_driver.h"

void motor_init(motor_t *motor) {
    // Set IN1 and IN2 pins as outputs
    gpio_set_direction(motor->in1_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(motor->in2_pin, GPIO_MODE_OUTPUT);

    // Configure PWM timer (only once)
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    // Configure PWM channel
    ledc_channel_config_t ledc_conf = {
        .channel = motor->pwm_channel,
        .duty = 0,
        .gpio_num = motor->pwm_pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_conf);
}

void motor_forward(motor_t *motor, uint32_t speed) {
    gpio_set_level(motor->in1_pin, 1);
    gpio_set_level(motor->in2_pin, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, motor->pwm_channel, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, motor->pwm_channel);
}

void motor_backward(motor_t *motor, uint32_t speed) {
    gpio_set_level(motor->in1_pin, 0);
    gpio_set_level(motor->in2_pin, 1);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, motor->pwm_channel, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, motor->pwm_channel);
}

void motor_stop(motor_t *motor) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, motor->pwm_channel, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, motor->pwm_channel);
}
