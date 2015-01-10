#pragma once
  
#include <pebble.h>

void message_init(Window *window);
void message_deinit();
void message_update(const char *message);