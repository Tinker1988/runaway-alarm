#include "deep_sleep.h"
#include "esp_log.h"
#include "esp_sleep.h"

#define TAG "DEEP_SLEEP"

// config wakeup after wakeup_time_ms
void deep_sleep_init(uint64_t wakeup_time_ms) {
  ESP_LOGI(TAG, "Configuring deep sleep for %llu ms", wakeup_time_ms);
  esp_sleep_enable_timer_wakeup(wakeup_time_ms * 1000); // ms to us
}

// enter deep sleep
void deep_sleep_start() {
  ESP_LOGI(TAG, "Entering deep sleep now.");
  esp_deep_sleep_start();
}
