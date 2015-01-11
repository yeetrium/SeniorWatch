#include "input.h"
#include "status.h"
#include "message.h"
#include "notification.h"
#include "menu.h"
  
const int doubletap_interval = 250;
  
bool pressed_up = false;
bool pressed_select = false;
bool pressed_down = false;

bool emergency_doubletap = false;

const uint32_t vibes_pattern_double_long[] = { 400, 100, 400 };

void vibes_double_long_pulse() {
  VibePattern pat = {
    .durations = vibes_pattern_double_long,
    .num_segments = ARRAY_LENGTH(vibes_pattern_double_long),
  };
  vibes_enqueue_custom_pattern(pat);
}

void doubletap_end() {
  emergency_doubletap = false;
  if(!emergency_doubletap) {
    status_push_update("Sending emergency message...");
    message_send(EMERGENCY);
    vibes_double_pulse();
  }
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
      vibes_double_long_pulse();
      send_connect_event_to_phone(PHONE_CALL_KEY, "911");
    } else {
      emergency_doubletap = true;
      app_timer_register(doubletap_interval, doubletap_end, NULL);
    }
  } else {
    if(pressed_up)          message_send(UP);
    else if(pressed_select) menu_push_update();
    else if(pressed_down)   message_send(DOWN);
  }
  
  pressed_up = pressed_select = pressed_down = false;
}

void menu_release_handler(ClickRecognizerRef recognizer, void *context) {
  if(pressed_up)          menu_scroll_up();
  else if(pressed_down)   menu_scroll_down();
  
  pressed_up = pressed_select = pressed_down = false;
}

void status_release_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "STATUS ACTION");
  if(pressed_select) {
    status_pop();
  }
}
  
void click_config_provider_main(void *context) {
  // Register the ClickHandlers
  window_raw_click_subscribe(BUTTON_ID_UP, up_press_handler, release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_press_handler, release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_press_handler, release_handler, NULL);
}

void click_config_provider_status(void *context) {
  // Register the ClickHandlers
  window_raw_click_subscribe(BUTTON_ID_UP, up_press_handler, status_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_press_handler, status_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_press_handler, status_release_handler, NULL);
}

void click_config_provider_menu(void *context) {
  // Register the ClickHandlers
  window_raw_click_subscribe(BUTTON_ID_UP, up_press_handler, menu_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_press_handler, menu_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_press_handler, menu_release_handler, NULL);
}
  
void input_init_main(Window *window) {
  window_set_click_config_provider(window, click_config_provider_main);
}

void input_init_status(Window *window) {
  window_set_click_config_provider(window, click_config_provider_status);
  status_input_subscribe(click_config_provider_status);
}

void input_init_menu(Window *window) {
  window_set_click_config_provider(window, click_config_provider_menu);
}

void input_deinit() {
}