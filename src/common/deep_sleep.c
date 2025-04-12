#include "deep_sleep.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include <time.h>

#define TAG "DEEP_SLEEP"
#define CONFIG_BUTTON_GPIO 0  // Define the GPIO for config button

void deep_sleep_init(void) {
    // Check wakeup cause
    ESP_LOGI(TAG, "Initializing deep sleep module");
}

void deep_sleep_wakeup_at(int hour, int minute) {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Calculate time until alarm in seconds
    struct tm alarm_time = timeinfo;
    alarm_time.tm_hour = hour;
    alarm_time.tm_min = minute;
    alarm_time.tm_sec = 0;
    
    time_t alarm_timestamp = mktime(&alarm_time);
    
    // If alarm is in the past, set it for tomorrow
    if (alarm_timestamp <= now) {
        alarm_timestamp += 86400; // Add 24 hours
    }
    
    // Calculate microseconds until alarm
    uint64_t sleep_time_us = (alarm_timestamp - now) * 1000000ULL;
    
    ESP_LOGI(TAG, "Setting deep sleep wakeup for %02d:%02d (in %llu seconds)",
             hour, minute, (alarm_timestamp - now));
    
    // Enable timer wakeup
    esp_sleep_enable_timer_wakeup(sleep_time_us);
}

void deep_sleep_configure_button_wakeup(uint32_t gpio_num) {
    // Enable wakeup on button press (active low)
    esp_sleep_enable_ext0_wakeup(gpio_num, 0);
    ESP_LOGI(TAG, "Enabled wakeup on GPIO %d", gpio_num);
}

wakeup_reason_t deep_sleep_get_wakeup_reason(void) {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI(TAG, "Wakeup caused by timer");
            return WAKEUP_TIMER;
            
        case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI(TAG, "Wakeup caused by button press");
            return WAKEUP_BUTTON;
            
        default:
            ESP_LOGI(TAG, "Wakeup was not caused by deep sleep");
            return WAKEUP_UNDEFINED;
    }
}

void deep_sleep_start(void) {
    ESP_LOGI(TAG, "Entering deep sleep now");
    
    // Small delay to allow log to be printed
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // Enter deep sleep
    esp_deep_sleep_start();
}
