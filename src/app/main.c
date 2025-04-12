#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "time.h"
#include <stdio.h>
#include <sys/time.h>

// Application components
#include "alarm.h"
#include "deep_sleep.h"
#include "motion_logic.h"
#include "motor_driver.h"
#include "state_machine.h"
#include "ultrasonic_sensor.h"
#include "web_server.h"

#define LEFT_MOTOR_IN1_PIN GPIO_NUM_25
#define LEFT_MOTOR_IN2_PIN GPIO_NUM_26
#define LEFT_MOTOR_PWM_PIN GPIO_NUM_27
#define RIGHT_MOTOR_IN1_PIN GPIO_NUM_14
#define RIGHT_MOTOR_IN2_PIN GPIO_NUM_12
#define RIGHT_MOTOR_PWM_PIN GPIO_NUM_13

#define LEFT_SENSOR_TRIGGER_PIN GPIO_NUM_17
#define LEFT_SENSOR_ECHO_PIN GPIO_NUM_16
#define RIGHT_SENSOR_TRIGGER_PIN GPIO_NUM_4
#define RIGHT_SENSOR_ECHO_PIN GPIO_NUM_2

static const char *TAG = "MAIN";

// Hardware components (accessible by state machine)
motor_t left_motor = {.in1_pin = LEFT_MOTOR_IN1_PIN,
                      .in2_pin = LEFT_MOTOR_IN2_PIN,
                      .pwm_pin = LEFT_MOTOR_PWM_PIN,
                      .pwm_channel = LEDC_CHANNEL_0};

motor_t right_motor = {.in1_pin = RIGHT_MOTOR_IN1_PIN,
                       .in2_pin = RIGHT_MOTOR_IN2_PIN,
                       .pwm_pin = RIGHT_MOTOR_PWM_PIN,
                       .pwm_channel = LEDC_CHANNEL_1};

ultrasonic_sensor_t left_sensor = {.trigger_pin = LEFT_SENSOR_TRIGGER_PIN,
                                   .echo_pin = LEFT_SENSOR_ECHO_PIN};

ultrasonic_sensor_t right_sensor = {.trigger_pin = RIGHT_SENSOR_TRIGGER_PIN,
                                    .echo_pin = RIGHT_SENSOR_ECHO_PIN};

// Initialize time synchronization
static void initialize_sntp(void) {
  ESP_LOGI(TAG, "Initializing SNTP");

  // Only initialize SNTP without network operations
  // This is an alternative approach if you're not planning to use SNTP for now

  // Set a default time without trying to connect to NTP servers
  struct tm timeinfo;
  timeinfo.tm_year = 2023 - 1900;
  timeinfo.tm_mon = 0;
  timeinfo.tm_mday = 1;
  timeinfo.tm_hour = 12;
  timeinfo.tm_min = 0;
  timeinfo.tm_sec = 0;
  time_t t = mktime(&timeinfo);
  struct timeval now = {.tv_sec = t};
  settimeofday(&now, NULL);

  // Set timezone to local time
  setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
  tzset();

  ESP_LOGI(TAG, "System time set to default: 2023-01-01 12:00:00");
}

void app_main(void) {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Initialize system components
  ESP_LOGI(TAG, "Initializing system components");
  deep_sleep_init();
  initialize_sntp();
  alarm_init();

  // Initialize hardware
  motor_init(&left_motor);
  motor_init(&right_motor);
  ultrasonic_init(&left_sensor);
  ultrasonic_init(&right_sensor);

  // Initialize and run the state machine
  state_machine_init();
  state_machine_run();

  // This point should never be reached
  ESP_LOGE(TAG, "State machine exited unexpectedly");
}
