idf_component_register(
  SRCS 
    "main.c"
    "alarm.c"
    "motion_logic.c"
    "web_server.c"
    "state_machine.c"
  INCLUDE_DIRS 
    "." "../common/" "../drivers/"
  REQUIRES
    driver
    esp_http_server
    nvs_flash
    esp_netif
    esp_wifi
    esp_timer
)
