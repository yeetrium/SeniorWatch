#pragma once

#include <pebble.h>

void face_init(Window *window);
void face_deinit();
void face_front();
void face_greeting_init();
void face_time_init();
void face_time_update(const char* time_buffer);
void face_greeting_update(const char *text);