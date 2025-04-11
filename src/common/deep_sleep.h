#ifndef DEEP_SLEEP_H
#define DEEP_SLEEP_H

#include <stdint.h>

void deep_sleep_init(uint64_t wakeup_time_ms);
void deep_sleep_start();

#endif // !DEEP_SLEEP_H
