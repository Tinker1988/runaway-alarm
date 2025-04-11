#include "alarm.h"
#include "deep_sleep.h"

#include "driver/gpio.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include <stdio.h>

void app_main(void) {
  printf("Runaway Alarm Clock Booted!\n");

  // wake up cause info
  print_wakeup_reason();

  // simulate alarm trigger
  trigger_alarm();

  // wait before going to sleep again
  vTaskDelay(pdMS_TO_TICKS(10000));

  enter_deep_sleep();
}
