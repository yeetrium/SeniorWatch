#include <pebble.h>
  
#include "message.h"
#include "status.h"
#include "input.h"
#include "notification.h"
  
Window *window_main;

const char *status_default = "Looking good!";

void window_load(Window *window) {
  status_init(window);
  status_update(status_default);
}

void window_unload(Window *window) {
  status_deinit();
}

void init() {
   // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  window_main = window_create();
  
  window_set_window_handlers(window_main, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  input_init(window_main);
  initNotification();
  
  window_stack_push(window_main, true);
}

void deinit() {
  input_deinit();
  window_destroy(window_main);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}