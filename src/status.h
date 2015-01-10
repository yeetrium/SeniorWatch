#pragma once
  
#include <pebble.h>

void status_init(Window *window);
void status_deinit();
void status_update(const char *status);