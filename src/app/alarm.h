#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>
#include <time.h>

typedef struct {
    int hour;
    int minute;
    bool enabled;
} alarm_config_t;

void alarm_init(void);
void alarm_set(int hour, int minute);
void alarm_start(void);
void alarm_stop(void);
bool alarm_is_triggered(void);
bool alarm_should_activate(void);
void alarm_save_config(void);
void alarm_load_config(void);
alarm_config_t* alarm_get_config(void);

#endif // ALARM_H
