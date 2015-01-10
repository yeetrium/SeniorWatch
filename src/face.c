#include "face.h"
  
Layer *layer_face;
TextLayer *time_layer;
TextLayer *greeting_layer;

// TODO: Add real greetings
const char *greetings[8] = { 
  "You're going to do great today.",
  "Get some sunshine, beautiful!",
  "It's a small world after all...",
  "Greeting4",
  "Greeting5",
  "Greeting6",
  "Greeting7",
  "Greeting8",
};

void face_greeting_init();

void face_time_init();
void face_time_update();

void face_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  layer_face = layer_create(bounds);
  
  layer_add_child(window_layer, layer_face);
  
  // initiate modules
  //face_time_init();
  face_greeting_init();
}

void face_deinit() {
  text_layer_destroy(time_layer);
  layer_destroy(layer_face);
}

// greeting

void face_greeting_hide() {
  text_layer_set_text(greeting_layer, "");
}

void face_greeting_init() {
  // graphics
  GRect bounds = layer_get_bounds(layer_face);
  
  bounds.origin.x = 5;
  bounds.origin.y = 5;
  bounds.size.w -= 10;
  bounds.size.h *= 0.25;
  
  greeting_layer = text_layer_create(bounds);
  text_layer_set_background_color(greeting_layer, GColorClear);
  text_layer_set_font(greeting_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_color(greeting_layer, GColorBlack);
  text_layer_set_text_alignment(greeting_layer, GTextAlignmentLeft);
  
  layer_add_child(layer_face, text_layer_get_layer(greeting_layer));
  
  // TODO: Pre-condition is that there are currenlty no notifications
  text_layer_set_text(greeting_layer, greetings[rand() % 9]);
  
  app_timer_register(5000, face_greeting_hide, NULL);
  //tick_timer_service_subscribe(MINUTE_UNIT, face_greeting_hide);
}

// time

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  face_time_update();
}

void face_time_init() {
  // graphics
  GRect bounds = layer_get_bounds(layer_face);
  
  bounds.origin.x = 5;
  bounds.origin.y = bounds.size.h * 0.5 - bounds.size.h * 0.15;
  bounds.size.w -= 10;
  bounds.size.h *= 0.3;
  
  time_layer = text_layer_create(bounds);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
  layer_add_child(layer_face, text_layer_get_layer(time_layer));
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  face_time_update();
}

void face_time_update() {
   // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  text_layer_set_text(time_layer, buffer);
}