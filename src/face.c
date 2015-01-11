#include "face.h"
  
Layer *layer_face;
TextLayer *time_layer;
TextLayer *greeting_layer;

AppTimer *greeting_timer;

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

void face_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  layer_face = layer_create(bounds);
  layer_add_child(window_layer, layer_face);
  
  // initiate modules
  face_greeting_init();
  face_time_init();
}

void face_deinit() {
  text_layer_destroy(time_layer);
  layer_destroy(layer_face);
}

// greeting

void face_greeting_hide() {
  text_layer_set_text(greeting_layer, "");
}

void face_greeting_update(const char *text) {
  text_layer_set_text(greeting_layer, text);
  int lifespan = 5000 + (strlen(text) * 200);
  
  if(lifespan > 0) {
    if(greeting_timer != NULL) app_timer_reschedule(greeting_timer, lifespan);
    else greeting_timer = app_timer_register(lifespan, face_greeting_hide, NULL);
  } else if(greeting_timer != NULL) {
    app_timer_cancel(greeting_timer);
  }
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
  face_greeting_update(greetings[rand() % 9]);
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
}

void face_time_update(const char* time_buffer) {
  text_layer_set_text(time_layer, time_buffer);
}