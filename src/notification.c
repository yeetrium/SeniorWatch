#include <pebble.h>
  
#include "status.h"
  
#define RESPONSE_NOTIFICATION_KEY 0
#define GENERAL_NOTIFICATION_KEY 1
#define PHONE_CALL_KEY 2

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message received!");
  Tuple *data = dict_read_first(iterator);
  
  while (data != NULL) {
    // Size and storage of input message
    // match size with char limit from
    // companion application
    static char s_buffer[42];
    
    // Switch t->key to manage
    // different types of messages?
    switch (data->key) {
      // Optimize calls to status_udpate()
      // by updating status either way?
      case RESPONSE_NOTIFICATION_KEY:
        // TODO: Require a response from watch user
        break;
      case GENERAL_NOTIFICATION_KEY:
        snprintf(s_buffer, sizeof(s_buffer), "'%s'", data->value->cstring);
        status_update(s_buffer);
        vibes_short_pulse();
        break;
      case PHONE_CALL_KEY:
        // TODO: Vibrate a while or
        // until phone call is accepted
        break;
      default:
        // Unknown key
        break;
    }
    
    data = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void send_connect_event_to_phone(char *message) {
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if(iter == NULL) {
		return;
	}
	
  // Replace 0 with keys
	dict_write_cstring(iter, 0, message);
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void initNotification(void) {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
}