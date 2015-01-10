#include <pebble.h>
  
#include "message.h"
#include "status.h"
#include "input.h"
#include "notification.h"
  
Window *window_main;
TextLayer *text_time_layer;

static PropertyAnimation *up_animation;
static PropertyAnimation *down_animation;

#define ConstantGRect(x, y, w, h) {{(x), (y)}, {(w), (h)}}

GRect UPPER_TO_RECT = ConstantGRect(0, 5, 144, 168-5);
GRect LOWER_TO_RECT = ConstantGRect(0, 112, 144, 168-112);

#define ANIMATION_DURATION_IN_MS 1500

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

static void init_animations() {
  
  Layer *layer = text_layer_get_layer(text_time_layer);

  up_animation = property_animation_create_layer_frame(layer, &LOWER_TO_RECT, &UPPER_TO_RECT);
  animation_set_duration((Animation*) up_animation, ANIMATION_DURATION_IN_MS);
  animation_set_curve((Animation*) up_animation, AnimationCurveEaseOut);

  down_animation = property_animation_create_layer_frame(layer, &UPPER_TO_RECT, &LOWER_TO_RECT);
  animation_set_duration((Animation*) down_animation, ANIMATION_DURATION_IN_MS);
  animation_set_curve((Animation*) down_animation, AnimationCurveEaseOut);
}

static void schedule_animation(struct tm *tick_time) {

  static bool use_upper_to_rect = false;

  int32_t animation_delay_required_in_ms = 60000 - (tick_time->tm_sec * 1000) - (ANIMATION_DURATION_IN_MS - 100);

  if (animation_delay_required_in_ms < 0) {
    // In the hopefully rare case that we don't have time to schedule the animation
    // before the value is due to change we just let it change in place.
    return;
  }

  if (use_upper_to_rect) {
    animation_set_delay((Animation*) up_animation, animation_delay_required_in_ms);
    animation_schedule((Animation*) up_animation);
  } else {
    animation_set_delay((Animation*) down_animation, animation_delay_required_in_ms);
    animation_schedule((Animation*) down_animation);
  }

  use_upper_to_rect = !use_upper_to_rect;

}


static void tick_handler(struct tm *tick_time, TimeUnits units_change) {
  
  static char time_text[] = "00:00";

  strftime(time_text, sizeof(time_text), "%R", tick_time);
  text_layer_set_text(text_time_layer, time_text);

  schedule_animation(tick_time);
  
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
  
  window_set_background_color(window_main, GColorBlack);

  text_time_layer = text_layer_create(UPPER_TO_RECT);
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(window_main), text_layer_get_layer(text_time_layer));

  init_animations();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  input_init(window_main);
  initNotification();
  
  window_stack_push(window_main, true);
}

void deinit() {
  property_animation_destroy(up_animation);
  property_animation_destroy(down_animation);

  text_layer_destroy(text_time_layer);
  
  input_deinit();
  window_destroy(window_main);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}