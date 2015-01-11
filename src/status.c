#include "status.h"
#include "window_manager.h"
  
Layer *layer_status;
TextLayer *text_layer_status;
InverterLayer *layer_flasher;

AppTimer *status_timer = NULL;

void status_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  layer_status = layer_create(bounds);
  
  layer_add_child(window_layer, layer_status);
  
  bounds.origin.x = 5;
  bounds.origin.y += bounds.size.h * 0.5;
  bounds.size.w -= 10;
  bounds.size.h *= 0.5;
  
  text_layer_status = text_layer_create(bounds);
  text_layer_set_background_color(text_layer_status, GColorClear);
  text_layer_set_font(text_layer_status, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(text_layer_status, GColorBlack);
  text_layer_set_text_alignment(text_layer_status, GTextAlignmentCenter);
  
  layer_add_child(layer_status, text_layer_get_layer(text_layer_status));
}

void status_deinit() {
  text_layer_destroy(text_layer_status);
  layer_destroy(layer_status);
}

void status_update(const char *status) {
  text_layer_set_text(text_layer_status, status);
}

void status_push_update(const char *status, const int lifespan) {
  window_manager_push(STATUS, false);
  status_update(status);
  if(lifespan > 0) {
    if(status_timer != NULL) app_timer_reschedule(status_timer, lifespan);
    else status_timer = app_timer_register(lifespan, status_pop, NULL);
  } else if(status_timer != NULL) {
    app_timer_cancel(status_timer);
  }
}

void status_pop() {
  status_timer = NULL;
  window_manager_pop(true);
}