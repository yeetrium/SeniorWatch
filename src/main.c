#include <pebble.h>
  
#include "message.h"
#include "status.h"
#include "input.h"
#include "notification.h"
  
Window *window_main;

const char *status_default = "Looking good!";
// TODO: Add real greetings
const char *greetings[8] = { 
  "Make sure to stretch!",
  "Did you take your medication?",
  "Greeting3",
  "Greeting4",
  "Greeting5",
  "Greeting6",
  "Greeting7",
  "Greeting8",
};

static void tick_handler(struct tm *tick_time, TimeUnits units_change) {
  // TODO: Pre-condition is that there are currenlty no notifications
  status_update(greetings[rand() % 9]);
}

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
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
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