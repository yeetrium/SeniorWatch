#pragma once
  
#include <pebble.h>

void status_init(Window *window);
void status_deinit();
void status_update(const char *status);

void status_input_subscribe(ClickConfigProvider);

// show it loudly
void status_push_update(const char *status);
void status_pop();