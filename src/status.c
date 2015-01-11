#include "status.h"
#include "window_manager.h"
  
Layer *layer_status;
ActionBarLayer *action_bar;
TextLayer *text_layer_status;
InverterLayer *layer_flasher;
GBitmap *icon_bitmap;

AppTimer *status_timer = NULL;

void status_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  layer_status = layer_create(bounds);
  
  layer_add_child(window_layer, layer_status);
  
  bounds.origin.x = 5;
  bounds.origin.y += bounds.size.h * 0.4;
  bounds.size.w -= 25;
  bounds.size.h *= 0.6;
  
  text_layer_status = text_layer_create(bounds);
  text_layer_set_background_color(text_layer_status, GColorClear);
  text_layer_set_font(text_layer_status, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(text_layer_status, GColorBlack);
  text_layer_set_text_alignment(text_layer_status, GTextAlignmentCenter);
  
  icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHECKMARK);
  
  action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(action_bar, window);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, icon_bitmap);
  
  layer_add_child(layer_status, text_layer_get_layer(text_layer_status));
}

void status_deinit() {
  text_layer_destroy(text_layer_status);
  layer_destroy(layer_status);
}

void status_update(const char *status) {
  text_layer_set_text(text_layer_status, status);
}

void status_push_update(const char *status) {
  window_manager_push(STATUS, false);
  status_update(status);
}

void status_pop() {
  status_timer = NULL;
  window_manager_pop(true);
}