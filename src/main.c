#include <pebble.h>
  
#include "message.h"
#include "notification.h"
#include "window_manager.h"

void init() {
   // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  window_manager_init();

  initNotification();
}

void deinit() {
  window_manager_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}