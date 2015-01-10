#include "status.h"
  
TextLayer *text_layer_status;

void status_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  GRect text_bounds = layer_get_bounds(window_layer);
  
  text_bounds.origin.x = 5;
  text_bounds.origin.y += text_bounds.size.h * 0.5;
  text_bounds.size.w -= 10;
  text_bounds.size.h *= 0.5;
  
  text_layer_status = text_layer_create(text_bounds);
  text_layer_set_background_color(text_layer_status, GColorClear);
  text_layer_set_font(text_layer_status, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(text_layer_status, GColorBlack);
  text_layer_set_text_alignment(text_layer_status, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(text_layer_status));
}

void status_deinit() {
  text_layer_destroy(text_layer_status);
}

void status_update(const char *status) {
  text_layer_set_text(text_layer_status, status);
}