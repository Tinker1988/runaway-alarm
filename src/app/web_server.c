#include "web_server.h"
#include "alarm.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <string.h>

#define TAG "WEB_SERVER"

#define WIFI_SSID "RunawayAlarm"
#define WIFI_PASS "alarmclock"
#define MAX_CONN_RETRY 5

static httpd_handle_t server = NULL;

// HTML page for alarm configuration
static const char *html_page =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<title>Runaway Alarm Clock</title>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<style>"
    "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; "
    "text-align: center; }"
    "h1 { color: #333; }"
    "form { max-width: 300px; margin: 0 auto; padding: 20px; }"
    "label { display: block; margin-bottom: 10px; }"
    "input[type='time'] { width: 100%; padding: 8px; margin-bottom: 20px; }"
    "input[type='submit'] { background-color: #4CAF50; color: white; padding: "
    "10px 15px; border: none; cursor: pointer; }"
    "</style>"
    "</head>"
    "<body>"
    "<h1>Runaway Alarm Clock</h1>"
    "<form action='/set_alarm' method='post'>"
    "<label for='alarm_time'>Set Alarm Time:</label>"
    "<input type='time' id='alarm_time' name='alarm_time' required>"
    "<input type='submit' value='Set Alarm'>"
    "</form>"
    "</body>"
    "</html>";

// Handler for the root path (/)
static esp_err_t root_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  httpd_resp_send(req, html_page, strlen(html_page));
  return ESP_OK;
}

// Handler for setting the alarm
static esp_err_t set_alarm_handler(httpd_req_t *req) {
  char content[100];
  int recv_size = req->content_len;

  if (recv_size > sizeof(content) - 1) {
    recv_size = sizeof(content) - 1;
  }

  int ret = httpd_req_recv(req, content, recv_size);
  if (ret <= 0) {
    return ESP_FAIL;
  }
  content[ret] = '\0';

  // Parse the form data to extract alarm_time
  // Format will be "alarm_time=HH:MM"
  char *time_str = strstr(content, "alarm_time=");
  if (time_str) {
    time_str += 11; // Move past "alarm_time="
    int hour = atoi(time_str);
    time_str = strchr(time_str, ':');
    int minute = 0;
    if (time_str) {
      minute = atoi(time_str + 1);
    }

    // Set the alarm
    alarm_set(hour, minute);

    ESP_LOGI(TAG, "Alarm set for %02d:%02d", hour, minute);

    // Redirect back to the main page with a success message
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
  }

  // If we couldn't parse the time, return an error
  httpd_resp_send_500(req);
  return ESP_FAIL;
}

// Register URI handlers
static httpd_uri_t root = {
    .uri = "/", .method = HTTP_GET, .handler = root_handler, .user_ctx = NULL};

static httpd_uri_t set_alarm = {.uri = "/set_alarm",
                                .method = HTTP_POST,
                                .handler = set_alarm_handler,
                                .user_ctx = NULL};

// Function to start the server
static httpd_handle_t start_webserver(void) {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.lru_purge_enable = true;

  // Start the httpd server
  ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
  if (httpd_start(&server, &config) == ESP_OK) {
    // Set URI handlers
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &set_alarm);
    return server;
  }

  ESP_LOGI(TAG, "Error starting server!");
  return NULL;
}

// Wi-Fi event handler
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
    wifi_event_ap_staconnected_t *event =
        (wifi_event_ap_staconnected_t *)event_data;
    ESP_LOGI(TAG, "Station " MACSTR " joined, AID=%d", MAC2STR(event->mac),
             event->aid);
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    wifi_event_ap_stadisconnected_t *event =
        (wifi_event_ap_stadisconnected_t *)event_data;
    ESP_LOGI(TAG, "Station " MACSTR " left, AID=%d", MAC2STR(event->mac),
             event->aid);
  }
}

// Initialize Wi-Fi in AP mode
static void wifi_init_ap(void) {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_ap();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

  wifi_config_t wifi_config = {
      .ap =
          {
              .ssid = WIFI_SSID,
              .ssid_len = strlen(WIFI_SSID),
              .password = WIFI_PASS,
              .max_connection = 4,
              .authmode = WIFI_AUTH_WPA_WPA2_PSK,
              .channel = 1,
          },
  };

  if (strlen(WIFI_PASS) == 0) {
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  }

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "Wi-Fi AP started with SSID: %s password: %s channel: %d",
           WIFI_SSID, WIFI_PASS, 1);
}

void web_server_init(void) {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Initialize WiFi as AP
  wifi_init_ap();
}

void web_server_start(void) { server = start_webserver(); }

void web_server_stop(void) {
  if (server) {
    httpd_stop(server);
    server = NULL;
  }
}
