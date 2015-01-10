#include "message.h"
  
TextLayer *text_layer_message;

void message_init(Window *window) {
  text_layer_message = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(text_layer_message, GColorClear);
  text_layer_set_text_color(text_layer_message, GColorBlack);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer_message));
}

void message_deinit() {
  text_layer_destroy(text_layer_message);
}

void message_update(const char *message) {
  text_layer_set_text(text_layer_message, message);
}