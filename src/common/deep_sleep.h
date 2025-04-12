#ifndef DEEP_SLEEP_H
#define DEEP_SLEEP_H

#include <freertos/FreeRTOS.h>
#include <stdint.h>
#include <time.h>

// Wake-up sources
typedef enum { WAKEUP_TIMER, WAKEUP_BUTTON, WAKEUP_UNDEFINED } wakeup_reason_t;

void deep_sleep_init(void);
void deep_sleep_wakeup_at(int hour, int minute);
void deep_sleep_start(void);
wakeup_reason_t deep_sleep_get_wakeup_reason(void);
void deep_sleep_configure_button_wakeup(uint32_t gpio_num);

#endif // DEEP_SLEEP_H
