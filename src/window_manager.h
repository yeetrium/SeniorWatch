#pragma once
  
#include <pebble.h>
  
typedef enum {
  FACE,
  STATUS,
  MENU,
} window_type;

window_type window_manager_get_top_type();

void window_manager_init();
void window_manager_deinit();

void window_manager_push(window_type type, bool animated);
void window_manager_pop(bool animated);