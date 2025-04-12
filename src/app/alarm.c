#include "alarm.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <time.h>

#define BUZZER_GPIO GPIO_NUM_13
#define STOP_BUTTON_GPIO GPIO_NUM_14  // Button to stop the alarm
#define TAG "ALARM"

static bool alarm_active = false;
static alarm_config_t alarm_config = {0, 0, false};

// Function to check if button is pressed to stop alarm
static bool is_stop_button_pressed() {
    return gpio_get_level(STOP_BUTTON_GPIO) == 0;  // Assuming active low button
}

void alarm_init(void) {
    // Initialize buzzer
    gpio_reset_pin(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BUZZER_GPIO, 0);
    
    // Initialize stop button with pull-up
    gpio_reset_pin(STOP_BUTTON_GPIO);
    gpio_set_direction(STOP_BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(STOP_BUTTON_GPIO, GPIO_PULLUP_ONLY);
    
    // Load alarm configuration from NVS
    alarm_load_config();
}

void alarm_set(int hour, int minute) {
    alarm_config.hour = hour;
    alarm_config.minute = minute;
    alarm_config.enabled = true;
    
    ESP_LOGI(TAG, "Alarm set for %02d:%02d", hour, minute);
    
    // Save config to NVS
    alarm_save_config();
}

void alarm_start(void) {
    ESP_LOGI(TAG, "Alarm activated!");
    alarm_active = true;
    gpio_set_level(BUZZER_GPIO, 1);
}

void alarm_stop(void) {
    gpio_set_level(BUZZER_GPIO, 0);
    alarm_active = false;
    ESP_LOGI(TAG, "Alarm stopped");
}

bool alarm_is_triggered(void) {
    // Check if stop button is pressed to turn off alarm
    if (alarm_active && is_stop_button_pressed()) {
        ESP_LOGI(TAG, "Stop button pressed");
        alarm_stop();
        return false;
    }
    return alarm_active;
}

bool alarm_should_activate(void) {
    if (!alarm_config.enabled) {
        return false;
    }
    
    // Get current time
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Check if it's alarm time
    if (timeinfo.tm_hour == alarm_config.hour && 
        timeinfo.tm_min == alarm_config.minute && 
        timeinfo.tm_sec < 5) { // 5-second window to trigger
        return true;
    }
    
    return false;
}

void alarm_save_config(void) {
    nvs_handle_t nvs_handle;
    esp_err_t err;
    
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS: %s", esp_err_to_name(err));
        return;
    }
    
    err = nvs_set_i32(nvs_handle, "alarm_hour", alarm_config.hour);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error setting alarm hour: %s", esp_err_to_name(err));
    }
    
    err = nvs_set_i32(nvs_handle, "alarm_min", alarm_config.minute);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error setting alarm minute: %s", esp_err_to_name(err));
    }
    
    err = nvs_set_u8(nvs_handle, "alarm_enabled", alarm_config.enabled ? 1 : 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error setting alarm enabled: %s", esp_err_to_name(err));
    }
    
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error committing NVS: %s", esp_err_to_name(err));
    }
    
    nvs_close(nvs_handle);
}

void alarm_load_config(void) {
    nvs_handle_t nvs_handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS: %s", esp_err_to_name(err));
        return;
    }
    
    int32_t hour, minute;
    uint8_t enabled;
    
    err = nvs_get_i32(nvs_handle, "alarm_hour", &hour);
    if (err == ESP_OK) {
        alarm_config.hour = hour;
    }
    
    err = nvs_get_i32(nvs_handle, "alarm_min", &minute);
    if (err == ESP_OK) {
        alarm_config.minute = minute;
    }
    
    err = nvs_get_u8(nvs_handle, "alarm_enabled", &enabled);
    if (err == ESP_OK) {
        alarm_config.enabled = (enabled == 1);
    }
    
    nvs_close(nvs_handle);
    
    ESP_LOGI(TAG, "Loaded alarm config: %02d:%02d, enabled: %d", 
             alarm_config.hour, alarm_config.minute, alarm_config.enabled);
}

alarm_config_t* alarm_get_config(void) {
    return &alarm_config;
}
