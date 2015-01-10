#pragma once
  
typedef enum {
  UP,
  SELECT,
  DOWN,
  EMERGENCY
} message_type;

void message_send(message_type type);