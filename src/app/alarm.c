#include "pico
// alarm.c
#include "alarm.h"
#include "pico/stdlib.h"
#include <stdio.h>

void alarm_buzz_cycle_until_button(int buzzer_gpio, int button_gpio) {
    gpio_init(buzzer_gpio);
    gpio_set_dir(buzzer_gpio, GPIO_OUT);

    gpio_init(button_gpio);
    gpio_set_dir(button_gpio, GPIO_IN);
    gpio_pull_up(button_gpio); // Active LOW

    printf("Alarm cycle started. Buzz every 30s until button is pressed.\n");

    while (gpio_get(button_gpio)) {
        gpio_put(buzzer_gpio, 1);
        sleep_ms(1000); // Buzz 1s
        gpio_put(buzzer_gpio, 0);
        sleep_ms(29000); // Wait 29s
    }

    printf("Button pressed. Stopping alarm.\n");
    gpio_put(buzzer_gpio, 0);
}

