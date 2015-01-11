#pragma once
  
#include <pebble.h>

void menu_init(Window *window);
void menu_deinit(void);
uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void  *data);
uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data);
void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
bool menu_get_visibility(void);
void menu_push_update(void);
void menu_pop(void);
void menu_scroll_up(void);
void menu_scroll_down(void);
void menu_toggle(void);
void menu_update_row(const int row, char *title, char *subtitle);