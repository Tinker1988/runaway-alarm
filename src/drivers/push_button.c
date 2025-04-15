#include "push_button.h"
#include <stdbool.h>

void push_button_init(push_button_t *button) {
  gpio_init(button->pin);
  gpio_set_dir(button->pin, GPIO_IN);

  // enable pull-up if the button is active low
  if (button->is_active_low) {
    gpio_pull_up(button->pin);
  } else {
    gpio_pull_down(button->pin);
  }

  button->last_state = !button->is_active_low; // Initialize to not pressed
  button->last_debounce_time = 0;

  // Default debounce delay if not set
  if (button->debounce_delay_ms == 0) {
    button->debounce_delay_ms = 50;
  }
}

bool push_button_is_pressed(push_button_t *button) {
  // Read current button state
  bool pin_state = gpio_get(button->pin);

  // Convert based on active high/low configuration
  bool is_pressed = button->is_active_low ? !pin_state : pin_state;

  return is_pressed;
}

bool push_button_was_pressed(push_button_t *button) {
  // Read current button state
  bool pin_state = gpio_get(button->pin);

  // Convert based on active high/low configuration
  bool current_state = button->is_active_low ? !pin_state : pin_state;

  // Get current time
  uint64_t current_time = time_us_64() / 1000; // Convert to ms

  // Check if state changed
  if (current_state != button->last_state) {
    button->last_debounce_time = current_time;
  }

  // Check if debounce time has passed and button is pressed
  bool button_event = false;
  if ((current_time - button->last_debounce_time) > button->debounce_delay_ms) {
    if (current_state && !button->last_state) {
      button_event = true; // Button press detected
    }
  }

  // Update last state
  button->last_state = current_state;

  return button_event;
}
