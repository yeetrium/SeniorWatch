#include "input.h"
#include "message.h"
  
void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  message_send(0);
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  message_send(1);
}

void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  message_send(2);
}
  
void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
  
void input_init(Window *window) {
  window_set_click_config_provider(window, click_config_provider);
}

void input_deinit() {
}