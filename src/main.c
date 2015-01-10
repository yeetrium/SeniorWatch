#include <pebble.h>
  
#include "message.h"
  
Window *window_main;

const char *message_default = "Looking good!";

void window_load(Window *window) {
  message_init(window);
  message_update(message_default);
}

void window_unload(Window *window) {
  message_deinit();
}

void init() {
  window_main = window_create();
  
  window_set_window_handlers(window_main, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  window_stack_push(window_main, true);
}

void deinit() {
  window_destroy(window_main);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}