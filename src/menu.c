#include "menu.h"
#include "window_manager.h"
#include "task.h"

MenuLayer *menu_layer;
bool is_visible = false;

void menu_init(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback
  });
  
  // TODO: Toggle visibility where
  // needed to display and hide menu.
  // Should be triggered when back
  // button is pressed
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
  //layer_set_hidden(menu_layer_get_layer(menu_layer), true);
}

void menu_deinit(void) {
  menu_layer_destroy(menu_layer);
}

// Specify number of sections
uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void  *data) {
  return 1;
}

// Specify number of items per section
// Additionally dynamically add or remove items with this
uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return 10;
    default:
      return 0;
  }
}

// Specify height of section
int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Draw sections
void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "Task List");
      break;
    
    default:
      // Non-existent section
      break;
  }
}

// Draw items
void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(0), task_title_get_row(0), NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(1), task_title_get_row(1), NULL);
          break;      
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(2), task_title_get_row(2), NULL);
          break;
        case 3:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(3), task_title_get_row(3), NULL);
          break;
        case 4:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(4), task_title_get_row(4), NULL);
          break;
        case 5:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(5), task_title_get_row(5), NULL);
          break;
        case 6:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(6), task_title_get_row(6), NULL);
          break;
        case 7:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(7), task_title_get_row(7), NULL);
          break;
        case 8:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(8), task_title_get_row(8), NULL);
          break;
        case 9:
          menu_cell_basic_draw(ctx, cell_layer, task_time_get_row(9), task_title_get_row(9), NULL);
          break;
        default:
          // Non-existent row
          break;
      }
      break;
  }
}

// Item is selected
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  menu_toggle();
}

bool menu_get_visibility(void) {
  return is_visible;
}

void menu_push_update(void) {
  window_manager_push(MENU, true);
}

void menu_pop(void) {
  window_manager_pop(true);
}

void menu_scroll_up(void) {
  menu_layer_set_selected_next(menu_layer, true, MenuRowAlignTop, true);
}

void menu_scroll_down(void) {
  menu_layer_set_selected_next(menu_layer, false, MenuRowAlignBottom, true);
}

void menu_toggle(void) {
  is_visible = !is_visible;
  if (is_visible) layer_set_hidden(menu_layer_get_layer(menu_layer), false);
  else layer_set_hidden(menu_layer_get_layer(menu_layer), true);
}
