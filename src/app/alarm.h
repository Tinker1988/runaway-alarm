#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>

void alarm_init(void);
void alarm_start(void);
void alarm_stop(void);
bool alarm_triggered(void);

#endif // !ALARM_H
