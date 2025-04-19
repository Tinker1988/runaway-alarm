#include "push_button.h"

void button_init(button_t *button) {
  gpio_init(button->pin);
  gpio_set_dir(button->pin, GPIO_IN);
  gpio_pull_up(button->pin); // Default to pull-up; works well with active-low
}

bool button_is_pressed(button_t *button) {
  bool level = gpio_get(button->pin);
  return button->active_low ? !level : level;
}
