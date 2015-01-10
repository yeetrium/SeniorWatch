#include "message.h"
#include "status.h"

char *message_up = "I'm hungry. Get me some food please.";
char *message_select = "I'm cold. Get me a jacket please.";
char *message_down = "I need my medications. Get me them please.";
  
char *message_emergency = "I need help NOW!";

void message_send(message_type type) {
  switch(type) {
    case UP:
      status_update(message_up);
      break;
    case SELECT:
      status_update(message_select);
      break;
    case DOWN:
      status_update(message_down);
      break;
    case EMERGENCY:
      status_update(message_emergency);
      break;
  }
}