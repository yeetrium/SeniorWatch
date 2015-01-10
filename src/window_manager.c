#include "window_manager.h"
#include "face.h"
#include "status.h"
#include "input.h"

Window *window_main;
Window *window_status;

void window_manager_push(window_type type, bool animated) {
  switch(type) {
    case FACE:
      window_stack_push(window_main, animated);
      break;
    case STATUS:
      window_stack_push(window_status, animated);
      break;
  }
}

void window_manager_pop(bool animated) {
  window_stack_pop(animated);
}

void window_manager_() {
  window_stack_pop(true);
}

void window_load_main(Window *window) {
  face_init(window);
}

void window_unload_main(Window *window) {
  face_deinit();
}

void window_load_status(Window *window) {
  status_init(window);
}

void window_unload_status(Window *window) {
  status_deinit();
}

void window_create_main() {
  window_main = window_create();
  
  window_set_window_handlers(window_main, (WindowHandlers) {
    .load = window_load_main,
    .unload = window_unload_main
  });
  
  window_set_background_color(window_main, GColorWhite);
}

void window_create_status() {
  window_status = window_create();
  
  window_set_window_handlers(window_status, (WindowHandlers) {
    .load = window_load_status,
    .unload = window_unload_status
  });
  
  window_set_background_color(window_status, GColorWhite);
}

void window_manager_init() {
  window_create_main();
  window_create_status();
  
  input_init_main(window_main);
  input_init_status(window_status);
  
  window_stack_push(window_main, true);
}

void window_manager_deinit() {
  window_destroy(window_main);
  window_destroy(window_status);
  
  input_deinit();
}