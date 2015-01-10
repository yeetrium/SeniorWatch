#include "input.h"
#include "status.h"
#include "message.h"
#include "window_manager.h"
  
const int doubletap_interval = 250;
  
bool pressed_up = false;
bool pressed_select = false;
bool pressed_down = false;

bool emergency_doubletap = false;

void doubletap_end() {
  emergency_doubletap = false;
}
  
void up_press_handler(ClickRecognizerRef recognizer, void *context) {
  pressed_up = true;
}

void select_press_handler(ClickRecognizerRef recognizer, void *context) {
  pressed_select = true;
}

void down_press_handler(ClickRecognizerRef recognizer, void *context) {
  pressed_down = true;
}

void release_handler(ClickRecognizerRef recognizer, void *context) {
  if(pressed_up && pressed_select && pressed_down) {
    if(emergency_doubletap) {
      // call 911
      status_push_update("Dialing 911...");
      emergency_doubletap = false;
    } else {
      status_push_update("Emergency message sent!");
      message_send(EMERGENCY);
      emergency_doubletap = true;
      app_timer_register(doubletap_interval, doubletap_end, NULL);
    }
  } else {
    if(pressed_up)          message_send(UP);
    else if(pressed_select) message_send(SELECT);
    else if(pressed_down)   message_send(DOWN);
  }
  
  pressed_up = pressed_select = pressed_down = false;
}
  
void click_config_provider_main(void *context) {
  // Register the ClickHandlers
  window_raw_click_subscribe(BUTTON_ID_UP, up_press_handler, release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_press_handler, release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_press_handler, release_handler, NULL);
}

void click_config_provider_status(void *context) {
  // Register the ClickHandlers
  window_raw_click_subscribe(BUTTON_ID_UP, up_press_handler, release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_press_handler, release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_press_handler, release_handler, NULL);
}
  
void input_init_main(Window *window) {
  window_set_click_config_provider(window, click_config_provider_main);
}

void input_init_status(Window *window) {
  window_set_click_config_provider(window, click_config_provider_status);
}

void input_deinit() {
}