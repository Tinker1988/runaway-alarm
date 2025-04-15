#include "pico/stdlib.h"
#include "ultrasonic.h"
#include <stdio.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3

int main() {
  stdio_init_all();

  ultrasonic_config_t sensor = {.trigger_pin = TRIGGER_PIN,
                                .echo_pin = ECHO_PIN};

  printf("Initializing ultrasonic sensor...\n");

  if (!ultrasonic_init(&sensor)) {
    printf("Failed to initialize sensor\n");
    return 1;
  }

  while (true) {
    float distance =
        ultrasonic_measure_distance_cm(&sensor, 30000); // 30 ms timeout

    if (distance > 0.0f) {
      printf("Distance: %.2f cm\n", distance);
    } else {
      printf("Timeout: no echo received\n");
    }

    sleep_ms(500); // Wait before next reading
  }

  ultrasonic_deinit(&sensor);
  return 0;
}
