#include "state_machine.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Application components
#include "alarm.h"
#include "deep_sleep.h"
#include "motion_logic.h"
#include "motor_driver.h"
#include "ultrasonic_sensor.h"
#include "web_server.h"

#define TAG "STATE_MACHINE"

#define CONFIG_BUTTON_GPIO GPIO_NUM_0 // GPIO0 is often the BOOT button
#define CONFIG_TIMEOUT_MS 300000      // 5 minutes timeout for config mode

// External hardware configurations (should be defined in a common header)
extern motor_t left_motor;
extern motor_t right_motor;
extern ultrasonic_sensor_t left_sensor;
extern ultrasonic_sensor_t right_sensor;

// Current system state
static system_state_t current_state = STATE_IDLE;

// Function to handle the IDLE state
static void handle_idle_state(void) {
  ESP_LOGI(TAG, "Entering IDLE state");

  // Make sure motors are stopped
  motor_stop(&left_motor);
  motor_stop(&right_motor);

  // Start configuration mode (web server)
  web_server_init();
  web_server_start();

  // Set up configuration timeout
  uint32_t start_time = esp_log_timestamp();
  uint32_t current_time;
  bool config_timeout = false;

  // Wait in idle state until configuration is done or timeout
  while (!config_timeout) {
    // Check if timeout has occurred
    current_time = esp_log_timestamp();
    if ((current_time - start_time) > CONFIG_TIMEOUT_MS) {
      config_timeout = true;
      ESP_LOGI(TAG, "Configuration timeout reached");
    }

    // Check if alarm has been configured
    alarm_config_t *config = alarm_get_config();
    if (config->enabled) {
      ESP_LOGI(TAG, "Alarm configured for %02d:%02d", config->hour,
               config->minute);
      // Stop the web server before transitioning
      web_server_stop();

      // Transition to SLEEPING state
      current_state = STATE_SLEEPING;
      return;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  // If timeout occurred and no alarm was set, stay in IDLE
  web_server_stop();
  ESP_LOGI(TAG, "No alarm configured, staying in IDLE state");
}

// Function to handle the SLEEPING state
static void handle_sleeping_state(void) {
  ESP_LOGI(TAG, "Entering SLEEPING state");

  // Get alarm configuration
  alarm_config_t *config = alarm_get_config();

  if (!config->enabled) {
    ESP_LOGI(TAG, "No alarm configured, returning to IDLE state");
    current_state = STATE_IDLE;
    return;
  }

  // Set up deep sleep to wake at alarm time
  ESP_LOGI(TAG, "Setting up deep sleep until %02d:%02d", config->hour,
           config->minute);
  deep_sleep_wakeup_at(config->hour, config->minute);

  // Also enable wakeup on button press to enter config mode
  deep_sleep_configure_button_wakeup(CONFIG_BUTTON_GPIO);

  // Enter deep sleep (this function does not return unless ESP32 wakes up)
  deep_sleep_start();

  // If we reach here, ESP32 has woken up from sleep
  wakeup_reason_t reason = deep_sleep_get_wakeup_reason();

  if (reason == WAKEUP_TIMER) {
    // Woke up due to alarm time
    ESP_LOGI(TAG, "Woke up due to alarm timer");
    current_state = STATE_ALARM_ACTIVE;
  } else {
    // Woke up due to button or other reason
    ESP_LOGI(TAG, "Woke up due to button press or other reason");
    current_state = STATE_IDLE;
  }
}

// Function to handle the ALARM_ACTIVE state
static void handle_alarm_active_state(void) {
  ESP_LOGI(TAG, "Entering ALARM_ACTIVE state");

  // Start the alarm (buzzer)
  alarm_start();

  // Main alarm loop
  while (alarm_is_triggered()) {
    // Run obstacle avoidance logic
    motion_logic_update(&left_sensor, &right_sensor, &left_motor, &right_motor);

    // Check if stop button is pressed (handled in alarm_is_triggered)
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  // Alarm has been stopped
  ESP_LOGI(TAG, "Alarm stopped, returning to IDLE state");

  // Stop motors
  motor_stop(&left_motor);
  motor_stop(&right_motor);

  // Transition back to IDLE state
  current_state = STATE_IDLE;
}

void state_machine_init(void) {
  // Determine initial state based on wakeup reason
  wakeup_reason_t wakeup_reason = deep_sleep_get_wakeup_reason();

  if (wakeup_reason == WAKEUP_TIMER) {
    // Woke up due to alarm time
    current_state = STATE_ALARM_ACTIVE;
  } else {
    // Woke up due to button or first boot
    current_state = STATE_IDLE;
  }
}

void state_machine_run(void) {
  // Main state machine loop
  while (1) {
    switch (current_state) {
    case STATE_IDLE:
      handle_idle_state();
      break;

    case STATE_SLEEPING:
      handle_sleeping_state();
      break;

    case STATE_ALARM_ACTIVE:
      handle_alarm_active_state();
      break;

    default:
      ESP_LOGE(TAG, "Unknown state: %d", current_state);
      current_state = STATE_IDLE;
      break;
    }

    // Small delay to prevent watchdog timeouts
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

system_state_t state_machine_get_state(void) { return current_state; }
