#pragma once
  
#include <pebble.h>
BitmapLayer *s_background_layer;
GBitmap *s_background_bitmap;  

void input_init_main(Window *window);
void input_init_status(Window *window);
void input_init_menu(Window *window);
void input_deinit();