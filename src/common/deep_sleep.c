#include "deep_sleep.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_system.h"

void enter_deep_sleep() {
  esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 seconds
  ESP_LOGI("SLEEP", "Entering deep sleep for 10 seconds...");
  esp_deep_sleep_start();
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  switch (cause) {
  case ESP_SLEEP_WAKEUP_TIMER:
    ESP_LOGI("WAKE", "Wakeup caused by timer");
    break;
  default:
    ESP_LOGI("WAKE", "Wakeup not from timer, cause: %d", cause);
    break;
  }
}
