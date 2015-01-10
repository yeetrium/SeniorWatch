#include "message.h"
#include "status.h"
#include "notification.h"
#include "window_manager.h"
#include "face.h"

char *message_up = "I'm hungry. Get me some food please.";
char *message_select = "I need help. Come when you're available please.";
char *message_down = "I need my medications. Get me them please.";
  
char *message_emergency = "Help me NOW!";

void message_send(message_type type) {
  switch(type) {
    case UP:
      face_greeting_update("Sending 'hungry' message...");
      send_connect_event_to_phone(message_up);
      break;
    case SELECT:
      face_greeting_update("Sending 'help' message...");
      send_connect_event_to_phone(message_select);
      break;
    case DOWN:
      face_greeting_update("Sending 'medications' message...");
      send_connect_event_to_phone(message_down);
      break;
    case EMERGENCY:
      send_connect_event_to_phone(message_emergency);
      break;
  }
}