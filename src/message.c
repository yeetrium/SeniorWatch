#include "message.h"
#include "status.h"
#include "notification.h"
#include "window_manager.h"

char *message_up = "I'm hungry. Get me some food please.";
char *message_select = "I'm cold. Get me a jacket please.";
char *message_down = "I need my medications. Get me them please.";
  
char *message_emergency = "Help me NOW!";

void message_send(message_type type) {
  switch(type) {
    case UP:
      status_push_update(message_up);
      //send_connect_event_to_phone(message_up);
      break;
    case SELECT:
      status_update(message_select);
      //send_connect_event_to_phone(message_select);
      break;
    case DOWN:
      status_update(message_down);
      //send_connect_event_to_phone(message_down);
      break;
    case EMERGENCY:
      //status_update(message_emergency);
      send_connect_event_to_phone(message_emergency);
      break;
  }
}