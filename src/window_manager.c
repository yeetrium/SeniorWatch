#include <ctype.h>

#include "window_manager.h"
#include "face.h"
#include "status.h"
#include "input.h"
#include "menu.h"
#include "task.h"

Window *window_main;
Window *window_status;
Window *window_menu;

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((int)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(((int)*end))) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

void tick_handler(struct tm *tick_time, TimeUnits units_change) {
  static char s_time_buffer[16];
  static char s_time_daynight_buffer[16];
  static char s_date_buffer[16];
  
  // Displays the current time
  if (clock_is_24h_style()) {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%k:%M", tick_time);
  } else {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%l:%M", tick_time);
  }
  
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %e", tick_time);
  face_time_update(trimwhitespace(s_time_buffer), s_date_buffer);
  
  strftime(s_time_daynight_buffer, sizeof(s_time_daynight_buffer), "%l:%M%p", tick_time);
  task_check(trimwhitespace(s_time_daynight_buffer));
  
  // Add each window's update function if needed
}

window_type window_manager_get_top_type() {
  Window *temp = window_stack_get_top_window();
  
  if(temp == window_status) {
    return STATUS;
  } else if(temp == window_menu) {
    return MENU;
  }
  
  return FACE;
}

void window_manager_push(window_type type, bool animated) {
  if(window_manager_get_top_type() == type) return;
  
  switch(type) {
    case FACE:
      window_stack_push(window_main, animated);
      break;
    case STATUS:
      window_stack_push(window_status, animated);
      break;
    case MENU:
      window_stack_push(window_menu, animated);
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
  input_init_main(window_main);
}

void window_unload_main(Window *window) {
  face_deinit();
}

void window_load_status(Window *window) {
  status_init(window);
  input_init_status(window_status);
}

void window_unload_status(Window *window) {
  status_deinit();
}

void window_load_menu(Window *window) {
  menu_init(window);
  input_init_menu(window_menu);
}

void window_unload_menu(Window *window) {
  menu_deinit();
}

void window_create_main() {
  window_main = window_create();
  
  window_set_window_handlers(window_main, (WindowHandlers) {
    .load = window_load_main,
    .unload = window_unload_main
  });
  
  window_set_background_color(window_main, GColorBlack);
}

void window_create_status() {
  window_status = window_create();
  
  window_set_window_handlers(window_status, (WindowHandlers) {
    .load = window_load_status,
    .unload = window_unload_status
  });
  
  window_set_background_color(window_status, GColorBlack);
}

void window_create_menu() {
  window_menu = window_create();
  
  window_set_window_handlers(window_menu, (WindowHandlers) {
    .load = window_load_menu,
    .unload = window_unload_menu
  });
  
  window_set_background_color(window_menu, GColorWhite);
}

void window_manager_init() {
  window_create_main();
  window_create_status();
  window_create_menu();
  
  window_stack_push(window_main, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void window_manager_deinit() {
  window_destroy(window_main);
  window_destroy(window_status);
  
  input_deinit();
}