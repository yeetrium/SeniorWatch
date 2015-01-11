#pragma once

#include <pebble.h>
   
typedef enum {
  UNUSABLE_NOTIFICATION_KEY,
  GENERAL_NOTIFICATION_KEY,
  RESPONSE_NOTIFICATION_KEY,
  PHONE_CALL_KEY,
  UPDATE_TASK_KEY
}notification_key;
  
void initNotification(void);
void send_connect_event_to_phone(int key, char *message);